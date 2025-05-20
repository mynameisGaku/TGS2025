#pragma once
// ◇継承元
#include "UI_Canvas.h"

// ◇演出・機能
#include "KeyDefine.h"
#include <functional>

/// <summary>
/// ボタン表示とボタン押下処理を行うクラス
/// </summary>
/// <author> ミッチ </author>
class UI_Button : public UI_Canvas {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// 仮想ボタン、押下処理で初期化する
	/// </summary>
	/// <param name="image">ボタンに使用する画像ハンドル</param>
	/// <param name="trs">座標・回転・拡縮の情報</param>
	/// <param name="input">仮想ボタン</param>
	/// <param name="timing">ボタン押下処理を呼ぶタイミング</param>
	/// <param name="func">ボタン押下処理</param>
	/// <param name="padNumber">パッドの番号</param>
	UI_Button(const int& image, const RectTransform& trs, const std::string& inputName, const std::vector<KeyDefine::TouchPhase>& timing, const std::function<void()>& func, const int& padNumber = DX_INPUT_PAD1);
	
	/// <summary>
	/// 入力の種類、押下処理で初期化する
	/// </summary>
	/// <param name="imagePath">ボタンに使用する画像のパス</param>
	/// <param name="trs">描画座標</param>
	/// <param name="key">入力の種類</param>
	/// <param name="timing">ボタン押下処理を呼ぶタイミング</param>
	/// <param name="func">ボタン押下処理</param>
	/// <param name="padNumber">パッドの番号</param>
	UI_Button(const int& image, const RectTransform& trs, const std::vector<KeyDefine::KeyCode>& keyCode, const std::vector<KeyDefine::TouchPhase>& timing, const std::function<void()>& func, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// UI_Buttonを生成する
	/// </summary>
	/// <param name="imagePath">ボタンに使用する画像のパス</param>
	/// <param name="trs">描画座標</param>
	UI_Button(const int& image, const RectTransform& trs);
	
	~UI_Button();

	//==========================================================================================
	// ▼各種関数

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// 押下判定と押下処理
	/// </summary>
	/// <param name="phase">押下タイミング</param>
	/// <returns>処理を実行した場合、trueを返す</returns>
	bool PushProcess(const KeyDefine::TouchPhase& phase);

	/// <summary>
	/// マウスによる押下判定と押下処理
	/// </summary>
	/// <param name="phase">押下タイミング</param>
	/// <returns>処理を実行した場合、trueを返す</returns>
	bool PushProcessMouse(const KeyDefine::TouchPhase& phase);

	/// <summary>
	/// 押された際に画像を暗く描画する
	/// </summary>
	void DrawDarken();

	/// <summary>
	/// 入力種類をキーボード・パッドとマウスに分類する
	/// </summary>
	void SeparateInputTypes();

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// マウスのクリックに反応するかを設定する
	/// </summary>
	void SetIsMouseClick(const bool& value);

	/// <summary>
	/// ボタンを押した際に画像を暗く描画するか設定する
	/// </summary>
	inline void SetIsPushedDarkness(const bool& value) { isPushedDarkenes = value; }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// ボタンとマウスとの当たり判定
	/// </summary>
	bool ColCheck_Mouse();

private:
	/// <summary>
	/// UI_Buttonを生成する
	/// </summary>
	/// <param name="image">ボタンに使用する画像ハンドル</param>
	/// <param name="trs">描画座標</param>
	/// <param name="input">仮想ボタン</param>
	/// <param name="key">入力の種類</param>
	/// <param name="timing">ボタン押下処理を呼ぶタイミング</param>
	/// <param name="func">ボタン押下処理</param>
	/// <param name="padNumber">パッドの番号</param>
	UI_Button(const int& image, const RectTransform& trs, const std::string& inputName, const std::vector<KeyDefine::KeyCode>& keyCode, const std::vector<KeyDefine::TouchPhase>& timing, const std::function<void()>& func, const int& padNumber = DX_INPUT_PAD1);

	int padNumber;			// パッドの番号
	bool isPushed;			// 押されているか
	bool isPushedDarkenes;	// 押された際に画像を暗くするか
	bool isMouseClick;		// マウスのクリックに反応するか

	std::function<void()> action;				// ボタン押下処理
	std::vector<KeyDefine::TouchPhase> timing;	// 押下処理を呼ぶタイミング

	std::string inputName;		// 仮想ボタンの名前
	std::vector<KeyDefine::KeyCode> allKeyCode;		// 全ての入力種類
	std::vector<KeyDefine::KeyCode> keyCodes;		// キーボード・パッドの入力種類
	std::vector<KeyDefine::KeyCode> mouseKeyCodes;	// マウスの入力種類
};
