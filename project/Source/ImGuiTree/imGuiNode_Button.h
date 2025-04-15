#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��Button�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_Button : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��Button�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	/// <param name="func">�{�^�����������Ƃ��̏���</param>
	ImGuiNode_Button(std::string label, std::function<void()> func);

	~ImGuiNode_Button();

	//================================================================================
	// ���e��֐�

	void Update() override;

private:
	std::function<void()> action;	// �{�^�����������Ƃ��̏���
};

#endif // IMGUI