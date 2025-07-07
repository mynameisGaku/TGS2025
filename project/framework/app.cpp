#include "framework/App.h"
#include "framework/sceneManager.h"
#include "src/util/time/GameTime.h"
#include "src/util/input/InputManager.h"
#include "src/util/math/Random.h"
#include "src/util/singleton/singleton.h"
#include "src/common/game/GameManager.h"

#include "vendor/ImGui/imgui.h"
int hShadow = -1;

bool exitFlag = false;

void AppInit()
{
	SceneManager::Start();
	exitFlag = false;

	// シャドウマップを生成する
	hShadow = MakeShadowMap(4096, 4096);

	// 影を出すライトを決める(真下に向ける)
	SetShadowMapLightDirection(hShadow, VGet(0.5, -1.0f, 0.0f));
}

void AppUpdate()
{
	if (InputManager::Push(KeyDefine::KeyCode::Escape))
		exitFlag = true;

	GTime.Update();
	SceneManager::Update();

	Random.SetSeed(Random.GetInt());
}

void AppDraw()
{
	VECTOR camPos = GetCameraPosition();
	//MATRIX camM = MGetRotY(GetCameraAPIViewportMatrix());
	VECTOR offset = VTransform(VGet(0, 0, 500), GetCameraAPIViewportMatrix());

	// 影計算をするエリア範囲
	SetShadowMapDrawArea(hShadow, camPos - VGet(500, 500, 500), camPos + offset);

	ShadowMap_DrawSetup(hShadow);
	SceneManager::Draw();
	ShadowMap_DrawEnd();

	SetUseShadowMap(0, hShadow);
	SceneManager::Draw();
	SetUseShadowMap(0, -1);
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