#include "src/util/debug/imgui/imGuiFunc.h"

#ifdef IMGUI

// ◇汎用
#include "src/util/string/StringUtil.h"
#include "framework/SceneManager.h"
#include "framework/sceneBase.h"
#include <DxLib.h> // GetFPSで使用

// ◇個別で必要な物
#include "src/util/debug/imgui/imGuiManager.h"

void ImGuiFunc::FPS::TranscribeFPS(ImGuiNode_Base* node) {

	if (node == nullptr)
		return;

	if (fpsRenderInterval <= 0) {
		ImGuiNode_Text* text = dynamic_cast<ImGuiNode_Text*>(node);

		if (text != nullptr)
			text->SetText(StringUtil::FormatToString(DISP_TEXT, GetFPS()));
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