#include "titleScene.h"

// ◇汎用
#include "../Library/sceneManager.h"
#include "../Library/resourceLoader.h"
#include "DebugScreen.h"

// ◇演出・機能
#include "InputManager.h"
#include "Fader.h"

#include "SettingManager.h"

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

	if (InputManager::Push(KeyDefine::KeyCode::P)) {
		SceneManager::ChangeScene("PlayScene");
	}

	SceneBase::Update();
}

void TitleScene::Draw() {

	SceneBase::Draw();
	
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));

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
