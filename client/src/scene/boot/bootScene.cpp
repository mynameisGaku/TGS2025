#include "src/scene/boot/bootScene.h"
#include "framework/sceneManager.h"

#include "src/common/system/SystemManager.h"
#include "src/common/load_screen/LoadScreen.h"
#include "src/common/game/GameManager.h"
#include "src/common/network/NetworkManager.h"

BootScene::BootScene(std::string name) : SceneBase(true, name)
{
	SceneBase* common = SceneManager::CommonScene();
	
	systemManager = common->Instantiate<SystemManager>();

	loadScreen = common->Instantiate<LoadScreen>();
	common->SetDrawOrder(loadScreen, 10000);

	gameM = common->Instantiate<GameManager>();

	NetworkManager* netM = common->Instantiate<NetworkManager>();
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	systemManager->LoadUpdate();

	if (systemManager->IsLoadEnd())
	{
		SceneManager::ChangeScene("TitleScene"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
	}
}

void BootScene::Draw()
{
}
