#pragma once

// ���p����
#include "imGuiNode_Base.h"

#ifdef IMGUI

namespace {

	static unsigned int windowID = 0;
}

/// <summary>
/// ImGui��BeginChild�̋@�\�����N���X
/// </summary>
class ImGuiNode_BeginChildren : public ImGuiNode_Base {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ImGui��BeginChild�𐶐�����
	/// </summary>
	/// <param name="_windowSizeX">�g�g�݂̑傫��(X��)</param>
	/// <param name="_windowSizeY">�g�g�݂̑傫��(Y��)</param>
	ImGuiNode_BeginChildren(float _windowSizeX, float _windowSizeY) :
		id(windowID++),
		windowSizeX(_windowSizeX),
		windowSizeY(_windowSizeY)
	{
	};

	~ImGuiNode_BeginChildren() {};

	//================================================================================
	// ���e��֐�

	inline void Update() override { ImGui::BeginChild(ImGui::GetID((void*)id), ImVec2(windowSizeX, windowSizeY), ImGuiWindowFlags_NoTitleBar); }

private:
	__int64 id;			// ���Ԗڂ�Window��
	float windowSizeX;	// �g�g�݂̑傫��(X��)
	float windowSizeY;	// �g�g�݂̑傫��(Y��)
};

#endif // IMGUI