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
	for (int i = 0; i < (int)CameraManager::AllCameras().size(); i++)
		CameraManager::ResetCamera(i);

	Fader::FadeIn(1.0f, EasingType::Linear);

	GameManager* gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();
	m_ResultData = gameM->GetResultData();

	SettingManager* settingManager = Instantiate<SettingManager>();
	CharaManager* charaM = Instantiate<CharaManager>();

	if (m_ResultData.WinnerCharaIDs.empty())
	{
		m_Winner_PadNumber = DX_INPUT_PAD1;
		m_Exists_Winner = false;
	}
	else
	{
		m_Winner_PadNumber = m_ResultData.WinnerCharaIDs[0] + 1;
		m_Exists_Winner = true;

		const int size = (int)m_ResultData.WinnerCharaIDs.size();
		for (int i = 0; i < size; i++)
		{
			Vector3 position = Vector3((i - (size - 1) / 2.0f) * 80.0f, 0.0f, 200.0f);
			charaM->Create(gameM->GetResultData().WinnerTeamName[0], Transform(position, Vector3::Zero, Vector3::Ones));
		}
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
		m_IsPlayingBGM_Flag = true;
	}
	else
	{
		SoundManager::Play("BGM_ResultScene_Fanfare.wav", "Fanfare");
		m_IsPlayingBGM_Flag = false;
	}
}

ResultScene::~ResultScene() {

	SoundManager::Stop("BGM_ResultScene.wav", "BGM");
	SoundManager::Stop("BGM_ResultScene_Draw.wav", "BGM");
	SoundManager::Stop("BGM_ResultScene_Fanfare.wav", "BGM");
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
	bool isPlayingCameraPerf = CameraManager::IsPlayingPerformance();
	bool isInput = InputManager::Push("AnyKey", m_Winner_PadNumber);

	bool isPlayingFanfare = false;
	bool isPlayingResultBGM = false;
	float playingResultBGM_Rate = 1.0f;

	if (m_Exists_Winner)
	{
		float fanfareRate = SoundManager::GetPlayingSoundRate("BGM_ResultScene_Fanfare.wav", "Fanfare");
		isPlayingFanfare = SoundManager::IsPlaying("BGM_ResultScene_Fanfare.wav", "Fanfare");
		isPlayingResultBGM = SoundManager::IsPlaying("BGM_ResultScene.wav", "BGM");
		playingResultBGM_Rate = SoundManager::GetPlayingSoundRate("BGM_ResultScene.wav", "BGM");

		if (not isPlayingResultBGM && isPlayingFanfare && fanfareRate >= 0.8f)
		{
			SoundManager::Play("BGM_ResultScene.wav", "BGM");
			m_IsPlayingBGM_Flag = true;
			isPlayingResultBGM = true;
		}
	}
	else
	{
		isPlayingResultBGM = SoundManager::IsPlaying("BGM_ResultScene_Draw.wav", "BGM");
		playingResultBGM_Rate = SoundManager::GetPlayingSoundRate("BGM_ResultScene_Draw.wav", "BGM");
	}

	bool canGoNext = (not isPlayingCameraPerf && isInput);
	bool stopBGM = (not isPlayingResultBGM || playingResultBGM_Rate >= 0.9f) && m_IsPlayingBGM_Flag;

	if (canGoNext || stopBGM)
	{
		if (m_Exists_Winner)
		{
			if (isPlayingResultBGM)
				SoundManager::FadeOut("BGM_ResultScene.wav", "BGM", 1.0f, EasingType::Linear);

			if (isPlayingFanfare)
				SoundManager::FadeOut("BGM_ResultScene_Fanfare.wav", "Fanfare", 1.0f, EasingType::Linear);
		}
		else
		{
			if (SoundManager::IsPlaying("BGM_ResultScene_Draw.wav", "BGM"))
				SoundManager::FadeOut("BGM_ResultScene_Draw.wav", "BGM", 1.0f, EasingType::Linear);
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
