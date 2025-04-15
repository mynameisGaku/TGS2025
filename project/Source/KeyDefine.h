#pragma once

// ◇汎用
#include <Dxlib.h>
#include <unordered_map>

namespace KeyDefine {

	/// <summary>
	/// 入力の種類 
	/// </summary>
	enum class KeyCode {
		None = -1,
		Up,				// デジタル方向ボタン上
		Down,			// デジタル方向ボタン下
		Left,			// デジタル方向ボタン左
		Right,			// デジタル方向ボタン右
		Start,			// STARTボタン
		Back,			// BACKボタン
		LeftThumb,		// 左スティック押し込み
		RightThumb,		// 右スティック押し込み
		LeftShoulder,	// LBボタン
		RightShoulder,	// RBボタン
		ButtonA,		// Aボタン
		ButtonB,		// Bボタン
		ButtonX,		// Xボタン
		ButtonY,		// Yボタン

		A,				// Aキー
		B,				// Bキー
		C,				// Cキー
		D,				// Dキー
		E,				// Eキー
		F,				// Fキー
		G,				// Gキー
		H,				// Hキー
		I,				// Iキー
		J,				// Jキー
		K,				// Kキー
		L,				// Lキー
		M,				// Mキー
		N,				// Nキー
		O,				// Oキー
		P,				// Pキー
		Q,				// Qキー
		R,				// Rキー
		S,				// Sキー
		T,				// Tキー
		U,				// Uキー
		V,				// Vキー
		W,				// Wキー
		X,				// Xキー
		Y,				// Yキー
		Z,				// Zキー

		Alpha0,			// 0キー
		Alpha1,			// 1キー
		Alpha2,			// 2キー
		Alpha3,			// 3キー
		Alpha4,			// 4キー
		Alpha5,			// 5キー
		Alpha6,			// 6キー
		Alpha7,			// 7キー
		Alpha8,			// 8キー
		Alpha9,			// 9キー

		F1,				// F1キー
		F2,				// F2キー
		F3,				// F3キー
		F4,				// F4キー
		F5,				// F5キー
		F6,				// F6キー
		F7,				// F7キー
		F8,				// F8キー
		F9,				// F9キー
		F10,			// F10キー
		F11,			// F11キー
		F12,			// F12キー

		BackSpace,		// BackSpaceキー
		Tab,			// Tabキー
		Enter,			// Enterキー

		LeftShift,		// 左Shiftキー
		RightShift,		// 右Shiftキー
		LeftControl,	// 左Ctrlキー
		RightControl,	// 右Ctrlキー
		Escape,			// Escキー
		Space,			// スペースキー
		PageUp,			// PageUpキー
		PageDown,		// PageDownキー
		End,			// Endキー
		Home,			// Homeキー
		UpArrow,		// 上キー
		DownArrow,		// 下キー
		LeftArrow,		// 左キー
		RightArrow,		// 右キー
		Insert,			// Insertキー
		Delete,			// Deleteキー

		LeftClick,		// 左クリック
		RightClick,		// 右クリック
		MiddleClick,	// ホイールクリック

		MouseButton1,	// マウス１ボタン
		MouseButton2,	// マウス２ボタン
		MouseButton3,	// マウス３ボタン
		MouseButton4,	// マウス４ボタン
		MouseButton5,	// マウス５ボタン
	};

	/// <summary>
	/// 入力状況
	/// </summary>
	enum class TouchPhase {
		None = -1,
		Begin,		// 押した瞬間
		Moved,		// 移動中
		Stationary,	// 押しているが移動していない
		Ended,		// 離された瞬間
		Canceled,	// キャンセル
	};

	/// <summary>
	/// 入力するデバイスの種類
	/// </summary>
	enum class DeviceType {
		None = -1,
		Key,		// キーボード
		Pad,		// パッド
		Mouse,		// マウス
	};

	/// <summary>
	/// 入力情報
	/// </summary>
	struct KeyInfo {
		int* value;				// 入力の値
		KeyCode keyCode;		// 入力の種類
		DeviceType deviceType;	// 入力するデバイスの種類
	};

	//============================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	void SetKeyInputValue();
	void SetPadInputValue();
	void SetMouseInputValue();

	/// <summary>
	/// 入力種類から入力デバイスを取得する
	/// </summary>
	/// <param name="key">入力種類</param>
	/// <returns>入力デバイス</returns>
	DeviceType KeyCodeToDeviceType(KeyCode key);

	/// <summary>
	/// 入力デバイスから入力種類を取得する
	/// </summary>
	/// <param name="type">入力デバイス</param>
	/// <returns>入力種類</returns>
	std::list<KeyInfo> DeviceTypeToKeyInfo(DeviceType type);

	/// <summary>
	/// 全ての入力情報を取得する
	/// </summary>
	std::unordered_map<KeyCode, KeyInfo> GetKeyAll();
};