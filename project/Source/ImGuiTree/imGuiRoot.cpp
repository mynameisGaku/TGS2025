#include "imGuiRoot.h"

#ifdef IMGUI

// ž”Ä—p
#include "../../Library/myDxLib.h"

ImGuiRoot::ImGuiRoot(std::string _label) {

	label = _label;
	isOpen = false;

	beginChildren = nullptr;
	endChildren = nullptr;
}

ImGuiRoot::~ImGuiRoot() {

	for (auto itr = children.begin(); itr != children.end();) {
		delete* itr;
		itr = children.erase(itr);
		if (itr == children.end()) break;
	}

	for (auto itr = node.begin(); itr != node.end();) {
		delete* itr;
		itr = node.erase(itr);
		if (itr == node.end()) break;
	}

	if (beginChildren != nullptr) {
		delete beginChildren;
		beginChildren = nullptr;
	}

	if (endChildren != nullptr) {
		delete endChildren;
		endChildren = nullptr;
	}
}

void ImGuiRoot::Update() {

	if (ImGui::TreeNode(label.c_str())) {

		isOpen = true;

		// ¥‚Ü‚Æ‚ß‚éˆ—
		if (beginChildren != nullptr)
			beginChildren->Update();

		// ¥eŽqŠK‘wNode‚Ìˆ—
		for (const auto& itr : children) {
			if (itr != nullptr)
				itr->Update();
		}

		// ¥ŠŽNode‚Ìˆ—
		for (const auto& itr : node) {
			if (itr != nullptr)
				itr->Update();
		}

		// ¥‚Ü‚Æ‚ß‚éˆ—‚Ì’âŽ~
		if (endChildren != nullptr)
			endChildren->Update();

		ImGui::TreePop();
	}
	else
		isOpen = false;
}

ImGuiNode_Base* ImGuiRoot::Add(ImGuiNode_Base* _node) {

	node.push_back(_node);
	return _node;
}

ImGuiRoot* ImGuiRoot::AddChild(ImGuiRoot* root) {

	children.push_back(root);
	return root;
}

void ImGuiRoot::Delete(std::string nodeLabel) {

	for (auto itr = node.begin(); itr != node.end(); itr++) {
		if ((*itr)->Label() == nodeLabel) {
			delete* itr;
			node.erase(itr);
			break;
		}
	}

	for (auto itr = children.begin(); itr != children.end(); itr++) {
		if ((*itr)->Label() == nodeLabel) {
			delete* itr;
			children.erase(itr);
			break;
		}
	}
}

void ImGuiRoot::Delete(ImGuiNode_Base* _node) {

	for (auto itr = node.begin(); itr != node.end(); itr++) {
		if (*itr == _node) {
			delete* itr;
			node.erase(itr);
			break;
		}
	}

	for (auto itr = children.begin(); itr != children.end(); itr++) {
		if (*itr == _node) {
			delete* itr;
			children.erase(itr);
			break;
		}
	}
}

void ImGuiRoot::NodeBeginChild(float windowSizeX, float windowSizeY) {

	node.push_back(new ImGuiNode_BeginChildren(windowSizeX, windowSizeY));
}

void ImGuiRoot::NodeEndChild() {

	node.push_back(new ImGuiNode_EndChildren());
}

void ImGuiRoot::SetChild(float windowSizeX, float windowSizeY) {

	if (beginChildren != nullptr)
		delete beginChildren;

	if (endChildren != nullptr)
		delete endChildren;

	beginChildren = new ImGuiNode_BeginChildren(windowSizeX, windowSizeY);
	endChildren = new ImGuiNode_EndChildren();
}

ImGuiRoot* ImGuiRoot::Children(std::string nodeLabel) {

	for (auto itr : children) {
		if (itr->Label() == nodeLabel)
			return itr;
	}
	return nullptr;
}

ImGuiRoot* ImGuiRoot::SearchChildren(std::string nodeLabel) {

	for (auto itr : children) {
		if (itr->Label() == nodeLabel) {
			return itr;
		}
		else {
			auto search = itr->SearchChildren(nodeLabel);
			if (search != nullptr)
				return search;
		}
	}
	return nullptr;
}

#endif // IMGUI