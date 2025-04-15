#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��CheckBox�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_CheckBox : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��CheckBox�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	/// <param name="value">�ϓ�������l�̃A�h���X</param>
	ImGuiNode_CheckBox(std::string label, bool* value);

	~ImGuiNode_CheckBox();

	//================================================================================
	// ���e��֐�

	void Update() override;

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �ϓ�������l��ݒ肷��
	/// </summary>
	/// <param name="value">�ϓ�������l�̃A�h���X</param>
	inline void SetValue(bool* value) { isCheck = value; }

private:
	bool* isCheck;	// ��
};

#endif // IMGUI