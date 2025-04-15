#include "imGuiNode_SliderInt.h"

#ifdef IMGUI

ImGuiNode_SliderInt::ImGuiNode_SliderInt(std::string _label, int* _value, int _min, int _max, std::string _format) {

	label = _label;
	format = _format;

	value = _value;
	min = _min;
	max = _max;
}

ImGuiNode_SliderInt::~ImGuiNode_SliderInt() {

	value = nullptr;
}

void ImGuiNode_SliderInt::Update() {

	if (value != nullptr)
		ImGui::SliderInt(label.c_str(), value, min, max, format.c_str());

	ImGuiNode_Base::Update();
}

#endif // IMGUI