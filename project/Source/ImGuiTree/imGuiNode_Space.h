#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��Space�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_Space : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��Space�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	ImGuiNode_Space(std::string label);

	~ImGuiNode_Space();

	//================================================================================
	// ���e��֐�

	void Update() override;

};

#endif // IMGUI