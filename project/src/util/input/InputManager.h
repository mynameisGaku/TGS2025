#pragma once

#include "src/util/math/vector3.h"
#include <string>

#include "src/util/input/KeyDefine.h"

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
	/// 入力情報の更新処理
	/// </summary>
	void InputDataUpdate();

	/// <summary>
	/// 先行入力情報の更新処理
	/// </summary>
	/// <param name="advancedEntry">先行入力の保持時間</param>
	void AdvancedEntryUpdate(const int& padNumber, const float& advancedEntry);

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
	/// <param name="padNumber">パッドの番号</param>
	bool Push(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 押された1フレームのみtrueを返す
	/// </summary>
	/// <param name="name">仮想ボタンの名前</param>
	/// <param name="padNumber">パッドの番号</param>
	bool Push(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 押されている間のみtrueを返す
	/// </summary>
	/// <param name="keyCode">入力情報の種類</param>
	/// <param name="padNumber">パッドの番号</param>
	bool Hold(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 押されている間のみtrueを返す
	/// </summary>
	/// <param name="name">仮想ボタンの名前</param>
	/// <param name="padNumber">パッドの番号</param>
	bool Hold(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 離した1フレームのみtrueを返す
	/// </summary>
	/// <param name="keyCode">入力情報の種類</param>
	/// <param name="padNumber">パッドの番号</param>
	bool Release(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 離した1フレームのみtrueを返す
	/// </summary>
	/// <param name="name">仮想ボタンの名前</param>
	/// <param name="padNumber">パッドの番号</param>
	bool Release(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 同時押しされた場合、trueを返す
	/// </summary>
	/// <param name="keyCodes">同時押しする入力情報の種類</param>
	/// <param name="padNumber">パッドの番号</param>
	bool Simultaneously(const std::list<KeyDefine::KeyCode>& keyCodes, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 仮想ボタンに登録されている入力種類を取得する
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	std::vector<KeyDefine::KeyCode> KeyList(const std::string& name);

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 直近の入力情報を取得する
	/// </summary>
	/// <param name="padNumber">パッドの番号</param>
	std::list<KeyDefine::InputData> LatestInput(const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 先行入力されている入力情報を取得する
	/// </summary>
	bool AdvancedEntry(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 先行入力されている入力情報を取得する
	/// </summary>
	bool AdvancedEntry(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// アナログスティックの傾きを取得する
	/// </summary>
	Vector3 AnalogStick(int padNumber = DX_INPUT_PAD1);

#ifdef _DEBUG

	//==========================================================================================
	// ▼デバッグ

	/// <summary>
	/// デバッグ用に入力情報を描画する
	/// </summary>
	/// <param name="x">描画座標(X軸)</param>
	/// <param name="y">描画座標(Y軸)</param>
	void DrawTest(const float& x, const float& y);

#endif // _DEBUG
};
