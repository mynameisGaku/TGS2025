#include "titleScene.h"

// ���ėp
#include "framework/SceneManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"


// �����o�E�@�\
#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"

#include "src/common/setting/SettingManager.h"
#include "src/util/easing/easing.h"
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>

TitleScene::TitleScene(std::string name) : SceneBase(true, name) {

	Fader::FadeIn(1.0f, EasingType::Linear);

	SettingManager* settingManager = Instantiate<SettingManager>();
}

TitleScene::~TitleScene() {

}

void TitleScene::Update() {

	switch (sceneState) {
	case SceneState::BeforePlay:BeforePlayUpdate();	break;
	case SceneState::InPlay:	InPlayUpdate();		break;
	case SceneState::AfterPlay:	AfterPlayUpdate();	break;
	}

	if (InputManager::Push("AnyKey"))
	{
		auto& net = NetworkRef::Inst();
		if (net.IsNetworkEnable)
		{
			NetworkManager* nm = SceneManager::CommonScene()->FindGameObject<NetworkManager>();
			nm->SendSceneTransitToPlay();
		}
		
		SceneManager::ChangeScene("PlayScene");
	}

	SceneBase::Update();
}

void TitleScene::Draw() {

	SceneBase::Draw();

#ifdef TRUE //DEBUG
	DrawString(100, 400, "Push [Z]Key To Play", GetColor(255, 255, 255));
#endif
}

void TitleScene::BeforePlayUpdate()
{
}

void TitleScene::InPlayUpdate()
{
}

void TitleScene::AfterPlayUpdate()
{
}
