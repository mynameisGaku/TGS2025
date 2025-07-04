#include "resultScene.h"

// ◇汎用
#include "framework/SceneManager.h"


// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

#include "src/common/setting/SettingManager.h"
#include "src/util/easing/easing.h"

#include "src/common/camera/CameraManager.h"
#include "src/scene/result/ui/UI_Setter_ResultScene.h"

#include "src/reference/camera/CameraPerformanceRef.h"

ResultScene::ResultScene(const std::string& name) : SceneBase(true, name) {

	Fader::FadeIn(1.0f, EasingType::Linear);

	SettingManager* settingManager = Instantiate<SettingManager>();

	Instantiate<UI_Setter_ResultScene>();

	Camera* mainCamera = CameraManager::MainCamera();
	mainCamera->SetPerformance("ResultScene");
}

ResultScene::~ResultScene() {

}

void ResultScene::Update() {

	switch (sceneState) {
	case SceneState::BeforePlay:BeforePlayUpdate();	break;
	case SceneState::InPlay:	InPlayUpdate();		break;
	case SceneState::AfterPlay:	AfterPlayUpdate();	break;
	}

	if (InputManager::Push(KeyDefine::KeyCode::T)) {
		SceneManager::ChangeScene("TitleScene");
	}

	SceneBase::Update();
}

void ResultScene::Draw() {

	SceneBase::Draw();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));

}

void ResultScene::BeforePlayUpdate()
{
}

void ResultScene::InPlayUpdate()
{
}

void ResultScene::AfterPlayUpdate()
{
}
