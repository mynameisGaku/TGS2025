#pragma once
// ◇継承元
#include "src/util/ui/UI_Canvas.h"

// ◇汎用
#include "src/util/Utils.h"

// ◇演出・機能
#include "src/util/font/Font.h"

/// <summary>
/// テキスト表示を行うUIクラス
/// </summary>
/// <author> ミッチ </author>
class UI_Text : public UI_Canvas {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// テキストオブジェクトを生成する
	/// </summary>
	/// <param name="text">表示したい文字列</param>
	/// <param name="trs">描画位置</param>
	/// <param name="fontInfo">フォントの情報</param>
	/// <param name="scrollTotalTime">文字送りの秒数</param>
	/// <param name="scrollSound">文字送りの際に流れる効果音</param>
	UI_Text(const std::string& text, const RectTransform& trs, const FontInfo& fontInfo, const float& scrollTotalTime = -1.0f, const std::string& scrollSE = "");

	~UI_Text();

	//================================================================================
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
	/// 文字列を描画する
	/// </summary>
	void DrawTexts();

	/// <summary>
	/// 文字送り処理
	/// </summary>
	void ScrollTextProcess();

	/// <summary>
	/// サブトーンの設定を自分の状態に合わせたものに適応させる
	/// </summary>
	void ApplySubtone();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 表示する文字列を設定する(その他の設定は変更しない)
	/// </summary>
	void SetText(const std::string& text);

	/// <summary>
	/// 表示する文字列とフォント情報を設定する
	/// </summary>
	/// <param name="text">文字列</param>
	/// <param name="font">フォント情報</param>
	void SetText(const std::string& text, const FontInfo& font);

	// 文字の色を変更する
	inline void SetTextColor(const int& color) { fontInfo.color = color; }

	// スクロールを設定する
	inline void SetScrollTime(const float& value) { scrollTime = scrollTotalTime = value; }

	// スクロール中に再生する効果音を設定する
	inline void SetScrollSE(const std::string& se) { scrollSE = se; }

	/// <summary>
	/// サブトーンを設定する
	/// </summary>
	void SetSubtone(const UI_Subtone& sub) override;

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// 文字列を取得する
	/// </summary>
	inline std::string Text() const { return text; }

	/// <summary>
	/// 文字数を取得する
	/// </summary>
	inline int Len() const { return static_cast<int>(text.length()); }

	/// <summary>
	/// 文字の色を取得する
	/// </summary>
	inline int Color() const { return fontInfo.color; }

	/// <summary>
	/// 文字送りの効果時間の割合
	/// </summary>
	inline float ScrollRate() const { return scrollTime / scrollTotalTime; }

	/// <summary>
	/// 描画する文字列の幅を取得する
	/// </summary>
	inline Vector2 TextSize() const { return textSize; }

	/// <summary>
	/// 文字送りしているかを取得する
	/// </summary>
	inline bool IsScroll() const { return scrollTime > 0.0f; }

	// アンカーポイントによるズレ値を取得する
	Vector2 DisplacementByAnchorPoint() override;

private:
	//================================================================================
	// ▼文字列関連

	std::string text;	// 文字列
	Vector2 textSize;	// 文字列幅
	int length;			// 文字数

	FontInfo fontInfo;	// フォントの情報

	//================================================================================
	// ▼文字送り関連

	std::vector<String::Letter> letters;	// 文字列を一文字ずつ保存する
	std::string scrollSE;	// 文字送りする際のサウンド

	int afterScrollText;	// 1フレーム前のスクロール済の文字数
	float scrollTime;		// 文字列をスクロールする時間
	float scrollTotalTime;	// 文字列をスクロールする全体時間

};
