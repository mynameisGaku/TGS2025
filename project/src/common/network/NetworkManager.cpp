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

// --- 静的メンバの初期化 ---
SOCKET NetworkManager::g_ListenSock = INVALID_SOCKET;
std::vector<ClientInfo*> NetworkManager::g_Clients;
std::mutex NetworkManager::g_Mutex;
SOCKET NetworkManager::g_Sock = INVALID_SOCKET;

// --- コンストラクタ ---
NetworkManager::NetworkManager()
{
    auto& net = NetworkRef::Inst();
    net.Load(true);

    if (!net.IsNetworkEnable)
        return;

    // WinSockの初期化
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
    {
        Logger::FormatDebugLog("WSAStartup 失敗: %d", WSAGetLastError());
        __debugbreak();
    }

    // --- クライアントとして起動 ---
    if (!net.IsHost)
    {
        // サーバー接続用ソケットの作成
        g_Sock = socket(AF_INET, SOCK_STREAM, 0);
        if (g_Sock == INVALID_SOCKET)
        {
            Logger::FormatDebugLog("ソケット作成失敗: エラーコード: %d", WSAGetLastError());
            __debugbreak();
        }

        // サーバーのアドレス情報を設定
        SOCKADDR_IN addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(net.Port);
        inet_pton(AF_INET, net.HostIP.c_str(), &addr.sin_addr);

        // サーバーに接続
        if (connect(g_Sock, (SOCKADDR*)&addr, sizeof(addr)) != 0)
        {
            Logger::FormatDebugLog("接続失敗: IP[%s], PORT[%d], エラーコード: %d", net.HostIP, net.Port, WSAGetLastError());
            MessageBoxA(NULL, "サーバーへの接続に失敗しました。", "接続エラー", MB_OK | MB_ICONERROR);
            __debugbreak();
        }

        Logger::FormatDebugLog("サーバーに接続しました");

        // 接続成功メッセージをサーバーに送信
        const char* joinMsg = "[System] クライアントが接続しました";
        PacketHeader header = { PACKET_MESSAGE, (int)strlen(joinMsg) + 1 };
        send(g_Sock, (char*)&header, sizeof(header), 0);
        send(g_Sock, joinMsg, header.size, 0);

        // サーバーからの受信処理を別スレッドで開始
        _beginthreadex(NULL, 0, RecvThread, NULL, 0, NULL);
    }
    else
    {
        // --- サーバー（ホスト）として起動 ---

        // クライアント受付用のソケットを作成
        g_ListenSock = socket(AF_INET, SOCK_STREAM, 0);

        // ローカルアドレス構成
        SOCKADDR_IN addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(net.Port);
        addr.sin_addr.s_addr = INADDR_ANY;

        // ソケットにポートを割り当てて、接続待ち状態に
        bind(g_ListenSock, (SOCKADDR*)&addr, sizeof(addr));
        listen(g_ListenSock, SOMAXCONN);

        Logger::FormatDebugLog("サーバー起動: ポート %d で待機中...", ntohs(addr.sin_port));

        // 接続受付処理を別スレッドで行う
        _beginthreadex(NULL, 0, HostAcceptThread, NULL, 0, NULL);
    }
}

// --- デストラクタ ---
NetworkManager::~NetworkManager()
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;

    // サーバーの待ち受けソケットを閉じる
    closesocket(g_ListenSock);
    WSACleanup();

    // クライアント情報を解放
    for (auto client : g_Clients)
        delete client;
    g_Clients.clear();
}

// --- 更新処理 ---
void NetworkManager::Update() {}

// --- 描画処理 ---
void NetworkManager::Draw() {}

void NetworkManager::SendJson(const std::string& json)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;

    PacketHeader header;
    header.type = PACKET_JSON;
    header.size = static_cast<int>(json.size()) + 1;

    // --- クライアントの場合 ---
    if (!net.IsHost && g_Sock != INVALID_SOCKET)
    {
        send(g_Sock, reinterpret_cast<const char*>(&header), sizeof(header), 0);
        send(g_Sock, json.c_str(), header.size, 0);
        return;
    }

    // --- ホストの場合（全クライアントに送信） ---
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

// --- 全クライアントにデータを送信する関数 ---
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

// --- サーバー側：クライアントごとの受信処理スレッド ---
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
        // パケットヘッダーを受信
        int res = recv(sock, (char*)&header, sizeof(header), MSG_WAITALL);
        if (res <= 0) break;

        // メッセージパケットの処理
        if (header.type == PACKET_MESSAGE)
        {
            char* msg = new char[header.size];
            recv(sock, msg, header.size, MSG_WAITALL);

            std::string fullMsg = "[" + info->name + "] " + msg;
            PacketHeader h = { PACKET_MESSAGE, (int)fullMsg.size() + 1 };
            Broadcast(h, fullMsg.c_str(), sock);
            delete[] msg;
        }
        // JSONパケットの処理
        else if (header.type == PACKET_JSON)
        {
            char* jsonData = new char[header.size];
            recv(sock, jsonData, header.size, MSG_WAITALL);
            auto json = RawJson::CreateObject({ {jsonData, header.size} });
            Logger::FormatDebugLog("[受信] JSON: %s", json);
            delete[] jsonData;
        }
        else
        {
            Logger::FormatDebugLog("[受信] 不明なパケットタイプ: %d", header.type);
            break;
        }
    }

    Logger::FormatDebugLog("[切断] %s", info->name.c_str());
    closesocket(sock);

    // クライアント情報を削除
    {
        std::lock_guard<std::mutex> lock(NetworkManager::g_Mutex);
        auto& clients = NetworkManager::g_Clients;
        clients.erase(std::remove(clients.begin(), clients.end(), info), clients.end());
    }

    delete info;
    return 0;
}

// --- クライアント側：サーバーからの受信処理スレッド ---
unsigned __stdcall RecvThread(void* param)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return -1;

    while (true)
    {
        PacketHeader header;
        // パケットヘッダーの受信
        int res = recv(NetworkManager::g_Sock, (char*)&header, sizeof(header), MSG_WAITALL);
        if (res <= 0) break;

        // メッセージ
        if (header.type == PACKET_MESSAGE)
        {
            char* msg = new char[header.size];
            recv(NetworkManager::g_Sock, msg, header.size, MSG_WAITALL);
            Logger::FormatDebugLog("[受信] %s", msg);
            delete[] msg;
        }
        // JSON
        else if (header.type == PACKET_JSON)
        {
            char* jsonData = new char[header.size];
            recv(NetworkManager::g_Sock, jsonData, header.size, MSG_WAITALL);
            auto json = RawJson::CreateObject({ {jsonData, header.size} });
            Logger::FormatDebugLog("[受信] JSON: %s", json);
            delete[] jsonData;
        }
        else
        {
            Logger::FormatDebugLog("[受信] 不明なパケットタイプ: %d", header.type);
        }
    }

    Logger::FormatDebugLog("サーバーとの接続が切断されました");
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
            Logger::FormatDebugLog("accept失敗: %d", WSAGetLastError());
            continue;
        }
        Logger::FormatDebugLog("クライアント接続受付完了");
        _beginthreadex(NULL, 0, ClientThread, (void*)clientSock, 0, NULL);
    }

    return 0;
}
