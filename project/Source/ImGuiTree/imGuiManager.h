#pragma once
#include "imGuiNode_Base.h"

// ��ImGui�̋@�\���g�p���邩
#ifdef IMGUI

// ���ėp
#include <map>
#include <unordered_map>

// ���ʂŕK�v�ȕ�
#include "imGuiFunc.h"
#include "imGuiRoot.h"
#include "imGuiNode_Text.h"
#include "imGuiNode_SliderInt.h"
#include "imGuiNode_SliderFloat.h"
#include "imGuiNode_Button.h"
#include "imGuiNode_CheckBox.h"
#include "imGuiNode_DropDown.h"
#include "imGuiNode_Space.h"
#include "imGuiNode_ColorEdit4.h"
#include "imGuiNode_BeginChildren.h"
#include "imGuiNode_EndChildren.h"

/// <summary>
/// ImGui�̏������܂Ƃ߂ĊǗ�����
/// �f�o�b�O�E�B���h�E�ƃC���t�H���[�V�����E�B���h�E������
/// </summary>
/// <author> �~�b�` </author>
namespace ImGuiManager {

	//=============================================================================================
	// ���e��֐�

	/// <summary>
	/// ����������
	/// </summary>
	void Init();
	
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui�X�V����
	/// </summary>
	void ImGuiUpdate();

	/// <summary>
	/// ImGuiInfo�X�V����
	/// </summary>
	void ImGuiInfoUpdate();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
	
	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// ���̂����݂��邩�m�F����B���݂��Ȃ���ΐ�������B
	/// </summary>
	void CheckInstance();

	//=============================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �������s������ݒ肷��
	/// </summary>
	void SetIsActive(bool value);

	//=============================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �������s�������擾����
	/// </summary>
	bool IsActive();

	/// <summary>
	/// ImGuiInfo�̍��������擾����
	/// </summary>
	const std::map<std::string, ImGuiRoot*>* const InfoRoots();

	/// <summary>
	/// ImGuiInfo�̃m�[�h���擾����
	/// </summary>
	const std::map<std::string, ImGuiNode_Base*>* const InfoNodes();

	/// <summary>
	/// ImGui�̍��������擾����
	/// </summary>
	const std::unordered_map<std::string, ImGuiRoot*>* const Roots();

	/// <summary>
	/// ImGui�̃m�[�h���擾����
	/// </summary>
	const std::unordered_map<std::string, ImGuiNode_Base*>* const Nodes();

	//=============================================================================================
	// ��Node�֘A

	/// <summary>
	/// Node��ǉ�����
	/// </summary>
	/// <param name="node">�ǉ�����Node�̎���</param>
	/// <returns>�ǉ�����Node</returns>
	ImGuiNode_Base* AddNode(ImGuiNode_Base* node);

	/// <summary>
	/// Node����������
	/// </summary>
	/// <param name="label">�������閼�O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Node�A�������nullptr</returns>
	template<class C>
	C* FindNode(std::string label) {

		auto nodes = Nodes();

		if (nodes == nullptr)
			return nullptr;

		if ((*nodes).contains(label) == false)
			return nullptr;

		for (const auto& itr : *nodes) {
			if (itr.first == label)
				return dynamic_cast<C*>(itr.second);
		}

		return nullptr;
	}

	//=============================================================================================
	// ��Root�֘A

	/// <summary>
	/// Root��ǉ�����
	/// </summary>
	/// <param name="root">�ǉ�����Root�̎���</param>
	/// <returns>�ǉ�����Root</returns>
	ImGuiRoot* AddRoot(ImGuiRoot* root);
	
	/// <summary>
	/// Root����������
	/// </summary>
	/// <param name="label">�������閼�O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Root�A�������nullptr</returns>
	ImGuiRoot* FindRoot(std::string label);

	//=============================================================================================
	// ��ImGuiInfo�֘A

	/// <summary>
	/// ImGuiInfo��Node��ǉ�����
	/// </summary>
	/// <param name="node">�ǉ�����Node�̎���</param>
	/// <returns>�ǉ�����Node</returns>
	ImGuiNode_Base* AddNodeInfo(ImGuiNode_Base* node);

	/// <summary>
	/// ImGuiInfo��Node����������
	/// </summary>
	/// <param name="label">�������閼�O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Node�A�������nullptr</returns>
	template<class C>
	C* FindNodeInfo(std::string label) {

		auto infoNodes = InfoNodes();

		if (infoNodes == nullptr)
			return nullptr;

		if ((*infoNodes).contains(label) == false)
			return nullptr;

		for (const auto& itr : *infoNodes) {
			if (itr.first == label)
				return dynamic_cast<C*>(itr.second);
		}

		return nullptr;
	}

	/// <summary>
	/// ImGuiInfo��Root��ǉ�����
	/// </summary>
	/// <param name="root">�ǉ�����Root�̎���</param>
	/// <returns>�ǉ�����Root</returns>
	ImGuiRoot* AddRootInfo(ImGuiRoot* root);

	/// <summary>
	/// ImGuiInfo��Root����������
	/// </summary>
	/// <param name="label">�������閼�O</param>
	/// <returns>�����̖��O�ƈ�v���Ă���Root�A�������nullptr</returns>
	ImGuiRoot* FindRootInfo(std::string label);

	//=============================================================================================
	// ��Delete�֘A

	/// <summary>
	/// Node�ARoot��������̖��O�ƈ�v���Ă��镨���폜����
	/// </summary>
	/// <param name="label">�폜������Node�ARoot�̖��O</param>
	void Delete(std::string label);

	/// <summary>
	/// InfoNode�AInfoRoot��������̖��O�ƈ�v���Ă��镨���폜����
	/// </summary>
	/// <param name="label">�폜������Node�ARoot�̖��O</param>
	void DeleteInfo(std::string label);

}

#endif // IMGUI