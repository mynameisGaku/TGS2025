#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

namespace {

	static unsigned int windowID = 0;
}

/// <summary>
/// ImGuiのBeginChildの機能を持つクラス
/// </summary>
class ImGuiNode_BeginChildren : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのBeginChildを生成する
	/// </summary>
	/// <param name="_windowSizeX">枠組みの大きさ(X軸)</param>
	/// <param name="_windowSizeY">枠組みの大きさ(Y軸)</param>
	ImGuiNode_BeginChildren(float _windowSizeX, float _windowSizeY) :
		id(windowID++),
		windowSizeX(_windowSizeX),
		windowSizeY(_windowSizeY)
	{
	};

	~ImGuiNode_BeginChildren() {};

	//================================================================================
	// ▼各種関数

	inline void Update() override { ImGui::BeginChild(ImGui::GetID((void*)id), ImVec2(windowSizeX, windowSizeY), ImGuiWindowFlags_NoTitleBar); }

private:
	__int64 id;			// 何番目のWindowか
	float windowSizeX;	// 枠組みの大きさ(X軸)
	float windowSizeY;	// 枠組みの大きさ(Y軸)
};

#endif // IMGUI