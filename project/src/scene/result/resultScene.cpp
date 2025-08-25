#include "resultScene.h"

// ◇汎用
#include "framework/SceneManager.h"

// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

#include "src/common/setting/SettingManager.h"
#include "src/util/easing/easing.h"

//=== サウンド ===
#include "src/util/sound/SoundManager.h"

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

	m_ResultData = gameM->GetResultData();
	const int size = (int)gameM->GetResultData().WinnerCharaIDs.size();
	for (int i = 0; i < size; i++)
	{
		Vector3 position = Vector3((i - (size - 1) / 2.0f) * 80.0f, 0.0f, 200.0f);
		charaM->Create(gameM->GetResultData().WinnerTeamName[0], Transform(position, Vector3::Zero, Vector3::Ones));
	}

	Instantiate<UI_Setter_ResultScene>();

	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	for (int i = 0; i < CAMERA_NUM; ++i) {
		Camera* camera = CameraManager::GetCamera(i);
		if (camera)
			camera->SetPerformance("ResultScene");
	}

	if (m_ResultData.WinnerTeamName.empty())
	{
		SoundManager::FadeIn("BGM_ResultScene_Draw.wav", "BGM", 1.0f, EasingType::Linear);
	}
	else
	{
		SoundManager::FadeIn("BGM_ResultScene.wav", "BGM", 1.0f, EasingType::Linear);
		SoundManager::Play("BGM_ResultScene_Fanfare.wav", "Fanfare");
	}
}

ResultScene::~ResultScene() {

	SoundManager::Stop("BGM_ResultScene.wav", "BGM");
	SoundManager::Stop("BGM_ResultScene_Draw.wav", "BGM");
	CameraManager::SetIsScreenDivision(false);
}

void ResultScene::Update() {

	switch (sceneState) {
	case SceneState::BeforePlay:BeforePlayUpdate();	break;
	case SceneState::InPlay:	InPlayUpdate();		break;
	case SceneState::AfterPlay:	AfterPlayUpdate();	break;
	}

	SceneBase::Update();
}

void ResultScene::Draw() {

	SceneBase::Draw();
}

void ResultScene::BeforePlayUpdate()
{
	if (not Fader::IsPlaying())
		sceneState = SceneState::InPlay;
}

void ResultScene::InPlayUpdate()
{
	int padNumber = DX_INPUT_PAD1;

	if (not m_ResultData.WinnerCharaIDs.empty())
		padNumber = m_ResultData.WinnerCharaIDs[0] + 1;

	if (not CameraManager::IsPlayingPerformance() && InputManager::Push("AnyKey", padNumber))
	{
		if (m_ResultData.WinnerTeamName.empty())
		{
			SoundManager::FadeOut("BGM_ResultScene_Draw.wav", "BGM", 1.0f, EasingType::Linear);
		}
		else
		{
			SoundManager::FadeOut("BGM_ResultScene.wav", "BGM", 1.0f, EasingType::Linear);
		}

		Fader::FadeOut(1.0f, EasingType::Linear);
		sceneState = SceneState::AfterPlay;
	}
}

void ResultScene::AfterPlayUpdate()
{
	if (not Fader::IsPlaying())
		SceneManager::ChangeScene("TitleScene");
}
