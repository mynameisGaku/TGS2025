#include "titleScene.h"

// ◇汎用
#include "framework/app.h"
#include "framework/SceneManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"

// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

//=== サウンド ===
#include "src/util/sound/SoundManager.h"

#include "src/common/setting/SettingManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/easing/easing.h"
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>

#include <src/scene/title/ui/TitleUIController.h>
#include <src/util/ptr/PtrUtil.h>
#include <src/common/stage/Stage.h>

TitleScene::TitleScene(std::string name) : SceneBase(true, name) {
	Stage::Sky()->SetIsActive(false);

	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	int w, h;
	GetWindowSize(&w, &h);

	for (int i = 0; i < CAMERA_NUM; i++) {
		Camera* camera = CameraManager::GetCamera(i);
		if (camera == nullptr)
			continue;

		camera->SetDefinedDrawArea((w / CAMERA_NUM) * i, 0, w / CAMERA_NUM, h);
		camera->ApplyDefinedDrawArea();
		camera->ChangeState(&Camera::ChaseState);
	}

	if (w <= 1920)
		CameraManager::SetIsScreenDivision(false);
	else
		CameraManager::SetIsScreenDivision(true);

	Fader::FadeIn(1.0f, EasingType::Linear);
	SoundManager::FadeIn("BGM_TitleScene.wav", "BGM", 1.0f, EasingType::Linear);

	SettingManager* settingManager = Instantiate<SettingManager>();

	m_UIController = Instantiate<TitleUIController>();

	m_UIController->LoadCanvasesFromJson("data/json/ui/title/");
	m_UIController->Activate();
}

TitleScene::~TitleScene() {
	Stage::Sky()->SetIsActive(true);
	SoundManager::Stop("BGM_TitleScene.wav", "BGM");
	CameraManager::SetIsScreenDivision(false);
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
	if (not Fader::IsPlaying())
		sceneState = SceneState::InPlay;
}

void TitleScene::InPlayUpdate()
{
}

void TitleScene::AfterPlayUpdate()
{
	if (not Fader::IsPlaying())
		SceneManager::ChangeScene("PlayScene");
}
