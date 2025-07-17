#pragma once

#include <framework/gameObject.h>
#include <vendor/nlohmann/json.hpp>
#include <vector>
#include <mutex>
#include <string>
#include <src/common/network/user/User.h>
#include <src/scene/play/chara/Chara.h>

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
	/// Jsonを送信
	/// </summary>
	/// <param name="json">jsonデータ</param>
	void SendJson(const nlohmann::json& json);

	/// <summary>
	/// ユーザー情報を追加する
	/// </summary>
	/// <param name="name">追加するユーザーの名前</param>
	void SendAddUser(const std::string& name);

	/// <summary>
	/// シーンをプレイシーンに遷移させる
	/// </summary>
	void SendSceneTransitToPlay();

	/// <summary>
	/// キャラのトランスフォームを送信
	/// <para>Clientの場合: サーバーに送信</para>
	/// <para>Hostの場合: クライアント全体に送信</para>
	/// </summary>
	/// <param name="trs">適用するトランスフォーム</param>
	/// <param name="uuid">ターゲットの固有ID</param>
	void SendCharaTransform(const class Transform& trs, const std::string& uuid);
	/// <summary>
	///	キャラのステート変更依頼を送信
	/// <para>Clientの場合: サーバーに送信</para>
	/// <para>Hostの場合: クライアント全体に送信</para>
	/// </summary>
	/// <param name="state">変更先のステート</param>
	/// <param name="uuid">ターゲットの固有ID</param>
	void SendCharaChangeState(const std::string& state, const std::string& uuid);
	/// <summary>
	/// キャラのサブステート変更依頼を送信
	/// <para>Clientの場合: サーバーに送信</para>
	/// <para>Hostの場合: クライアント全体に送信</para>
    /// </summary>
    /// <param name="state">変更先のステート</param>
	/// <param name="uuid">ターゲットの固有ID</param>
    void SendCharaChangeSubState(const std::string& state, const std::string& uuid);
    /// <summary>
    /// キャラのリスポーンステート変更依頼を送信
    /// <para>Clientの場合: サーバーに送信</para>
    /// <para>Hostの場合: クライアント全体に送信</para>
    /// </summary>
    /// <param name="state">変更先のステート</param>
    /// <param name="uuid">ターゲットの固有ID</param>
	void SendCharaChangeRespawnState(const std::string& state, const std::string& uuid);
	/// <summary>
    /// キャラが動いているかどうかのフラグを送信
    /// <para>Clientの場合: サーバーに送信</para>
    /// <para>Hostの場合: クライアント全体に送信</para>
	/// </summary>
    /// <param name="flag">フラグの状態</param>
    /// <param name="uuid">ターゲットの固有ID</param>
	void SendSetCharaMoveFlag(bool flag, const std::string& uuid);
	/// <summary>
    /// キャラの全フラグを送信
    /// <para>Clientの場合: サーバーに送信</para>
    /// <para>Hostの場合: クライアント全体に送信</para>
	/// </summary>
    /// <param name="chara">送信元のキャラ</param>
    /// <param name="uuid">ターゲットの固有ID</param>
	void SendCharaAllFlag(Chara* chara, const std::string& uuid);
	void SendBallTransform();


	static SOCKET					g_ListenSock;		// サーバーが接続を待ち受けるためのソケット
	static std::vector<ClientInfo*> g_Clients;			// サーバーに接続中のクライアント一覧
	static std::mutex				g_Mutex;			// ミューテックス
	static SOCKET					g_Sock;				// クライアントが接続に使用するソケット
	static bool						g_Running;			// 実行中か
	static std::vector<User>		g_Users;			// ホストで管理されるユーザー情報
	static std::string				g_MyUUID;			// 自分のUUID クライアントが自身のUUIDを知るためにあるよ

private:
	void subscribe(const std::string& name);
};
