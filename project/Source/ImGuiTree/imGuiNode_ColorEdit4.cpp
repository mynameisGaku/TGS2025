#include "imGuiNode_ColorEdit4.h"

#ifdef IMGUI

ImGuiNode_ColorEdit4::ImGuiNode_ColorEdit4(std::string _label, float* _rgba[4]) {

	label = _label;
	*rgba = *_rgba;
}

ImGuiNode_ColorEdit4::~ImGuiNode_ColorEdit4() {

	*rgba = nullptr;
}

void ImGuiNode_ColorEdit4::Update() {

	if (rgba != nullptr)
		ImGui::ColorEdit4(label.c_str(), *rgba);

	ImGuiNode_Base::Update();
}

#endif // IMGUI