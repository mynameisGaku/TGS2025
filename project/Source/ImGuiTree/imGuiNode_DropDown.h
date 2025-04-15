#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

// ���ʂŕK�v�ȕ�
#include <vector>

/// <summary>
/// ImGui��DropDown�̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_DropDown : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��DropDown�𐶐�����
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	/// <param name="list">�I����(string�^)</param>
	/// <param name="func">�{�^�����������Ƃ��̏���</param>
	/// <param name="buttonText">�I������{�^���ɕ\�����镶����</param>
	ImGuiNode_DropDown(std::string label, const std::vector<std::string>& list, std::function<void(const std::string&)> func = nullptr, std::string buttonText = "Apply");

	~ImGuiNode_DropDown();

	//================================================================================
	// ���e��֐�

	void Update() override;

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���X�g��ݒ肷��
	/// </summary>
	inline void SetList(const std::vector<std::string>& _list) { list = _list; }

	/// <summary>
	/// �{�^���ɕ\�����镶�����ݒ肷��
	/// </summary>
	inline void SetButtonText(const std::string& _buttonText) { buttonText = _buttonText; }

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �{�^���e�L�X�g���擾����
	/// </summary>
	inline std::string ButtonText() const { return buttonText; }

	/// <summary>
	/// ���ݑI��ł���I�������擾����
	/// </summary>
	inline std::string CurrentItem() const { return currentItem; }

	/// <summary>
	/// ���ݑI��ł���v�f�����擾����
	/// </summary>
	inline int CurrentIndex() const { return currentIndex; }

private:
	std::vector<std::string> list;	// �I����
	std::string buttonText;			// �{�^���e�L�X�g
	std::string currentItem;		// ���ݑI��ł���I����
	int currentIndex;				// ���ݑI��ł���v�f��

	std::function<void(const std::string&)> action;	// �{�^�����������Ƃ��̏���
};

#endif // IMGUI