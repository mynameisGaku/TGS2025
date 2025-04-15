#pragma once
// ◇継承元
#include "UI_Canvas.h"

#include "Font.h"
#include "gauge.h"

class UI_Text;

/// <summary>
/// ゲージの処理を行うクラス
/// </summary>
class UI_Gauge : public UI_Canvas {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ゲージの生成をする
	/// </summary>
	/// <param name="trs">描画位置</param>
	/// <param name="value">現在値のアドレス</param>
	/// <param name="minValue">最小値</param>
	/// <param name="maxValue">最大値</param>
	/// <param name="gaugeSize">ゲージの大きさ</param>
	/// <param name="gaugeColor">ゲージの色</param>
	/// <param name="delayColor">減少時の色</param>
	/// <param name="increaseColor">増加時の色</param>
	/// <param name="backColor">背景色</param>
	/// <param name="gaugeThickness">ゲージの縁の太さ</param>
	/// <param name="gaugeLateSpeed">ゲージが遅れてくる速度</param>
	UI_Gauge(const RectTransform& trs, float* value, const float& minValue, const float& maxValue, const Vector2& gaugeSize, const UI_Define::SliderColor& color, const float& gaugeThickness = 1.0f, const float& gaugeLateSpeed = 0.04f, const int& innerEdgeColor = GetColor(0, 0, 0), const int& outerEdgeColor = GetColor(200, 200, 200));
	
	/// <summary>
	/// ゲージの生成をする
	/// </summary>
	/// <param name="trs">描画位置</param>
	/// <param name="value">現在値のアドレス</param>
	/// <param name="minValue">最小値</param>
	/// <param name="maxValue">最大値</param>
	/// <param name="gaugeGraph">ゲージの画像</param>
	/// <param name="delayGraph">ゲージの減少時の画像</param>
	/// <param name="increaseGraph">ゲージの増加時の画像</param>
	/// <param name="backGraph">ゲージの背景画像</param>
	/// <param name="gaugeLateSpeed">ゲージが遅れてくる速度</param>
	UI_Gauge(const RectTransform& trs, float* value, const float& minValue, const float& maxValue, const int& gaugeGraph, const int& delayGraph, const int& increaseGraph, const int& backGraph, const float& gaugeLateSpeed = 0.04f);

	~UI_Gauge();

	//==========================================================================================
	// ▼各種関数

	void Update() override;
	void Draw() override;

	//==========================================================================================
	// ▼セッター

	inline void SetValue(float* setValue) { value = setValue; }
	inline void SetMaxValue(const float& setMaxValue) { maxValue = max(setMaxValue, minValue + 1); }
	inline void SetMinValue(const float& setMinValue) { minValue = min(setMinValue, maxValue + 1); }

	inline void SetBackColor(const int& color) { colors.backColor = color; }

	//==========================================================================================
	// ▼ゲッター

	inline const float Value() const { return *value; }
	inline float MaxValue() const { return maxValue; }
	inline float MinValue() const { return minValue; }

	inline UI_Define::SliderColor Colors() const { return colors; }

private:
	//==========================================================================================
	// ▼メンバ変数

	Gauge gauge;

	float* value;	// 現在の値
	float maxValue;	// 最大値
	float minValue;	// 最小値

	int hGauge;			// ゲージの画像
	int hDelayColor;	// 減少時の画像
	int hIncreaseColor;	// 増加時の画像
	int hBackColor;		// 背景画像

	UI_Define::SliderColor colors;	// 色情報
	int innerEdgeColor;	// 内側の角の色
	int outerEdgeColor;	// 外側の角の色

	Vector2 gaugeSize;	// ゲージの大きさ

	float gaugeThickness;	// ゲージの縁の太さ
	float gaugeLateSpeed;	// ゲージが遅れてくる速度
};