#pragma once
#include "src/util/ui/UI_Canvas.h"

// ◇個別で必要な物
#include "src/util/font/Font.h"
#include "src/util/input/KeyDefine.h"

class UI_Text;

/// <summary>
/// スライダーの処理を行うクラス
/// </summary>
/// <author> ミッチ </author>
class UI_Slider : public UI_Canvas {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// スライダーの生成する
	/// </summary>
	/// <param name="trs">描画位置</param>
	/// <param name="value">変動させる値のアドレス</param>
	/// <param name="minValue">最小値</param>
	/// <param name="maxValue">最大値</param>
	/// <param name="backSize">スライダーの大きさ</param>
	/// <param name="color">スライダーに関する色情報</param>
	/// <param name="format">書式指定子</param>
	/// <param name="label">自身の名前</param>
	UI_Slider(const std::string& label, const RectTransform& trs, float* value, const float& minValue, const float& maxValue, const Vector2& size, const UI_Define::SliderColor& color, const std::string& format = "%.1f", const FontInfo& fontInfo = FontInfo());
	
	~UI_Slider();

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
	/// 背景の描画処理
	/// </summary>
	void DrawBack();

	/// <summary>
	/// 分割部分の描画
	/// </summary>
	void DrawDivisions();

	/// <summary>
	/// つまみの描画
	/// </summary>
	void DrawPoint();

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 書式指定子を設定する
	/// </summary>
	/// <param name="format">書式指定子</param>
	/// <param name="font">フォント情報</param>
	void SetFormatStr(const std::string& format, const FontInfo& font);
	
	/// <summary>
	/// スライダーの表示名を設定する
	/// </summary>
	/// <param name="label">スライダーの表示名</param>
	/// <param name="font">フォント情報</param>
	void SetLabelText(const std::string& label, const FontInfo& font);

	inline void SetValue(float* setValue) { value = setValue; }
	inline void SetMaxValue(const float& setMaxValue) { maxValue = max(setMaxValue, minValue + 1); }
	inline void SetMinValue(const float& setMinValue) { minValue = min(setMinValue, maxValue + 1); }
	
	inline void SetPointEnabled(const bool& enabled) { pointEnabled = enabled; }
	inline void SetPointSize(const Vector2& size) { pointSize = size; }
	inline void SetBackSize(const Vector2& size) { backSize = size; }

	inline void SetBackColor(const int& color) { colors.backColor = color; }
	inline void SetGaugeColor(const int& color) { colors.gaugeColor = color; }
	inline void SetPointColor(const int& color) { colors.pointColor = color; }

	//==========================================================================================
	// ▼ゲッター

	inline Vector2 ScreenPos() const { return rectTransform->Global().position - backSize; }

	inline const float Value() const { return *value; }
	inline float MaxValue() const { return maxValue; }
	inline float MinValue() const { return minValue; }

	inline bool PointEnabled() const { return pointEnabled; }
	inline Vector2 PointSize() const { return pointSize; }
	inline Vector2 BackSize() const { return backSize; }

	inline UI_Define::SliderColor Colors() const { return colors; }

private:
	//==========================================================================================
	// ▼メンバ変数

	std::string label;	// 表示する名前

	float* value;	// 現在の値
	float maxValue;	// 最大値
	float minValue;	// 最小値

	UI_Define::SliderColor colors;	// 色情報

	int divisions;	// 分割数

	bool pointEnabled;	// つまみの操作が有効か
	bool isPointMove;	// つまみが動いているか
	bool front;

	Vector2 pointSize;	// つまみの大きさ
	Vector2 backSize;	// スライダーの大きさ

	Vector2 pointPosition;	// つまみの座標

	std::string format;		// 書式指定子
	UI_Text* formatText;	// 数値の表示テキスト
	UI_Text* labelText;		// 自身の名前
	FontInfo fontInfo;		// フォント情報

	KeyDefine::TouchPhase touchPhase;

	/// <summary>
	/// つまみをクリックした処理
	/// </summary>
	void TouchBeginUpdate();

	/// <summary>
	/// つまみを動かす処理
	/// </summary>
	void TouchMovedUpdate();

	/// <summary>
	/// つまみから離した処理
	/// </summary>
	void TouchEndedUpdate();
};
