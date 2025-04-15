#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��SliderFloat�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_SliderFloat : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��SliderFloat�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	/// <param name="value">�ϓ�������l�̃A�h���X</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <param name="format">�����w��q</param>
	ImGuiNode_SliderFloat(std::string label, float* value, float min, float max, std::string format = "%.1f");

	~ImGuiNode_SliderFloat();

	//================================================================================
	// ���e��֐�

	void Update() override;

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �ϓ�������l��ݒ肷��
	/// </summary>
	/// <param name="_value">�ϓ�������l�̃A�h���X</param>
	void SetValue(float* _value) { value = _value; }

private:
	std::string format;	// �����w��q
	float* value;		// �ϓ�������l
	float min;			// �ŏ��l
	float max;			// �ő�l
};

#endif // IMGUI