#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのSliderIntの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_SliderInt : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのSliderIntを生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	/// <param name="value">変動させる値のアドレス</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="format">書式指定子</param>
	ImGuiNode_SliderInt(std::string label, int* value, int min, int max, std::string format = "%d");

	~ImGuiNode_SliderInt();

	//================================================================================
	// ▼各種関数

	void Update() override;

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 変動させる値を設定する
	/// </summary>
	/// <param name="_value">変動させる値のアドレス</param>
	inline void SetValue(int* _value) { value = _value; }

private:
	std::string format;	// 書式指定子
	int* value;			// 変動させる値
	int min;			// 最小値
	int max;			// 最大値
};

#endif // IMGUI