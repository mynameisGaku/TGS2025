#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��EndChild�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_EndChildren : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	ImGuiNode_EndChildren() {};
	~ImGuiNode_EndChildren() {};

	//================================================================================
	// ���e��֐�

	inline void Update() override { ImGui::EndChild(); }

};

#endif // IMGUI