#include "resultScene.h"

// ◇汎用
#include "framework/SceneManager.h"


// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

#include "src/common/setting/SettingManager.h"
#include "src/util/easing/easing.h"

#include "src/common/game/GameManager.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/result/ui/UI_Setter_ResultScene.h"

#include "src/reference/camera/CameraPerformanceRef.h"

ResultScene::ResultScene(const std::string& name) : SceneBase(true, name) {

	CameraManager::SetIsScreenDivision(true);
	Fader::FadeIn(1.0f, EasingType::Linear);

	GameManager* gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();

	SettingManager* settingManager = Instantiate<SettingManager>();

	CharaManager* charaM = Instantiate<CharaManager>();

	const int size = (int)gameM->GetResultData().WinnerCharaIDs.size();
	for (int i = 0; i < size; i++)
	{
		Vector3 position = Vector3((i - (size - 1) / 2.0f) * 80.0f, 0.0f, 0.0f);
		charaM->Create(gameM->GetResultData().WinnerTeamName[0], Transform(position, Vector3::Zero, Vector3::Ones));
	}

	Instantiate<UI_Setter_ResultScene>();

	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	for (int i = 0; i < CAMERA_NUM; ++i) {
		Camera* camera = CameraManager::GetCamera(i);
		if (camera)
			camera->SetPerformance("ResultScene");
	}
}

ResultScene::~ResultScene() {

	CameraManager::SetIsScreenDivision(false);
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
