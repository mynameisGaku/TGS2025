#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのSliderFloatの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_SliderFloat : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのSliderFloatを生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	/// <param name="value">変動させる値のアドレス</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="format">書式指定子</param>
	ImGuiNode_SliderFloat(std::string label, float* value, float min, float max, std::string format = "%.1f");

	~ImGuiNode_SliderFloat();

	//================================================================================
	// ▼各種関数

	void Update() override;

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 変動させる値を設定する
	/// </summary>
	/// <param name="_value">変動させる値のアドレス</param>
	void SetValue(float* _value) { value = _value; }

private:
	std::string format;	// 書式指定子
	float* value;		// 変動させる値
	float min;			// 最小値
	float max;			// 最大値
};

#endif // IMGUI