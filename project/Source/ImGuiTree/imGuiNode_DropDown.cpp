#include "imGuiNode_DropDown.h"

#ifdef IMGUI

#include <windows.h>	// min,max‚ðŽg—p‚·‚é‚½‚ß

ImGuiNode_DropDown::ImGuiNode_DropDown(std::string _label, const std::vector<std::string>& _list, std::function<void(const std::string&)> func, std::string _buttonText) {

	label = _label;
	list = _list;

	currentIndex = 0;
	currentItem = _list[currentIndex];

	action = func;
	buttonText = _buttonText;
}

ImGuiNode_DropDown::~ImGuiNode_DropDown()
{
}

void ImGuiNode_DropDown::Update() {

	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();

	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##custom combo", currentItem.c_str(), ImGuiComboFlags_NoArrowButton)) {

		for (int n = 0; n < list.size(); n++) {
			bool is_selected = (currentItem == list[n]);
			if (ImGui::Selectable(list[n].c_str(), is_selected)) {
				currentItem = list[n];
				currentIndex = n;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##l", ImGuiDir_Left)) {
		currentIndex = max(currentIndex - 1, 0);
		currentItem = list[currentIndex];
	}

	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
		currentIndex = min(currentIndex + 1, static_cast<int>(list.size()) - 1);
		currentItem = list[currentIndex];
	}

	ImGui::SameLine(0, style.ItemInnerSpacing.x);
	ImGui::Text("List");

	if (action != nullptr) {
		if (ImGui::Button(buttonText.c_str())) {
			action(currentItem);
		}
	}

	ImGuiNode_Base::Update();
}

#endif // IMGUI
