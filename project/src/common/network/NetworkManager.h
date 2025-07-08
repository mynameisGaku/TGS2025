#pragma once

#include <framework/gameObject.h>
#include <vector>
#include <mutex>
#include <string>

// パケットの種類を表す識別子
enum PacketType
{
	PACKET_MESSAGE = 1, // テキストメッセージ
	PACKET_JSON = 2,    // JSON形式のデータ
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

	void SendJson(const std::string& json);

	static SOCKET g_ListenSock;					// サーバーが接続を待ち受けるためのソケット
	static std::vector<ClientInfo*> g_Clients;	// サーバーに接続中のクライアント一覧
	static std::mutex g_Mutex;					// ミューテックス
	static SOCKET g_Sock;						// クライアントが接続に使用するソケット
};
