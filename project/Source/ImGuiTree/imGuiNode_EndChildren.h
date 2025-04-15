#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのEndChildの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_EndChildren : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	ImGuiNode_EndChildren() {};
	~ImGuiNode_EndChildren() {};

	//================================================================================
	// ▼各種関数

	inline void Update() override { ImGui::EndChild(); }

};

#endif // IMGUI