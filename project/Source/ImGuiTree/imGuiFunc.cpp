#include "imGuiFunc.h"

#ifdef IMGUI

// ”Ä—p
#include "../../Source/Util/Utils.h"
#include "../../Library/sceneManager.h"
#include "../../Library/sceneBase.h"

// ŒÂ•Ê‚Å•K—v‚È•¨
#include "imGuiManager.h"

void ImGuiFunc::FPS::TranscribeFPS(ImGuiNode_Base* node) {

	if (node == nullptr)
		return;

	if (fpsRenderInterval <= 0) {
		ImGuiNode_Text* text = dynamic_cast<ImGuiNode_Text*>(node);

		if (text != nullptr)
			text->SetText(Function::FormatToString(DISP_TEXT, GetFPS()));
	}
}

void ImGuiFunc::FPS::Update() {

	if (fpsRenderInterval-- <= 0)
		fpsRenderInterval = RENDER_INTERVAL;
}

void ImGuiFunc::Scene::TranscribeSceneName(ImGuiNode_Base* node) {

	ImGuiNode_Text* text = dynamic_cast<ImGuiNode_Text*>(node);

	if (text != nullptr)
		text->SetText("CurrentScene : " + SceneManager::CurrentScene()->Name());
}

void ImGuiFunc::Update() {

	FPS::Update();
}

#endif // IMGUI