#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGui��Text�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_Text : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��Text�𐶐�����
	/// </summary>
	/// <param name="lable">���g�̖��O</param>
	/// <param name="format">�\�����镶����</param>
	/// <param name="func">ImGuiNode_Base�������Ɏ��֐�(�������ύX�������ꍇ�ɁA�֐���ʂō���Ă���A���̈����ɓn���Ă�����)</param>
	ImGuiNode_Text(std::string lable, std::string format, std::function<void(ImGuiNode_Base*)> func = nullptr);

	~ImGuiNode_Text();

	//================================================================================
	// ���e��֐�

	void Update() override;

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �\�����镶�����ݒ肷��
	/// </summary>
	/// <param name="_format">�\�����镶����</param>
	inline void SetText(std::string _format) { format = _format; }

private:
	std::string format;	// �\�����镶����
};

#endif // IMGUI
