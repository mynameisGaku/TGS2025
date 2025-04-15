#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのButtonの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_Button : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのButtonを生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	/// <param name="func">ボタンを押したときの処理</param>
	ImGuiNode_Button(std::string label, std::function<void()> func);

	~ImGuiNode_Button();

	//================================================================================
	// ▼各種関数

	void Update() override;

private:
	std::function<void()> action;	// ボタンを押したときの処理
};

#endif // IMGUI