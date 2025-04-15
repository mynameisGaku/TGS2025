#pragma once
#include "../../Source/config.h"

#ifdef IMGUI

// ���ʂŕK�v�ȕ�
#include <string>
#include <functional>

// ��ImGui�ɕK�v�ȃt�@�C���̃C���N���[�h
#include "../../vendor/ImGui/imgui.h"
#include "../../vendor/ImGui/imgui_impl_dx11.h"
#include "../../vendor/ImGui/imgui_impl_win32.h"

/// <summary>
/// ImGui�̋@�\�p�̊��N���X
/// </summary>
/// <author> �~�b�` </author>
class ImGuiNode_Base {
public:
	//==================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	ImGuiNode_Base() {};
	virtual ~ImGuiNode_Base() {};

	//==================================================
	// ���e��֐�

	inline virtual void Update() {

		if (func != nullptr)
			func(this);
	}

	//==================================================
	// ���Z�b�^�[

	/// <summary>
	/// ImGuiNode���Ŏ��s�������֐���ݒ肷��(���t���[���Ăяo�����)
	/// </summary>
	/// <param name="_func">ImGuiNode���ŏ����������֐�</param>
	inline void SetFunction(std::function<void(ImGuiNode_Base*)> _func) {

		std::function<void(ImGuiNode_Base*)> fc = nullptr;
		if (_func != nullptr)
			fc = std::bind(_func, this);

		func = fc;
	}

	//==================================================
	// ���Q�b�^�[

	// ���g�̖��O���擾����
	inline std::string Label() const { return label; }

protected:
	std::string label;	// ���g�̖��O
	std::function<void(ImGuiNode_Base*)> func;	// ���s����
};

#endif // IMGUI
