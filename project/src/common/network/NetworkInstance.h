#pragma once
#include <string>
#include <src/reference/network/NetworkRef.h>
#include <DxLib.h>

/// <summary>
/// 接続１つにつき、１インスタンス
/// </summary>
class NetworkInstance
{
private:
	friend class NetworkManager;

	// 受け取れるメッセージの最大サイズ
	static const int MAX_MESSAGE_SIZE = 1024;
public:

	void Update();

	// ネットハンドルを取得
	int NetHandle();

	// ハンドルをセット
	void SetHandle(int hNet);

	// バッファにメッセージを格納する
	void SetMessageToBuffer(const char* message)
	{
		strncpy_s(m_MessageBuf, message, MAX_MESSAGE_SIZE - 1);
		m_MessageBuf[MAX_MESSAGE_SIZE - 1] = '\0'; // Ensure null termination
		m_NeedCheck = true;
	}

	// 先頭アドレスが返ります
	char* GetMessageBuffer()
	{
		return &m_MessageBuf[0];
	}

	// チェック必要状態にする
	const void NeedCheck() const { m_NeedCheck = true; }

	// 初期状態にクリア
	void Reset();

	void SetIP(const IPDATA& ip);

    bool CompareCurrentIP(const IPDATA& other) const;
    bool ComparePrevIP(const IPDATA& other) const;

private:
	int m_hNet{};								// ハンドラ
	char m_MessageBuf[MAX_MESSAGE_SIZE]{};		// メッセージのバッファ
	mutable bool m_NeedCheck{};					// チェック必要状態かどうか
	NetworkRef::IP m_CurrentIP{};				// 現在のIPアドレス
	NetworkRef::IP m_PrevIP{};					// 前回接続していたIPアドレス
};