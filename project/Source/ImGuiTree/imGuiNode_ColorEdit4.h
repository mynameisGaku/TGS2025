#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

// ���ʂŕK�v�ȕ�
#include <vector>

/// <summary>
/// ImGui��ColorEdit4�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_ColorEdit4 : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��ColorEdit4�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	/// <param name="rgba">R,G,B,A�̐��l�̃A�h���X</param>
	ImGuiNode_ColorEdit4(std::string label, float* rgba[4]);

	~ImGuiNode_ColorEdit4();

	//================================================================================
	// ���e��֐�

	void Update() override;

private:
	float* rgba[4];	// R,G,B,A�̐��l
};

#endif // IMGUI