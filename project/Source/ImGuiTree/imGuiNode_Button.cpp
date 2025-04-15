#include "imGuiNode_Button.h"

#ifdef IMGUI

ImGuiNode_Button::ImGuiNode_Button(std::string _label, std::function<void()> func) {

	label = _label;
	action = func;
}

ImGuiNode_Button::~ImGuiNode_Button() {

}

void ImGuiNode_Button::Update() {

	if (ImGui::Button(label.c_str())) {
		if (action != nullptr)
			action();
	}

	ImGuiNode_Base::Update();
}

#endif // IMGUI