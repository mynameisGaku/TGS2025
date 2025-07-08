#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "NetworkManager.h"
#include <src/reference/network/NetworkRef.h>
#include <src/util/file/json/RawJson.h>
#include <src/util/ptr/PtrUtil.h>
#include <src/util/logger/Logger.h>
#include <process.h>

// --- �ÓI�����o�̏����� ---
SOCKET NetworkManager::g_ListenSock = INVALID_SOCKET;
std::vector<ClientInfo*> NetworkManager::g_Clients;
std::mutex NetworkManager::g_Mutex;
SOCKET NetworkManager::g_Sock = INVALID_SOCKET;

// --- �R���X�g���N�^ ---
NetworkManager::NetworkManager()
{
    auto& net = NetworkRef::Inst();
    net.Load(true);

    if (!net.IsNetworkEnable)
        return;

    // WinSock�̏�����
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
    {
        Logger::FormatDebugLog("WSAStartup ���s: %d", WSAGetLastError());
        __debugbreak();
    }

    // --- �N���C�A���g�Ƃ��ċN�� ---
    if (!net.IsHost)
    {
        // �T�[�o�[�ڑ��p�\�P�b�g�̍쐬
        g_Sock = socket(AF_INET, SOCK_STREAM, 0);
        if (g_Sock == INVALID_SOCKET)
        {
            Logger::FormatDebugLog("�\�P�b�g�쐬���s: �G���[�R�[�h: %d", WSAGetLastError());
            __debugbreak();
        }

        // �T�[�o�[�̃A�h���X����ݒ�
        SOCKADDR_IN addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(net.Port);
        inet_pton(AF_INET, net.HostIP.c_str(), &addr.sin_addr);

        // �T�[�o�[�ɐڑ�
        if (connect(g_Sock, (SOCKADDR*)&addr, sizeof(addr)) != 0)
        {
            Logger::FormatDebugLog("�ڑ����s: IP[%s], PORT[%d], �G���[�R�[�h: %d", net.HostIP, net.Port, WSAGetLastError());
            MessageBoxA(NULL, "�T�[�o�[�ւ̐ڑ��Ɏ��s���܂����B", "�ڑ��G���[", MB_OK | MB_ICONERROR);
            __debugbreak();
        }

        Logger::FormatDebugLog("�T�[�o�[�ɐڑ����܂���");

        // �ڑ��������b�Z�[�W���T�[�o�[�ɑ��M
        const char* joinMsg = "[System] �N���C�A���g���ڑ����܂���";
        PacketHeader header = { PACKET_MESSAGE, (int)strlen(joinMsg) + 1 };
        send(g_Sock, (char*)&header, sizeof(header), 0);
        send(g_Sock, joinMsg, header.size, 0);

        // �T�[�o�[����̎�M������ʃX���b�h�ŊJ�n
        _beginthreadex(NULL, 0, RecvThread, NULL, 0, NULL);
    }
    else
    {
        // --- �T�[�o�[�i�z�X�g�j�Ƃ��ċN�� ---

        // �N���C�A���g��t�p�̃\�P�b�g���쐬
        g_ListenSock = socket(AF_INET, SOCK_STREAM, 0);

        // ���[�J���A�h���X�\��
        SOCKADDR_IN addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(net.Port);
        addr.sin_addr.s_addr = INADDR_ANY;

        // �\�P�b�g�Ƀ|�[�g�����蓖�ĂāA�ڑ��҂���Ԃ�
        bind(g_ListenSock, (SOCKADDR*)&addr, sizeof(addr));
        listen(g_ListenSock, SOMAXCONN);

        Logger::FormatDebugLog("�T�[�o�[�N��: �|�[�g %d �őҋ@��...", ntohs(addr.sin_port));

        // �ڑ���t������ʃX���b�h�ōs��
        _beginthreadex(NULL, 0, HostAcceptThread, NULL, 0, NULL);
    }
}

// --- �f�X�g���N�^ ---
NetworkManager::~NetworkManager()
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;

    // �T�[�o�[�̑҂��󂯃\�P�b�g�����
    closesocket(g_ListenSock);
    WSACleanup();

    // �N���C�A���g�������
    for (auto client : g_Clients)
        delete client;
    g_Clients.clear();
}

// --- �X�V���� ---
void NetworkManager::Update() {}

// --- �`�揈�� ---
void NetworkManager::Draw() {}

void NetworkManager::SendJson(const std::string& json)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;

    PacketHeader header;
    header.type = PACKET_JSON;
    header.size = static_cast<int>(json.size()) + 1;

    // --- �N���C�A���g�̏ꍇ ---
    if (!net.IsHost && g_Sock != INVALID_SOCKET)
    {
        send(g_Sock, reinterpret_cast<const char*>(&header), sizeof(header), 0);
        send(g_Sock, json.c_str(), header.size, 0);
        return;
    }

    // --- �z�X�g�̏ꍇ�i�S�N���C�A���g�ɑ��M�j ---
    if (net.IsHost)
    {
        std::lock_guard<std::mutex> lock(g_Mutex);
        for (auto& client : g_Clients)
        {
            send(client->sock, reinterpret_cast<const char*>(&header), sizeof(header), 0);
            send(client->sock, json.c_str(), header.size, 0);
        }
    }
}

// --- �S�N���C�A���g�Ƀf�[�^�𑗐M����֐� ---
void Broadcast(PacketHeader header, const void* data, SOCKET exclude)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;

    std::lock_guard<std::mutex> lock(NetworkManager::g_Mutex);
    for (auto& client : NetworkManager::g_Clients)
    {
        if (client->sock != exclude)
        {
            send(client->sock, (char*)&header, sizeof(header), 0);
            send(client->sock, (const char*)data, header.size, 0);
        }
    }
}

// --- �T�[�o�[���F�N���C�A���g���Ƃ̎�M�����X���b�h ---
unsigned __stdcall ClientThread(void* param)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return -1;

    SOCKET sock = (SOCKET)param;
    ClientInfo* info = new ClientInfo{ sock, "Unknown" };

    {
        std::lock_guard<std::mutex> lock(NetworkManager::g_Mutex);
        NetworkManager::g_Clients.push_back(info);
    }

    while (true)
    {
        PacketHeader header;
        // �p�P�b�g�w�b�_�[����M
        int res = recv(sock, (char*)&header, sizeof(header), MSG_WAITALL);
        if (res <= 0) break;

        // ���b�Z�[�W�p�P�b�g�̏���
        if (header.type == PACKET_MESSAGE)
        {
            char* msg = new char[header.size];
            recv(sock, msg, header.size, MSG_WAITALL);

            std::string fullMsg = "[" + info->name + "] " + msg;
            PacketHeader h = { PACKET_MESSAGE, (int)fullMsg.size() + 1 };
            Broadcast(h, fullMsg.c_str(), sock);
            delete[] msg;
        }
        // JSON�p�P�b�g�̏���
        else if (header.type == PACKET_JSON)
        {
            char* jsonData = new char[header.size];
            recv(sock, jsonData, header.size, MSG_WAITALL);
            auto json = RawJson::CreateObject({ {jsonData, header.size} });
            Logger::FormatDebugLog("[��M] JSON: %s", json);
            delete[] jsonData;
        }
        else
        {
            Logger::FormatDebugLog("[��M] �s���ȃp�P�b�g�^�C�v: %d", header.type);
            break;
        }
    }

    Logger::FormatDebugLog("[�ؒf] %s", info->name.c_str());
    closesocket(sock);

    // �N���C�A���g�����폜
    {
        std::lock_guard<std::mutex> lock(NetworkManager::g_Mutex);
        auto& clients = NetworkManager::g_Clients;
        clients.erase(std::remove(clients.begin(), clients.end(), info), clients.end());
    }

    delete info;
    return 0;
}

// --- �N���C�A���g���F�T�[�o�[����̎�M�����X���b�h ---
unsigned __stdcall RecvThread(void* param)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return -1;

    while (true)
    {
        PacketHeader header;
        // �p�P�b�g�w�b�_�[�̎�M
        int res = recv(NetworkManager::g_Sock, (char*)&header, sizeof(header), MSG_WAITALL);
        if (res <= 0) break;

        // ���b�Z�[�W
        if (header.type == PACKET_MESSAGE)
        {
            char* msg = new char[header.size];
            recv(NetworkManager::g_Sock, msg, header.size, MSG_WAITALL);
            Logger::FormatDebugLog("[��M] %s", msg);
            delete[] msg;
        }
        // JSON
        else if (header.type == PACKET_JSON)
        {
            char* jsonData = new char[header.size];
            recv(NetworkManager::g_Sock, jsonData, header.size, MSG_WAITALL);
            auto json = RawJson::CreateObject({ {jsonData, header.size} });
            Logger::FormatDebugLog("[��M] JSON: %s", json);
            delete[] jsonData;
        }
        else
        {
            Logger::FormatDebugLog("[��M] �s���ȃp�P�b�g�^�C�v: %d", header.type);
        }
    }

    Logger::FormatDebugLog("�T�[�o�[�Ƃ̐ڑ����ؒf����܂���");
    return 0;
}

unsigned __stdcall HostAcceptThread(void* param)
{
    SOCKET listenSock = NetworkManager::g_ListenSock;

    while (true)
    {
        int addrLen = sizeof(SOCKADDR_IN);
        SOCKADDR_IN clientAddr;
        SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
        if (clientSock == INVALID_SOCKET)
        {
            Logger::FormatDebugLog("accept���s: %d", WSAGetLastError());
            continue;
        }
        Logger::FormatDebugLog("�N���C�A���g�ڑ���t����");
        _beginthreadex(NULL, 0, ClientThread, (void*)clientSock, 0, NULL);
    }

    return 0;
}
