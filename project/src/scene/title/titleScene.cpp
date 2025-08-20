#include "titleScene.h"

// ◇汎用
#include "framework/SceneManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"


// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

#include "src/common/setting/SettingManager.h"
#include "src/util/easing/easing.h"
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>

#include <src/scene/title/ui/TitleUIController.h>
#include <src/util/ptr/PtrUtil.h>

TitleScene::TitleScene(std::string name) : SceneBase(true, name) {

	Fader::FadeIn(1.0f, EasingType::Linear);

	SettingManager* settingManager = Instantiate<SettingManager>();

	m_UIController = Instantiate<TitleUIController>();

	m_UIController->LoadCanvasesFromJson("data/json/ui/title/");
	m_UIController->Activate();
}

TitleScene::~TitleScene() {
}

void TitleScene::Update() {

	switch (sceneState) {
	case SceneState::BeforePlay:BeforePlayUpdate();	break;
	case SceneState::InPlay:	InPlayUpdate();		break;
	case SceneState::AfterPlay:	AfterPlayUpdate();	break;
	}

	SceneBase::Update();
}

void TitleScene::Draw() {

	SceneBase::Draw();

#ifdef TRUE //DEBUG

#endif
}

void TitleScene::BeforePlayUpdate()
{
}

void TitleScene::InPlayUpdate()
{
}

void TitleScene::AfterPlayUpdate()
{
}
