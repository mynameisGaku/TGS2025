#include "framework/App.h"
#include "framework/sceneManager.h"
#include "src/util/time/GameTime.h"
#include "src/util/input/InputManager.h"
#include "src/util/math/Random.h"
#include "src/util/singleton/singleton.h"

bool exitFlag = false;

void AppInit()
{
	SceneManager::Start();
	exitFlag = false;
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
	SceneManager::Draw();
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