#pragma once

#include "Util/Vector3.h"
#include <string>

#include "KeyDefine.h"

using namespace KeyDefine;

//==========================================================================================
// ▼定義値

namespace {

	static constexpr float HOLD_TIME = 0.25f;				// ボタンの長押しを判定する時間
	static constexpr float ADVANCED_ENTRY_TIME = 0.2f;		// 先行入力の保持時間

	/// <summary>
	/// 入力情報
	/// </summary>
	struct InputData {
		KeyDefine::KeyInfo keyInfo;	// 入力情報
		float putTime;				// 押されている秒数
		bool isPut;					// 押されているか

		InputData() : InputData(KeyDefine::KeyInfo(), 0.0f, false) {};

		InputData(KeyDefine::KeyInfo info, float time, bool put) :
			keyInfo(info),
			putTime(time),
			isPut(put)
		{
		};
	};

	/// <summary>
	/// 先行入力情報
	/// </summary>
	struct AdvancedEntryInfo {
		KeyDefine::KeyInfo keyInfo;	// 入力情報
		float saveTime;				// 保持する秒数

		AdvancedEntryInfo() : AdvancedEntryInfo(KeyDefine::KeyInfo(), 0.0f) {};

		AdvancedEntryInfo(KeyDefine::KeyInfo info, float time) :
			keyInfo(info),
			saveTime(time)
		{
		};
	};
}

//==========================================================================================

/// <summary>
/// 入力を管理するnamespace
/// </summary>
namespace InputManager {

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	//==========================================================================================
	// ▼入力情報の取得

	/// <summary>
	/// 押された1フレームのみtrueを返す
	/// </summary>
	/// <param name="keyCode">入力情報の種類</param>
	/// <param name="num">パッドの番号</param>
	bool Push(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 押された1フレームのみtrueを返す
	/// </summary>
	/// <param name="name">仮想ボタンの名前</param>
	/// <param name="num">パッドの番号</param>
	bool Push(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 押されている間のみtrueを返す
	/// </summary>
	/// <param name="keyCode">入力情報の種類</param>
	/// <param name="num">パッドの番号</param>
	bool Hold(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 押されている間のみtrueを返す
	/// </summary>
	/// <param name="name">仮想ボタンの名前</param>
	/// <param name="num">パッドの番号</param>
	bool Hold(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 離した1フレームのみtrueを返す
	/// </summary>
	/// <param name="keyCode">入力情報の種類</param>
	/// <param name="num">パッドの番号</param>
	bool Release(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 離した1フレームのみtrueを返す
	/// </summary>
	/// <param name="name">仮想ボタンの名前</param>
	/// <param name="num">パッドの番号</param>
	bool Release(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 同時押しされた場合、trueを返す
	/// </summary>
	/// <param name="keyCodes">同時押しする入力情報の種類</param>
	/// <param name="num">パッドの番号</param>
	bool Simultaneously(std::list<KeyDefine::KeyCode> keyCodes, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 仮想ボタンに登録されている入力種類を取得する
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	std::vector<KeyDefine::KeyCode> KeyList(std::string name);

	//==========================================================================================
	// ▼入力情報の確認

	/// <summary>
	/// 直近の入力情報を更新する
	/// </summary>
	/// <param name="advancedEntry">先行入力の保持時間</param>
	void AdvancedEntryUpdate(int num, float advancedEntry);

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 直近の入力情報を取得する
	/// </summary>
	/// <param name="num">パッドの番号</param>
	std::list<KeyDefine::KeyInfo> LatestInput(int num = DX_INPUT_PAD1);

	/// <summary>
	/// 先行入力されている入力情報を取得する
	/// </summary>
	bool AdvancedEntry(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// 先行入力されている入力情報を取得する
	/// </summary>
	bool AdvancedEntry(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// アナログスティックの傾きを取得する
	/// </summary>
	Vector3 AnalogStick();

	//==========================================================================================
	// ▼デバッグ
	
	/// <summary>
	/// デバッグ用に入力情報を描画する
	/// </summary>
	/// <param name="x">描画座標(X軸)</param>
	/// <param name="y">描画座標(Y軸)</param>
	void DrawTest(float x, float y);

};
