#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��SliderInt�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_SliderInt : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��SliderInt�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	/// <param name="value">�ϓ�������l�̃A�h���X</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <param name="format">�����w��q</param>
	ImGuiNode_SliderInt(std::string label, int* value, int min, int max, std::string format = "%d");

	~ImGuiNode_SliderInt();

	//================================================================================
	// ���e��֐�

	void Update() override;

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �ϓ�������l��ݒ肷��
	/// </summary>
	/// <param name="_value">�ϓ�������l�̃A�h���X</param>
	inline void SetValue(int* _value) { value = _value; }

private:
	std::string format;	// �����w��q
	int* value;			// �ϓ�������l
	int min;			// �ŏ��l
	int max;			// �ő�l
};

#endif // IMGUI