#include "imGuiManager.h"

// ��ImGui�̋@�\���g�p���邩
#ifdef IMGUI

// ���ėp
#include "../../Source/Util/Utils.h"
#include "../../Library/sceneManager.h"
#include "../../Library/sceneBase.h"

namespace {

	static const ImVec2 DEBUG_WINDOW_SIZE = ImVec2(270, Screen::HEIGHT);	// �f�o�b�O�E�B���h�E�̑傫��
	static const ImVec2 DEBUG_WINDOW_POS = ImVec2(0, 0);					// �f�o�b�O�E�B���h�E�̍��W

	static const ImVec2 INFO_WINDOW_SIZE = ImVec2(200, 200);				// �C���t�H���[�V�����E�B���h�E�̑傫��
	static const ImVec2 INFO_WINDOW_POS = ImVec2(Screen::WIDTH - 200, 0);	// �C���t�H���[�V�����E�B���h�E�̍��W

	bool isActive;	// �������邩

	std::map<std::string, ImGuiRoot*>* infoRoots;		// �Q�[���̏�ԁE�����E��ɕ\������
	std::map<std::string, ImGuiNode_Base*>* infoNodes;	// �Q�[���̏�ԁE�����E��ɕ\������

	std::unordered_map<std::string, ImGuiRoot*>* roots;
	std::unordered_map<std::string, ImGuiNode_Base*>* nodes;
}

void ImGuiManager::Init() {

	isActive = true;

	CheckInstance();

	AddNodeInfo(new ImGuiNode_Text("SceneName", "", ImGuiFunc::Scene::TranscribeSceneName));
	AddNodeInfo(new ImGuiNode_Text("FPS", "", ImGuiFunc::FPS::TranscribeFPS));
}

void ImGuiManager::Update() {

	if (isActive == false)
		return;

	ImGuiFunc::Update();
	ImGuiUpdate();
	ImGuiInfoUpdate();
}

void ImGuiManager::ImGuiUpdate() {

	//=====================================================
	// ��Debug Window

	ImGui::SetNextWindowSize(DEBUG_WINDOW_SIZE, ImGuiCond_Once);
	ImGui::SetNextWindowPos(DEBUG_WINDOW_POS, ImGuiCond_Once);
	ImGui::Begin("Debug Window");
	ImGui::Text("Setting List");

	//=====================================================
	// ��Roots

	if (roots != nullptr) {
		for (const auto& itr : *roots) {
			if (itr.second != nullptr)
				itr.second->Update();
		}
	}

	//=====================================================
	// ��Nodes

	if (nodes != nullptr) {
		for (const auto& itr : *nodes) {
			if (itr.second != nullptr)
				itr.second->Update();
		}
	}

	ImGui::End();

	//=====================================================
}

void ImGuiManager::ImGuiInfoUpdate() {

	//=====================================================
	// ��Infomation Window

	ImGui::SetNextWindowSize(INFO_WINDOW_SIZE, ImGuiCond_Once);
	ImGui::SetNextWindowPos(INFO_WINDOW_POS, ImGuiCond_Once);
	ImGui::Begin("Infomation Window");
	ImGui::Text("Infomation");

	//=====================================================
	// ��InfoRoots

	if (infoRoots != nullptr) {
		for (const auto& itr : *infoRoots) {
			if (itr.second != nullptr)
				itr.second->Update();
		}
	}

	//=====================================================
	// ��InfoNodes

	if (infoNodes != nullptr) {
		for (const auto& itr : *infoNodes) {
			if (itr.second != nullptr)
				itr.second->Update();
		}
	}

	ImGui::End();

	//=====================================================
}

void ImGuiManager::Draw() {

	if (isActive == false)
		return;

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Release() {

	isActive = false;

	//=====================================================
	// ��Nodes�̉��
	
	if (nodes != nullptr) {
		for (auto itr = (*nodes).begin(); itr != (*nodes).end();) {
			delete itr->second;
			itr = (*nodes).erase(itr);
		}

		Function::DeletePointer(nodes);
	}

	//=====================================================
	// ��Roots�̉��
	
	if (roots != nullptr) {
		for (auto itr = (*roots).begin(); itr != (*roots).end();) {
			delete itr->second;
			itr = (*roots).erase(itr);
		}

		Function::DeletePointer(roots);
	}

	//=====================================================
	// ��InfoNodes�̉��

	if (infoNodes != nullptr) {
		for (auto itr = (*infoNodes).begin(); itr != (*infoNodes).end();) {
			delete itr->second;
			itr = (*infoNodes).erase(itr);
		}

		Function::DeletePointer(infoNodes);
	}

	//=====================================================
	// ��InfoRoots�̉��

	if (infoRoots != nullptr) {
		for (auto itr = (*infoRoots).begin(); itr != (*infoRoots).end();) {
			delete itr->second;
			itr = (*infoRoots).erase(itr);
		}

		Function::DeletePointer(infoRoots);
	}

	//=====================================================
}

void ImGuiManager::CheckInstance() {

	if (infoRoots == nullptr)
		infoRoots = new std::map<std::string, ImGuiRoot*>;

	if (infoNodes == nullptr)
		infoNodes = new std::map<std::string, ImGuiNode_Base*>;

	if (roots == nullptr)
		roots = new std::unordered_map<std::string, ImGuiRoot*>;

	if (nodes == nullptr)
		nodes = new std::unordered_map<std::string, ImGuiNode_Base*>;
}

//===================================================================================
// ���f�o�b�O�E�B���h�E�֘A

ImGuiNode_Base* ImGuiManager::AddNode(ImGuiNode_Base* node) {

	// �m�[�h�Ǘ����s�����̂��Ȃ��ꍇ�A��������
	if (nodes == nullptr)
		nodes = new std::unordered_map<std::string, ImGuiNode_Base*>;

	(*nodes)[node->Label()] = node;

	return node;
}

ImGuiRoot* ImGuiManager::AddRoot(ImGuiRoot* root) {

	// ���[�g�Ǘ����s�����̂��Ȃ��ꍇ�A��������
	if (roots == nullptr)
		roots = new std::unordered_map<std::string, ImGuiRoot*>;

	(*roots)[root->Label()] = root;

	return root;
}

ImGuiRoot* ImGuiManager::FindRoot(std::string label) {

	if (roots == nullptr)
		return nullptr;

	if ((*roots).contains(label) == false) {

		for (const auto& itr : *roots) {
			ImGuiRoot* children = itr.second->SearchChildren(label);
			if (children != nullptr)
				return children;
		}

		return nullptr;
	}

	return (*roots)[label];
}

void ImGuiManager::Delete(std::string label) {

	// ��Nodes������
	if (nodes != nullptr) {

		for (auto itr = nodes->begin(); itr != nodes->end();) {
			if (itr->first != label) {
				itr++;
				continue;
			}

			delete itr->second;
			itr = nodes->erase(itr);
			break;
		}
	}

	// ��Roots������
	if (roots != nullptr) {

		for (auto itr = roots->begin(); itr != roots->end();) {

			ImGuiRoot* children = itr->second->SearchChildren(label);

			if (children != nullptr) {
				itr->second->Delete(children);
				children = nullptr;
			}

			if (itr->first != label) {
				itr++;
				continue;
			}

			delete itr->second;
			itr = roots->erase(itr);
			break;
		}
	}
}

//===================================================================================
// ���C���t�H���[�V�����E�B���h�E�֘A

ImGuiNode_Base* ImGuiManager::AddNodeInfo(ImGuiNode_Base* node) {

	if (infoNodes == nullptr)
		infoNodes = new std::map<std::string, ImGuiNode_Base*>;

	(*infoNodes)[node->Label()] = node;

	return node;
}

ImGuiRoot* ImGuiManager::AddRootInfo(ImGuiRoot* root) {

	if (infoRoots == nullptr)
		infoRoots = new std::map<std::string, ImGuiRoot*>;

	(*infoRoots)[root->Label()] = root;

	return root;
}

ImGuiRoot* ImGuiManager::FindRootInfo(std::string label) {

	if (infoRoots == nullptr)
		return nullptr;

	if ((*infoRoots).contains(label) == false) {

		for (const auto& itr : *infoRoots) {
			ImGuiRoot* children = itr.second->SearchChildren(label);
			if (children != nullptr)
				return children;
		}

		return nullptr;
	}

	return (*infoRoots)[label];
}

void ImGuiManager::DeleteInfo(std::string label) {

	// ��InfoNodes������
	if (infoNodes != nullptr) {

		for (auto itr = infoNodes->begin(); itr != infoNodes->end();) {
			if (itr->first != label) {
				itr++;
				continue;
			}

			delete itr->second;
			itr = infoNodes->erase(itr);
			break;
		}
	}

	// ��InfoRoots������
	if (infoRoots != nullptr) {

		for (auto itr = infoRoots->begin(); itr != infoRoots->end();) {

			ImGuiRoot* children = itr->second->SearchChildren(label);

			if (children != nullptr) {
				itr->second->Delete(children);
				children = nullptr;
			}

			if (itr->first != label) {
				itr++;
				continue;
			}

			delete itr->second;
			itr = infoRoots->erase(itr);
			break;
		}
	}
}

void ImGuiManager::SetIsActive(bool value) { isActive = value; }
bool ImGuiManager::IsActive() { return isActive; }

const std::map<std::string, ImGuiRoot*>* const ImGuiManager::InfoRoots() { return infoRoots; }
const std::map<std::string, ImGuiNode_Base*>* const ImGuiManager::InfoNodes() { return infoNodes; }
const std::unordered_map<std::string, ImGuiRoot*>* const ImGuiManager::Roots() { return roots; }
const std::unordered_map<std::string, ImGuiNode_Base*>* const ImGuiManager::Nodes() { return nodes; }

#endif // IMGUI