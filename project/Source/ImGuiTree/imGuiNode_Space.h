#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのSpaceの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_Space : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのSpaceを生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	ImGuiNode_Space(std::string label);

	~ImGuiNode_Space();

	//================================================================================
	// ▼各種関数

	void Update() override;

};

#endif // IMGUI