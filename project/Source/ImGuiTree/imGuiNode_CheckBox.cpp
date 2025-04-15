#include "imGuiNode_CheckBox.h"

#ifdef IMGUI

ImGuiNode_CheckBox::ImGuiNode_CheckBox(std::string _label, bool* value) {

	label = _label;
	isCheck = value;
}

ImGuiNode_CheckBox::~ImGuiNode_CheckBox() {

	isCheck = nullptr;
}

void ImGuiNode_CheckBox::Update() {

	if (isCheck != nullptr)
		ImGui::Checkbox(label.c_str(), isCheck);
}

#endif // IMGUI