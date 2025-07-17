#include "framework/App.h"
#include "framework/sceneManager.h"
#include "src/util/time/GameTime.h"
#include "src/util/input/InputManager.h"
#include "src/util/math/Random.h"
#include "src/util/singleton/singleton.h"
#include "src/common/game/GameManager.h"

#include "vendor/ImGui/imgui.h"

#include "src/common/camera/CameraMAnager.h"
#include "src/util/shadow_map/ShadowMap.h"
#include "src/common/system/SystemManager.h"

bool exitFlag = false;

void AppInit()
{
	SceneManager::Start();
	exitFlag = false;

	CameraManager::Init();

	ShadowMap::Init(4096, 4096);
}

void AppUpdate()
{
	if (InputManager::Push(KeyDefine::KeyCode::Escape))
		exitFlag = true;

	GTime.Update();
	SceneManager::Update();
	CameraManager::Update();

	Random.SetSeed(Random.GetInt());
}

void AppDraw()
{
	const int cameraNum = (int)CameraManager::AllCameras().size();

	if (cameraNum > 1 && CameraManager::IsScreenDivision()) {
		for (int i = 0; i < cameraNum; i++) {

			CameraManager::DrawScreenDivsition(i);

			ShadowMap::DrawBegin(i);
				SceneManager::Draw();

			ShadowMap::DrawEnd();
				CameraManager::DrawScreenDivsition(i);
				SceneManager::Draw();

		ShadowMap::CleanUp();
		}
	}
	else {

		CameraManager::Draw();
		CameraManager::DefaultScreenSize();

		ShadowMap::DrawBegin();
			SceneManager::Draw();
	
		ShadowMap::DrawEnd();
	
			CameraManager::Draw();
			CameraManager::DefaultScreenSize();
			SceneManager::Draw();
	
		ShadowMap::CleanUp();

	}

	//SetCameraPositionAndTarget_UpVecY(CameraManager::MainCamera()->WorldPos(), CameraManager::MainCamera()->Target());
}

void AppRelease()
{
	SceneManager::Release();
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