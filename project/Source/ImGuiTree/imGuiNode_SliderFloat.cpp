#include "imGuiNode_SliderFloat.h"

#ifdef IMGUI

ImGuiNode_SliderFloat::ImGuiNode_SliderFloat(std::string _label, float* _value, float _min, float _max, std::string _format) {

	label = _label;
	format = _format;

	value = _value;
	min = _min;
	max = _max;
}

ImGuiNode_SliderFloat::~ImGuiNode_SliderFloat() {

	value = nullptr;
}

void ImGuiNode_SliderFloat::Update() {

	if (value != nullptr)
		ImGui::SliderFloat(label.c_str(), value, min, max, format.c_str());

	ImGuiNode_Base::Update();
}

#endif // IMGUI