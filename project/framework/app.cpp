#include "framework/App.h"
#include "framework/sceneManager.h"
#include "src/util/time/GameTime.h"

bool exitFlag = false;

void AppInit()
{
	SceneManager::Start();
	exitFlag = false;
}

void AppUpdate()
{
	GTime.Update();
	SceneManager::Update();
}

void AppDraw()
{
	SceneManager::Draw();
}

void AppRelease()
{
	SceneManager::Release();
}

void Exit()
{
	exitFlag = true;
}

bool IsExit()
{
	return exitFlag;
}