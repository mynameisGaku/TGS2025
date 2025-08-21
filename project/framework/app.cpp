#include "framework/App.h"
#include "framework/sceneManager.h"
#include "src/util/time/GameTime.h"
#include "src/util/input/InputManager.h"
#include "src/util/math/Random.h"
#include "src/util/singleton/singleton.h"
#include "src/common/game/GameManager.h"

#include "vendor/ImGui/imgui.h"

#include "src/common/camera/CameraManager.h"
#include "src/common/system/SystemManager.h"

#include "src/util/screen/ScreenManager.h"
#include "src/util/shadow_map/ShadowMap.h"

#include "src/util/ui/UI_Manager.h"
#include "src/util/fader/Fader.h"

bool exitFlag = false;

void AppInit()
{
	ScreenManager::Init();
	SceneManager::Start();
	exitFlag = false;

	CameraManager::Init();
	Fader::Init();

	ShadowMap::Init(4096, 4096);
}

void AppUpdate()
{
	if (InputManager::Push(KeyDefine::KeyCode::Escape))
		exitFlag = true;

	GTime.Update();
	ScreenManager::Update();
	SceneManager::Update();
	CameraManager::Update();
	Fader::Update();

	Random.SetSeed(Random.GetInt());
}

void AppDraw()
{
	int cameraIndex = 0;

	for (const auto& camera : CameraManager::AllCameras())
	{
		if (not camera->IsActive() || not camera->IsView())
			continue;

		const std::string scrName = "CameraScreen_No." + std::to_string(cameraIndex);
		int drawX, drawY, drawW, drawH;

		camera->GetUsingDrawArea(&drawX, &drawY, &drawW, &drawH);

		ScreenImageData* pData = ScreenManager::GetScreenImageData(scrName);
		if (pData == nullptr)
		{
			ScreenManager::CreateScreen(scrName, drawX, drawY, drawW, drawH);
		}
		else
		{
			pData->drawX = drawX;
			pData->drawY = drawY;
			pData->drawW = drawW;
			pData->drawH = drawH;
		}

		ShadowMap::DrawBegin(cameraIndex);
			SceneManager::Draw();
		ShadowMap::DrawEnd();
		
		ScreenManager::DrawBegin(scrName);

		camera->Draw();
		SceneManager::Draw();
		ShadowMap::CleanUp();

		ScreenManager::DrawEnd(scrName);
		UI_Manager::DrawBack();

		cameraIndex++;
	}

	ScreenManager::CleanUp();
	Fader::Draw();
	UI_Manager::DrawFront();
}

void AppRelease()
{
	ScreenManager::Release();
	SceneManager::Release();
	Fader::Release();
	SingletonDeleter::Delete();
}

void Exit()
{
	exitFlag = true;
}

bool IsExit()
{
	return exitFlag;
}