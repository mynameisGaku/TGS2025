#include "imGuiNode_Text.h"

#ifdef IMGUI

ImGuiNode_Text::ImGuiNode_Text(std::string _lable, std::string _format, std::function<void(ImGuiNode_Base*)> _func) {

	label = _lable;
	format = _format;

	SetFunction(_func);
}

ImGuiNode_Text::~ImGuiNode_Text() {

}

void ImGuiNode_Text::Update() {

	if (format.empty() == false)
		ImGui::Text(format.c_str());

	ImGuiNode_Base::Update();
}

#endif // IMGUI