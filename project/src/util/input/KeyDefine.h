#pragma once

// ◇汎用
#include <Dxlib.h>
#include <unordered_map>

#include "src/util/math/vector2.h"

namespace KeyDefine {

	static constexpr int KEY_BUFFER = 256;					// キーボードのバッファ
	static constexpr float HOLD_TIME = 0.25f;				// ボタンの長押しを判定する時間
	static constexpr float ADVANCED_ENTRY_TIME = 0.2f;		// 先行入力の保持時間

	static constexpr int PAD_NUMBER_MAX = 2;				// パッドの最大接続数
	static constexpr float STICK_DEADZONE = 0.1f;			// スティックのデッドゾーン定義
	static constexpr float TRIGGER_DEADZONE = 0.2f;			// トリガーのデッドゾーン定義

	static const Vector2 STICK_SENSI_MAX = Vector2(10.0f);	// 最大スティック感度
	static const Vector2 MOUSE_SENSI_MAX = Vector2(10.0f);	// 最大マウス感度

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
	enum TouchPhase {
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
	/// マウスの移動方法
	/// </summary>
	enum class MouseMovement {
		Free = 0,	// 自由に操作可能
		Fixed,		// 画面中央で固定
		OnScreen,	// 画面内に留める
	};

	/// <summary>
	/// 入力情報
	/// </summary>
	class InputData {
	public:
		int* value;				// 入力値
		float pushTime;			// 押されている秒数
		KeyCode keyCode;		// 入力の種類
		DeviceType deviceType;	// 入力するデバイスの種類
		std::unordered_map<TouchPhase, bool> isInput[PAD_NUMBER_MAX + 1];		// 押されているか(入力状態、押下の有無)
		std::unordered_map<TouchPhase, bool> isAccepted[PAD_NUMBER_MAX + 1];	// 既に入力を受け付けたか(入力状態、押下の有無)

		InputData() :
			value(nullptr),
			pushTime(0.0f),
			keyCode(KeyCode::None),
			deviceType(DeviceType::None)
		{
		};

		/// <summary>
		/// 入力情報の初期化
		/// </summary>
		/// <param name="_value">入力値</param>
		/// <param name="_keyCode">入力の種類</param>
		/// <param name="_deviceType">入力するデバイスの種類</param>
		InputData(int* _value, KeyCode _keyCode, DeviceType _deviceType) :
			value(_value),
			pushTime(0.0f),
			keyCode(_keyCode),
			deviceType(_deviceType)
		{
		};

		/// <summary>
		/// 入力情報の初期化
		/// </summary>
		/// <param name="_value">入力値</param>
		/// <param name="_pushTime">押されている秒数</param>
		/// <param name="_isPush">押されているか</param>
		/// <param name="_keyCode">入力の種類</param>
		/// <param name="_deviceType">入力するデバイスの種類</param>
		InputData(int* _value, float _pushTime, KeyCode _keyCode, DeviceType _deviceType) :
			value(_value),
			pushTime(_pushTime),
			keyCode(_keyCode),
			deviceType(_deviceType)
		{
		};

		~InputData() {};

		inline void Update() {

			const int maxTouchPhase = TouchPhase::Canceled;

			for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++) {
				for (int j = 0; j < maxTouchPhase; j++) {
					const int padNumber = i;
					const TouchPhase touchPhase = static_cast<TouchPhase>(j);
					isInput[padNumber][touchPhase] = false;
					isAccepted[padNumber][touchPhase] = false;
				}
			}
		}
	};

	/// <summary>
	/// 先行入力情報
	/// </summary>
	class AdvancedEntryInfo {
	public:
		InputData inputData;// 入力情報
		float saveTime;		// 保持する秒数

		AdvancedEntryInfo() :
			inputData(InputData()),
			saveTime(0.0f)
		{
		};

		/// <summary>
		/// 先行入力情報の初期化
		/// </summary>
		/// <param name="data">入力情報</param>
		/// <param name="time">保持する秒数</param>
		AdvancedEntryInfo(const InputData& data, float time) :
			inputData(data),
			saveTime(time)
		{
		};

		~AdvancedEntryInfo() {};
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
	const std::list<InputData> DeviceTypeToInputData(DeviceType type);

	/// <summary>
	/// 全ての入力情報を取得する
	/// </summary>
	const std::unordered_map<KeyCode, InputData> GetKeyAll();

	/// <summary>
	/// 種類ごとの入力情報を取得する
	/// </summary>
	const InputData GetInputData(KeyCode key);
};