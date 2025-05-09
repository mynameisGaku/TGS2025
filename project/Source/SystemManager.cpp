#include "systemManager.h"

// ◇汎用
#include "../Library/resourceLoader.h"
#include "../Library/time.h"

// ◇演出・機能
#include "LoadScreen.h"

#include "ImGuiTree/imGuiManager.h"
#include "Font.h"
#include "InputManager.h"
#include "MouseController.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "Fader.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ShadowMap.h"
#include "Stage.h"
#include "StageObjectManager.h"
#include "UI_Manager.h"

// ◇個別で必要な物
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

SystemManager::SystemManager() {

	loadIndex = ltNone;

	isLoading = true;
	loadScreen = nullptr;

	ResourceLoader::Init();
}

SystemManager::~SystemManager() {

	Font::Release();
	InputManager::Release();
	SoundManager::Release();
	EffectManager::Release();
	Fader::Release();
	CameraManager::Release();
	LightManager::Release();
	Stage::Release();
	StageObjectManager::Release();
	UI_Manager::Release();

#ifdef IMGUI
	ImGuiManager::Release();
#endif // IMGUI

	ResourceLoader::ReleaseAllFile();
}

void SystemManager::Start() {

	loadScreen = SceneManager::CommonScene()->FindGameObject<LoadScreen>();

	if (loadScreen != nullptr) {
		loadScreen->FadeIn(0.1f);
		loadScreen->SetIsPushFadeOut(true);
	}
}

void SystemManager::Update() {

	if (isLoading) {
		float rate = (static_cast<float>(loadIndex) / static_cast<int>(LoadingType::ltMax));
		loadScreen->SetRate(rate);
		return;
	}

	InputManager::Update();
	SoundManager::Update();
	EffectManager::Update();
	Fader::Update();
	CameraManager::Update();
	LightManager::Update();
	Stage::Update();
	StageObjectManager::Update();
	UI_Manager::Update();

#ifdef IMGUI

	ImGuiManager::Update();

#endif // IMGUI

#ifdef _DEBUG

	Debug();

#endif // _DEBUG

	GameObject::Update();
}

void SystemManager::Draw() {

	GameObject::Draw();

	if (isLoading)
		return;

	EffectManager::Draw();

	UI_Manager::DrawBack();

	Fader::Draw();

#ifdef IMGUI

	ImGuiManager::Draw();

#endif // IMGUI
}

void SystemManager::DrawBefore() {

	if (isLoading)
		return;

	CameraManager::Draw();
	LightManager::Draw();
	Stage::Draw();
	StageObjectManager::Draw();
}

void SystemManager::LoadUpdate() {

	if (isLoading == false)
		return;

	switch (loadIndex) {
	case ltImGuiManager:

#ifdef IMGUI

		using namespace ImGuiFunc;

		ImGuiManager::Init();
		ImGuiManager::SetIsActive(true);

#endif // IMGUI
		break;

	case ltFont:			Font::Init();			break;
	case ltInputManager:	InputManager::Init();	break;
	case ltSoundManager:
		SoundManager::Init();
		SoundManager::LoadToCsv("data/csv/SoundData.csv");
		break;

	case ltEffectManager:
		EffectManager::Init();
		//EffectManager::LoadToCsv("data/csv/EffectData.csv");
		// エフェクト読み込む
		{
			//EffectManager::LoadFromJson("data/json/effect/Catch_Ready.json");
			//EffectManager::LoadFromJson("data/json/effect/Catch_Ready_Single_Line.json");
			EffectManager::LoadFromJson("data/json/effect/Catch_Ready_Single_Dust.json");
			EffectManager::LoadFromJson("data/json/effect/Catch_Ready_Single_Tornado.json");
			EffectManager::LoadFromJson("data/json/effect/Catch_Success.json");
			EffectManager::LoadFromJson("data/json/effect/Hit_Blue.json");
			EffectManager::LoadFromJson("data/json/effect/Hit_Gray.json");
			EffectManager::LoadFromJson("data/json/effect/Hit_Red.json");
			EffectManager::LoadFromJson("data/json/effect/Hit_NoColor.json");
		}
		break;

	case ltFader:			Fader::Init();			break;
	case ltCameraManager:	CameraManager::Init();	break;
	case ltLightManager:	LightManager::Init();	break;
	//case ltShadowMap:		ShadowMap::Init();		break;
	case ltStage:			Stage::Init();			break;
	case ltStageManager:
		StageObjectManager::Init();
		StageObjectManager::SetModelFilePath("data/model/Stage/");
		StageObjectManager::LoadFromJson("data/json/Stage/Stage.json");
		break;

	case ltUI_Manager:		UI_Manager::Init();		break;

	default:
		break;
	}

	loadIndex = min((LoadingType)((int)(loadIndex) + 1), LoadingType::ltMax);

	if (loadIndex == LoadingType::ltMax && loadScreen != nullptr && loadScreen->IsLoadEnd()) {
		isLoading = false;
		loadScreen->SetIsPushFadeOut(false);
		loadScreen->FadeOut(0.75f);
	}
}

bool SystemManager::IsLoadEnd() const {

	if (loadScreen == nullptr)
		return (isLoading == false);
	
	return (isLoading == false && loadScreen->IsLoadEnd());
}

#ifdef _DEBUG

void SystemManager::Debug() {

	if (IsLoadEnd() == false)
		return;
	
	if (InputManager::Push(KeyDefine::KeyCode::F1)) {
		ImGuiManager::SetIsActive(!ImGuiManager::IsActive());
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha9)) {
		MouseController::SetMouseMovement(KeyDefine::MouseMovement::Free);
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha0)) {
		MouseController::SetMouseMovement(KeyDefine::MouseMovement::Fixed);
	}
}

#endif // _DEBUG

void DrawBefore() {

	CameraManager::Draw();
	LightManager::Draw();
	Stage::Draw();
	StageObjectManager::Draw();
}
