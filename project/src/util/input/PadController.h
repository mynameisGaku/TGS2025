#pragma once

// ◇汎用
#include <string>
#include "../Library/time.h"

// ◇個別で必要な物
#include "KeyDefine.h"

namespace PadController {

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	//==========================================================================================
	// ▼入力情報の取得

	/// <summary>
	/// 現在のパッド入力を確認
	/// </summary>
	/// <param name="keyCode">入力の種類</param>
	/// <returns>押されていたら:True</returns>
	bool CheckPushStatusCurrent(KeyDefine::KeyCode keyCode, int padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 1フレーム前のパッド入力を確認
	/// </summary>
	/// <param name="keyCode">入力の種類</param>
	/// <returns>押されていたら:True</returns>
	bool CheckPushStatusBefore(KeyDefine::KeyCode keyCode, int padNumber = DX_INPUT_PAD1);

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// スティック感度を設定する
	/// </summary>
	void SetStickSensitivity(Vector2 value);

	/// <summary>
	/// パッドの振動を設定する
	/// </summary>
	/// <param name="padNumber">振動させるパッドの識別子</param>
	/// <param name="power">振動の強さ(0～1000)</param>
	/// <param name="time">効果時間(-1の場合、StopVibration()が呼ばれるまでの間振動する)</param>
	/// <param name="effectIndex">振動させるモーターの番号</param>
	inline void SetVibration(int padNumber, int power, float time, int effectIndex = -1) {
		StartJoypadVibration(padNumber, power, static_cast<int>(Time::SecToFrame(time)), effectIndex);
	}

	/// <summary>
	/// パッドの振動を停止する
	/// </summary>
	/// <param name="padNumber">振動させるパッドの識別子</param>
	/// <param name="effectIndex">振動させるモーターの番号</param>
	inline void StopVibration(int padNumber, int effectIndex = -1) {
		StopJoypadVibration(padNumber, effectIndex);
	}

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// スティック感度を取得する
	/// </summary>
	Vector2 StickSensitivity();

	/// <summary>
	/// 左スティックの入力値
	/// </summary>
	/// <param name="num">パッドの番号</param>
	/// <returns>1から-1の間の値</returns>
	Vector2 NormalizedLeftStick(int num = DX_INPUT_PAD1);

	/// <summary>
	/// 右スティックの入力値
	/// </summary>
	/// <param name="num">パッドの番号</param>
	/// <returns>1から-1の間の値</returns>
	Vector2 NormalizedRightStick(int num = DX_INPUT_PAD1);

	/// <summary>
	/// 左トリガーの入力値
	/// </summary>
	/// <param name="num">パッドの番号</param>
	/// <returns>1から0の間の値</returns>
	float NormalizedLeftTrigger(int num = DX_INPUT_PAD1);

	/// <summary>
	/// 右トリガーの入力値
	/// </summary>
	/// <param name="num">パッドの番号</param>
	/// <returns>1から0の間の値</returns>
	float NormalizedRightTrigger(int num = DX_INPUT_PAD1);

	/// <summary>
	/// パッドの入力情報を返す
	/// </summary>
	/// <param name="num">パッドの番号</param>
	/// <returns>入力情報</returns>
	XINPUT_STATE Input(int num = DX_INPUT_PAD1);

	/// <summary>
	/// 引数のパッド番号が0以上かつ、接続されているパッドの数以下の時、Trueを返す
	/// </summary>
	inline const bool CheckPadNumber(int num) { return (0 < num) && (num <= GetJoypadNum()); }
}