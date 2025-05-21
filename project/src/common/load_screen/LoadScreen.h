#pragma once
#include "framework/gameObject.h"

// ◇汎用
#include "src/util/Utils.h"
#include "src/util/easing/Easing.h"
#include "src/util/font/Font.h"
#include "src/util/math/Vector2.h"

namespace {

	static const Vector2 DRAW_TEXT_POS = Vector2(4.0f, Screen::HEIGHT_F - 64.0f);	// 文字の表示位置
	static const int TEXT_COLOR = GetColor(255, 255, 255);

	static const Vector2 GAUGE_POS = Vector2(4.0f, Screen::HEIGHT_F - 24.0f);		// ゲージの表示位置
	static const Vector2 GAUGE_SIZE = Vector2(Screen::WIDTH_HALF - 4.0f, 20.0f);	// ゲージの表示サイズ
	static const int GAUGE_COLOR = GetColor(220, 0, 220);	// ゲージの色
}

/// <summary>
/// ローディング画面の表示を行う
/// </summary>
class LoadScreen : public GameObject {
public:
	//================================================================================
	// ▼定義値

	/// <summary>
	/// 背景画像のスクロール方法
	/// </summary>
	enum class ScrollType {
		stNone =-1,	// 無
		stUp,		// 上スクロール
		stDown,		// 下スクロール
		stLeft,		// 左スクロール
		stRight,	// 右スクロール
		stMax
	};

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	LoadScreen();
	~LoadScreen();

	//================================================================================
	// ▼各種関数

	void Update() override;
	void Draw() override;

	/// <summary>
	/// 背景画像の描画
	/// </summary>
	void DrawBackGround();

	/// <summary>
	/// ロード中に表示する文字の描画
	/// </summary>
	void DrawLoadText();

	/// <summary>
	/// ロードゲージの描画
	/// </summary>
	void DrawLoadGauge();

	/// <summary>
	/// ボタンヒントの描画
	/// </summary>
	void DrawButtonTip();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// ロードの割合を設定する
	/// </summary>
	/// <param name="_rate">0.0f～1.0fの間</param>
	void SetRate(const float& rate);

	/// <summary>
	/// 背景画像を設定する
	/// </summary>
	/// <param name="filename">背景画像のファイルパス</param>
	/// <param name="scroll">背景画像をスクロールするか</param>
	void SetBackGround(const std::string& filename, const ScrollType& scroll = ScrollType::stNone);

	/// <summary>
	/// ボタン押下でフェードアウトへ移行するかを設定する
	/// </summary>
	inline void SetIsPushFadeOut(const bool& value) { isPushFadeOut = value; }

	/// <summary>
	/// フェードインする
	/// </summary>
	/// <param name="sec">効果時間(秒)</param>
	/// <param name="easingType">イージングの種類</param>
	inline void FadeIn(const float& sec, const EasingType& easingType = EasingType::Linear) { fadeEasing.SetEasing(0.0f, 255.0f, sec, easingType, true); }

	/// <summary>
	/// フェードアウトする
	/// </summary>
	/// <param name="sec">効果時間(秒)</param>
	/// <param name="easingType">イージングの種類</param>
	inline void FadeOut(const float& sec, const EasingType& easingType = EasingType::Linear) { fadeEasing.SetEasing(255.0f, 0.0f, sec, easingType, true); }

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// ロードが終了したか
	/// </summary>
	/// <returns>ロードが終了したならtrue</returns>
	bool IsLoadEnd() const;

	/// <summary>
	/// フェード中か
	/// </summary>
	/// <returns>フェード中ならtrue</returns>
	inline bool IsFade() const { return fadeEasing.info.isActive; }

private:
	EasingUtils::EasingFloat fadeEasing;	// フェード用イージング情報
	float rate;				// ロード割合
	float textWidth;		// 表示文字の横幅
	std::string loadText;	// ロード中に表示する文字

	int hBackground;	// 背景画像
	bool isPushFadeOut;	// ボタン押下でフェードアウトへ移行する

	Vector2 bgPos;	// 背景画像の描画座標
	ScrollType scrollType;	// 背景画像のスクロール方法

	FontInfo useFont;	// 使用するフォント情報

	const std::string LOADING_TEXT = "Now Loading... %3.0f";// ロード中に表示する文字
	const std::string LOAD_END_TEXT = "Load Complete!";		// ロード終了時に表示する文字
	const std::string BUTTON_TIP = "ボタンを押して次へ進む";// ボタンヒント
};