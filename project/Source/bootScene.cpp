#include "bootScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

#include "SystemManager.h"
#include "LoadScreen.h"

BootScene::BootScene(std::string name) : SceneBase(true, name)
{
	SceneBase* common = SceneManager::CommonScene();
	DebugScreen* ds = common->Instantiate<DebugScreen>();
	common->SetDrawOrder(ds, 10000);

	systemManager = common->Instantiate<SystemManager>();

	LoadScreen* loadScreen = common->Instantiate<LoadScreen>();
	common->SetDrawOrder(loadScreen, 10000);
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	systemManager->LoadUpdate();

	if (systemManager->IsLoadEnd())
		SceneManager::ChangeScene("TitleScene"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
}

void BootScene::Draw()
{
}
