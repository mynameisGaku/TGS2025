#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "NetworkManager.h"
#include <src/reference/network/NetworkRef.h>
#include <src/util/ptr/PtrUtil.h>
#include <src/util/logger/Logger.h>
#include <process.h>
#include <src/common/game/GameManager.h>
#include <vendor/nlohmann/json.hpp>
#include <src/util/editbox/editbox.hpp>
#include <src/scene/play/chara/CharaManager.h>
#include <src/util/transform/Transform.h>

// --- �ÓI�����o�̏����� ---
SOCKET NetworkManager::g_ListenSock = INVALID_SOCKET;
std::vector<ClientInfo*> NetworkManager::g_Clients;
std::mutex NetworkManager::g_Mutex;
SOCKET NetworkManager::g_Sock = INVALID_SOCKET;
bool NetworkManager::g_Running = false;
UINT NetworkManager::g_UUIDGenerator = 0;
std::vector<User> NetworkManager::g_Users;
UINT NetworkManager::g_MyUUID = 0;

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
			Logger::FormatDebugLog("�ڑ����s: IP[%s], PORT[%d], �G���[�R�[�h: %d", net.HostIP.c_str(), net.Port, WSAGetLastError());
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
		g_Running = true;
		_beginthreadex(NULL, 0, HostAcceptThread, NULL, 0, NULL);
	}

	// ���[�U�[���o�^
	// �_�C�A���O�{�b�N�X��\�����ă��[�U�[������͂��Ă��炤
	DialogBox(NULL, TEXT("DLG1"), 0, (DLGPROC)DlgProc1);

	// �T�[�o�[�փ��[�U�[���𑗐M�A�ǉ����Ă��炤
	subscribe(*nameText);
}

// --- �f�X�g���N�^ ---
NetworkManager::~NetworkManager()
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;


	g_Running = false;

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

void NetworkManager::SendJson(const nlohmann::json& json)
{
	SendJson(json.dump());
}

void NetworkManager::subscribe(const std::string& name)
{
	auto& net = NetworkRef::Inst();
	if (net.IsHost)
	{
		User user{};
		user.Name	= name;
		user.UUID	= ++g_UUIDGenerator;
		user.Socket	= g_ListenSock;
		user.IsHost = true; // �z�X�g�͎������g���z�X�g�Ƃ��ēo�^
		g_Users.push_back(user);

		nlohmann::json broadcast;

		broadcast["Command"] = "SetUserData";
		// �ԐM������e�Ƃ��āA�����̃L�����f�[�^�����X�g�A�b�v
		for (auto& u : g_Users)
		{
			// reply�Ɍ��ݑ��݂��郆�[�U�[�f�[�^��z��Ƃ��ēo�^
			nlohmann::json userJson;
			userJson["Name"] = u.Name;
			userJson["UUID"] = u.UUID;
			userJson["Socket"] = u.Socket;
			userJson["IsHost"] = u.IsHost;

			broadcast["Users"].push_back(userJson);
		}

		std::string str = broadcast.dump();

		// �ǉ���̔z����܂Ƃ߂đ���A�N���C�A���g�ɃZ�b�g����
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;

		Broadcast(header, str.c_str());
	}
	else
	{
		SendAddUser(name);
	}
}

void NetworkManager::SendAddUser(const std::string& name)
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;
	// JSON�`���Ń��[�U�[�ǉ��̃R�}���h���쐬
	nlohmann::json json;
	json["Command"]		= "AddUser";
	json["NeedReply"]	= true;
	json["Name"]		= name;
	// JSON�𕶎���ɕϊ�
	std::string jsonStr = json.dump();
	// JSON�𑗐M
	SendJson(jsonStr);
}

void NetworkManager::SendTransform(const Transform& trs, UINT uuid)
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;
	// Transform�f�[�^��JSON�`���ɕϊ�
	nlohmann::json json;
	json["Command"] = "SetTransform";
	json["Position"]["X"] = trs.position.x;
	json["Position"]["Y"] = trs.position.y;
	json["Position"]["Z"] = trs.position.z;
	json["Rotation"]["X"] = trs.rotation.x;
	json["Rotation"]["Y"] = trs.rotation.y;
	json["Rotation"]["Z"] = trs.rotation.z;
	json["Scale"]["X"] = trs.scale.x;
	json["Scale"]["Y"] = trs.scale.y;
	json["Scale"]["Z"] = trs.scale.z;
	json["UUID"] = uuid;
	json["NeedReply"] = false;
	// JSON�𕶎���ɕϊ�
	std::string jsonStr = json.dump();
	// JSON�𑗐M
	SendJson(jsonStr);
}

void NetworkManager::SendTransitToPlay()
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;
	// �Q�[���J�n�̃R�}���h��JSON�`���ō쐬
	nlohmann::json json;
	json["Command"] = "TransitToPlay";
	json["NeedReply"] = false;
	// JSON�𕶎���ɕϊ�
	std::string jsonStr = json.dump();
	// JSON�𑗐M
	SendJson(jsonStr);
}

void NetworkManager::SendChangeState(const std::string& state, UINT uuid)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;
	// �X�e�[�g�ύX�̃R�}���h�쐬
	nlohmann::json json;
	json["Command"] = "ChangeState";
	json["NeedReply"] = false;
	json["UUID"] = uuid;
	json["State"] = state;
	// �_���v
	std::string jsonStr = json.dump();
	// ���M
	SendJson(jsonStr);
}

void NetworkManager::SendChangeSubState(const std::string& state, UINT uuid)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;
    // �X�e�[�g�ύX�̃R�}���h�쐬
    nlohmann::json json;
    json["Command"] = "ChangeSubState";
    json["NeedReply"] = false;
    json["UUID"] = uuid;
    json["State"] = state;
    // �_���v
    std::string jsonStr = json.dump();
    // ���M
    SendJson(jsonStr);
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
			Broadcast(h, fullMsg.c_str());
			delete[] msg;
		}
		// JSON�p�P�b�g�̏���
		else if (header.type == PACKET_JSON)
		{
			char* jsonData = new char[header.size];
			recv(sock, jsonData, header.size, MSG_WAITALL);

			nlohmann::json json = nlohmann::json::parse(jsonData);
			delete[] jsonData;

			NetworkManager::HostCommandProcess(json, sock);
		}
		else
		{
			Logger::FormatDebugLog("[��M] �s���ȃp�P�b�g�^�C�v: %d", header.type);
			break;
		}
	}

	Logger::FormatDebugLog("[�ؒf] %s", info->name.c_str());

	// ���[�U�[��񂩂�폜
	{
		std::lock_guard<std::mutex> lock(NetworkManager::g_Mutex);
		auto it = std::find_if(NetworkManager::g_Users.begin(), NetworkManager::g_Users.end(), [info](const User& user) {
			return user.Socket == info->sock;
		});

		if (it != NetworkManager::g_Users.end()) {
			Logger::FormatDebugLog("[�폜] ���[�U�[: %s, UUID: %u", it->Name.c_str(), it->UUID);
			NetworkManager::g_Users.erase(it);
		}
	}

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

	SOCKET sock = (SOCKET)param;

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

			nlohmann::json json = nlohmann::json::parse(jsonData);
			delete[] jsonData;

			NetworkManager::ClientCommandProcess(json, sock);
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

	while (NetworkManager::g_Running)
	{
		int addrLen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN clientAddr;
		SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			if (!NetworkManager::g_Running) break; // �I������̃G���[�͖���
			Logger::FormatDebugLog("accept���s: %d", WSAGetLastError());
			continue;
		}

		Logger::FormatDebugLog("�N���C�A���g�ڑ���t����");
		_beginthreadex(NULL, 0, ClientThread, (void*)clientSock, 0, NULL);
	}

	return 0;
}