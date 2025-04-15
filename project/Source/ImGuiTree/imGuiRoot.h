#pragma once

// ���p����
#include "imGuiNode_Base.h"

// ��ImGui�̋@�\���g�p���邩
#ifdef IMGUI

// ���ėp
#include <map>
#include <vector>

// ���ʂŕK�v�ȕ�
#include "imGuiNode_BeginChildren.h"
#include "imGuiNode_EndChildren.h"

/// <summary>
/// ImGuiNode���q����ۂ̍����̋@�\�����N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiRoot : public ImGuiNode_Base {
public:
	//==================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="label">���g�̖��O</param>
	ImGuiRoot(std::string label);

	~ImGuiRoot();

	//==================================================
	// ���e��֐�

	void Update() override;

	/// <summary>
	/// ImGuiNode��ǉ�����
	/// </summary>
	/// <param name="node">ImGuiNode�̎���</param>
	/// <returns>�ǉ�����InGuiNode�̎���</returns>
	ImGuiNode_Base* Add(ImGuiNode_Base* node);

	/// <summary>
	/// ImGuiRoot��ǉ�����
	/// </summary>
	/// <param name="root">ImGuiRoot�̎���</param>
	/// <returns>�ǉ�����InGuiRoot�̎���</returns>
	ImGuiRoot* AddChild(ImGuiRoot* root);

	/// <summary>
	/// �����̖��O�ƈ�v���Ă���Node�AChild���폜����
	/// </summary>
	/// <param name="nodeLabel">�폜����Node�AChild�̖��O</param>
	void Delete(std::string nodeLabel);

	/// <summary>
	/// �����ƈ�v���Ă���Node�AChild���폜����
	/// </summary>
	/// <param name="nodeLabel">�폜����Node�AChild�̃|�C���^�[</param>
	void Delete(ImGuiNode_Base* node);
	
	/// Node��Child(�܂Ƃ߂�)�@�\��ǉ�����
	/// EndChild���Ă΂��܂łɁA�������ꂽNode���܂Ƃ߂�
	/// ���̂��߁AChild�𐶐������ꍇ�͕K��EndChild���������Ȃ���΂����Ȃ�
	/// �p�����[�^�[:
	///  windowSizeX �܂Ƃ߂��ۂ�Window�̑傫��(X��)
	///  windowSizeY �܂Ƃ߂��ۂ�Window�̑傫��(Y��)
	void NodeBeginChild(float windowSizeX, float windowSizeY);

	/// <summary>
	/// Child(�܂Ƃ߂�)�@�\���~����@�\��ǉ�����
	/// </summary>
	void NodeEndChild();

	/// <summary>
	/// �������Ă���Node(Child���܂߂�)�S�Ă�Child(�܂Ƃ߂�)�@�\��ǉ�����
	/// </summary>
	/// <param name="windowSizeX">�܂Ƃ߂��ۂ�Window�̑傫��(X��)</param>
	/// <param name="windowSizeY">�܂Ƃ߂��ۂ�Window�̑傫��(Y��)</param>
	void SetChild(float windowSizeX, float windowSizeY);

	/// <summary>
	/// �����̖��O�ƈ�v���Ă���Node���擾����
	/// </summary>
	/// <param name="nodeLabel">���O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Node�B��v���������Ȃ��ꍇnullptr</returns>
	template<class C>
	C* Node(std::string nodeLabel) {
		for (auto itr : node) {
			if (itr->Label() == nodeLabel)
				return dynamic_cast<C*>(itr);
		}
		return nullptr;
	}

	/// <summary>
	/// �����̖��O�ƈ�v���Ă���Root���擾����
	/// </summary>
	/// <param name="nodeLabel">���O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Root�B��v���������Ȃ��ꍇnullptr</returns>
	ImGuiRoot* Children(std::string nodeLabel);

	/// <summary>
	/// �����̖��O�ƈ�v���Ă���Root��e�q�K�w���������Ď擾����
	/// </summary>
	/// <param name="nodeLabel">���O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Root�B��v���������Ȃ��ꍇnullpt</returns>
	ImGuiRoot* SearchChildren(std::string nodeLabel);

	//==================================================
	// ���Q�b�^�[

	// �J���Ă����Ԃ����擾����
	inline bool IsOpen() const { return isOpen; }

private:
	std::vector<ImGuiNode_Base*> node;	// �q�����Ă���m�[�h
	std::vector<ImGuiRoot*> children;	// �e�q�K�w

	ImGuiNode_BeginChildren* beginChildren;	// �܂Ƃ߂�@�\
	ImGuiNode_EndChildren* endChildren;		// �܂Ƃ߂�@�\���~����@�\

	bool isOpen;	// �J���Ă����Ԃ�
};

#endif // IMGUI