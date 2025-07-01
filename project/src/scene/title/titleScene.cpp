#include "titleScene.h"

// ◇汎用
#include "framework/SceneManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"


// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

#include "src/common/setting/SettingManager.h"
#include "src/util/easing/easing.h"

TitleScene::TitleScene(std::string name) : SceneBase(true, name) {

	Fader::FadeIn(1.0f, EasingType::Linear);

	SettingManager* settingManager = Instantiate<SettingManager>();
}

TitleScene::~TitleScene() {

}

void TitleScene::Update() {

	switch (sceneState) {
	case SceneState::BeforePlay:BeforePlayUpdate();	break;
	case SceneState::InPlay:	InPlayUpdate();		break;
	case SceneState::AfterPlay:	AfterPlayUpdate();	break;
	}

	if (InputManager::Push("AnyKey")) {
		SceneManager::ChangeScene("PlayScene");
	}

	SceneBase::Update();
}

void TitleScene::Draw() {

	SceneBase::Draw();
	
	DrawString(100, 400, "Push [Z]Key To Play", GetColor(255, 255, 255));

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
