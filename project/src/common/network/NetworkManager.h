#pragma once

#include <framework/gameObject.h>
#include <vendor/nlohmann/json.hpp>
#include <vector>
#include <mutex>
#include <string>
#include <src/common/network/user/User.h>

// パケットの種類を表す識別子
enum PacketType
{
	PACKET_MESSAGE = 1,	// テキストメッセージ
	PACKET_JSON = 2,	// JSON形式のデータ
};

#pragma pack(push, 1)
// パケットのヘッダー構造体（全データの先頭につけて識別）
struct PacketHeader
{
	int type; // データの種類（上の enum）
	int size; // 後に続くデータのバイト数
};
#pragma pack(pop)

// クライアント情報（サーバー側で使用）
struct ClientInfo
{
	SOCKET sock;        // 通信用ソケット
	std::string name;   // クライアントの表示名（初期は "Unknown"）
};

// サーバーから全クライアントへデータを送信する関数
// exclude には送信元のソケットを指定することで、自分には送らないようにできる
void Broadcast(PacketHeader header, const void* data, SOCKET exclude = INVALID_SOCKET);

// クライアントとの通信処理（サーバー側）を行うスレッド関数
unsigned __stdcall ClientThread(void* param);

// クライアントがサーバーからのデータを受信するスレッド関数
unsigned __stdcall RecvThread(void* param);

// ホスト側でクライアントの接続を待ち受けるスレッド関数
unsigned __stdcall HostAcceptThread(void* param);

class NetworkManager : public GameObject
{
public:
	NetworkManager();
	~NetworkManager();

	void Update() override;
	void Draw() override;

	static void HostCommandProcess(nlohmann::json& json, SOCKET sock);
	static void ClientCommandProcess(nlohmann::json& json, SOCKET sock);

	/// <summary>
	/// Jsonを送信
	/// </summary>
	/// <param name="json">ダンプ後の文字列</param>
	void SendJson(const std::string& json);

	/// <summary>
	/// ユーザー情報を追加する
	/// </summary>
	/// <param name="name">追加するユーザーの名前</param>
	void SendAddUser(const std::string& name);

	void SendTransform(const class Transform& trs, UINT uuid);

	void SendTransitToPlay();


	static SOCKET					g_ListenSock;		// サーバーが接続を待ち受けるためのソケット
	static std::vector<ClientInfo*> g_Clients;			// サーバーに接続中のクライアント一覧
	static std::mutex				g_Mutex;			// ミューテックス
	static SOCKET					g_Sock;				// クライアントが接続に使用するソケット
	static bool						g_Running;			// 実行中か
	static UINT						g_UUIDGenerator;	// UUIDのジェネレーター 1から始まる
	static std::vector<User>		g_Users;			// ホストで管理されるユーザー情報
	static UINT						g_MyUUID;			// 自分のUUID クライアントが自身のUUIDを知るためにあるよ

	void subscribe(const std::string& name);
};
