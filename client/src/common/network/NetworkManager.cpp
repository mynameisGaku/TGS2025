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

// --- 静的メンバの初期化 ---
SOCKET NetworkManager::g_ListenSock = INVALID_SOCKET;
std::vector<ClientInfo*> NetworkManager::g_Clients;
std::mutex NetworkManager::g_Mutex;
SOCKET NetworkManager::g_Sock = INVALID_SOCKET;
bool NetworkManager::g_Running = false;
UINT NetworkManager::g_UUIDGenerator = 0;
std::vector<User> NetworkManager::g_Users;
UINT NetworkManager::g_MyUUID = 0;

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
			Logger::FormatDebugLog("接続失敗: IP[%s], PORT[%d], エラーコード: %d", net.HostIP.c_str(), net.Port, WSAGetLastError());
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
		g_Running = true;
		_beginthreadex(NULL, 0, HostAcceptThread, NULL, 0, NULL);
	}

	// ユーザー情報登録
	// ダイアログボックスを表示してユーザー名を入力してもらう
	DialogBox(NULL, TEXT("DLG1"), 0, (DLGPROC)DlgProc1);

	// サーバーへユーザー情報を送信、追加してもらう
	subscribe(*nameText);
}

// --- デストラクタ ---
NetworkManager::~NetworkManager()
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;


	g_Running = false;

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
		user.IsHost = true; // ホストは自分自身をホストとして登録
		g_Users.push_back(user);

		nlohmann::json broadcast;

		broadcast["Command"] = "SetUserData";
		// 返信する内容として、既存のキャラデータをリストアップ
		for (auto& u : g_Users)
		{
			// replyに現在存在するユーザーデータを配列として登録
			nlohmann::json userJson;
			userJson["Name"] = u.Name;
			userJson["UUID"] = u.UUID;
			userJson["Socket"] = u.Socket;
			userJson["IsHost"] = u.IsHost;

			broadcast["Users"].push_back(userJson);
		}

		std::string str = broadcast.dump();

		// 追加後の配列をまとめて送り、クライアントにセットする
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
	// JSON形式でユーザー追加のコマンドを作成
	nlohmann::json json;
	json["Command"]		= "AddUser";
	json["NeedReply"]	= true;
	json["Name"]		= name;
	// JSONを文字列に変換
	std::string jsonStr = json.dump();
	// JSONを送信
	SendJson(jsonStr);
}

void NetworkManager::SendTransform(const Transform& trs, UINT uuid)
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;
	// TransformデータをJSON形式に変換
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
	// JSONを文字列に変換
	std::string jsonStr = json.dump();
	// JSONを送信
	SendJson(jsonStr);
}

void NetworkManager::SendTransitToPlay()
{
	auto& net = NetworkRef::Inst();
	if (!net.IsNetworkEnable)
		return;
	// ゲーム開始のコマンドをJSON形式で作成
	nlohmann::json json;
	json["Command"] = "TransitToPlay";
	json["NeedReply"] = false;
	// JSONを文字列に変換
	std::string jsonStr = json.dump();
	// JSONを送信
	SendJson(jsonStr);
}

void NetworkManager::SendChangeState(const std::string& state, UINT uuid)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;
	// ステート変更のコマンド作成
	nlohmann::json json;
	json["Command"] = "ChangeState";
	json["NeedReply"] = false;
	json["UUID"] = uuid;
	json["State"] = state;
	// ダンプ
	std::string jsonStr = json.dump();
	// 送信
	SendJson(jsonStr);
}

void NetworkManager::SendChangeSubState(const std::string& state, UINT uuid)
{
    auto& net = NetworkRef::Inst();
    if (!net.IsNetworkEnable)
        return;
    // ステート変更のコマンド作成
    nlohmann::json json;
    json["Command"] = "ChangeSubState";
    json["NeedReply"] = false;
    json["UUID"] = uuid;
    json["State"] = state;
    // ダンプ
    std::string jsonStr = json.dump();
    // 送信
    SendJson(jsonStr);
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
			Broadcast(h, fullMsg.c_str());
			delete[] msg;
		}
		// JSONパケットの処理
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
			Logger::FormatDebugLog("[受信] 不明なパケットタイプ: %d", header.type);
			break;
		}
	}

	Logger::FormatDebugLog("[切断] %s", info->name.c_str());

	// ユーザー情報から削除
	{
		std::lock_guard<std::mutex> lock(NetworkManager::g_Mutex);
		auto it = std::find_if(NetworkManager::g_Users.begin(), NetworkManager::g_Users.end(), [info](const User& user) {
			return user.Socket == info->sock;
		});

		if (it != NetworkManager::g_Users.end()) {
			Logger::FormatDebugLog("[削除] ユーザー: %s, UUID: %u", it->Name.c_str(), it->UUID);
			NetworkManager::g_Users.erase(it);
		}
	}

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

	SOCKET sock = (SOCKET)param;

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

			nlohmann::json json = nlohmann::json::parse(jsonData);
			delete[] jsonData;

			NetworkManager::ClientCommandProcess(json, sock);
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

	while (NetworkManager::g_Running)
	{
		int addrLen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN clientAddr;
		SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			if (!NetworkManager::g_Running) break; // 終了直後のエラーは無視
			Logger::FormatDebugLog("accept失敗: %d", WSAGetLastError());
			continue;
		}

		Logger::FormatDebugLog("クライアント接続受付完了");
		_beginthreadex(NULL, 0, ClientThread, (void*)clientSock, 0, NULL);
	}

	return 0;
}