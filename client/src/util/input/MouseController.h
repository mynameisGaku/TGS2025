#pragma once

#include <string>
#include "src/util/input/KeyDefine.h"
#include "src/util/math/vector2.h"

namespace MouseController {

	/// <summary>
	/// マウスの情報
	/// </summary>
	struct MouseInfo {

		Vector2 position;		// マウスの座標(Vector2型)
		int newX, newY;			// マウスの今いる位置を取得する
		int oldX, oldY;			// マウスが元居た位置を取得する
		int moveX, moveY;		// New - Old で移動した距離を取得する
		int wheelRot;			// マウスホイールの回転量
		Vector2 sensitivity;	// マウス感度
		KeyDefine::MouseMovement movement;	// マウスの移動方法
		POINT point;			// マウスの情報
		HWND hWnd;				// ウィンドウの情報

		MouseInfo() :
			position(Vector2::Zero),
			newX(0), newY(0),
			oldX(0), oldY(0),
			moveX(0), moveY(0),
			wheelRot(GetMouseWheelRotVol()),
			sensitivity(Vector2::Ones),
			movement(KeyDefine::MouseMovement::Free),
			point({ 0 }),
			hWnd(0)
		{
		};

		/// <summary>
		/// 感度設定込みの移動量
		/// </summary>
		inline Vector2 Move() const { return Vector2(moveX * (sensitivity.x * 0.1f), moveY * (sensitivity.y * 0.1f)); }
	};

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

	void PushStatusUpdate();

	/// <summary>
	/// マウスをクリックした瞬間を検知する
	/// </summary>
	/// <param name="_keyInput">キーボードの入力情報</param>
	/// <returns>クリックしていたら:True</returns>
	bool CheckClick(KeyDefine::KeyCode keyCode);

	/// <summary>
	/// マウスクリックを長押ししているかを検知する
	/// </summary>
	/// <param name="_keyInput">キーボードの入力情報</param>
	/// <returns>クリックしていたら:True</returns>
	bool CheckHold(KeyDefine::KeyCode keyCode);

	/// <summary>
	/// マウスのクリックが解除されたか検知する
	/// </summary>
	/// <param name="_keyInput">キーボードの入力情報</param>
	/// <returns>クリックを離した瞬間:True</returns>
	bool CheckRelease(KeyDefine::KeyCode keyCode);

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// マウスの動きを設定する
	/// </summary>
	void SetMouseMovement(KeyDefine::MouseMovement move);

	/// <summary>
	/// マウス感度を設定する
	/// </summary>
	void SetMouseSensitivity(Vector2 value);

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// マウスの情報を取得する
	/// </summary>
	const MouseController::MouseInfo Info();

	//==========================================================================================
	// ▼判定

	/// <summary>
	/// マウスとの当たり判定(円)
	/// </summary>
	/// <param name="pos">判定を取りたい座標(中心)</param>
	/// <param name="r">半径</param>
	/// <returns>当たっていたら:True</returns>
	bool ColCheck_CursorToCircle(Vector2 pos, float r);

	/// <summary>
	/// マウスとの当たり判定(四角)
	/// </summary>
	/// <param name="pos">判定を取りたい座標(左上)</param>
	/// <param name="size">サイズ(右下)</param>
	/// <returns>当たっていたら:True</returns>
	bool ColCheck_CursorToBox(Vector2 pos, Vector2 size);
}