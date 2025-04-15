#include "imGuiNode_Space.h"

#ifdef IMGUI

ImGuiNode_Space::ImGuiNode_Space(std::string _label) {

	label = _label;
}

ImGuiNode_Space::~ImGuiNode_Space()
{
}

void ImGuiNode_Space::Update() {

	ImGui::Separator();

	ImGuiNode_Base::Update();
}

#endif // IMGUI