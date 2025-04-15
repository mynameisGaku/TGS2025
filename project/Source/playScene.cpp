#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

#include "InputManager.h"
#include "CharaManager.h"
#include "BallManager.h"

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	CharaManager* charaM = Instantiate<CharaManager>();
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(0.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));

	BallManager* ballM = Instantiate<BallManager>();
	ballM->CreateBall(Vector3(0.0f, 0.0f, -50.0f));
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (InputManager::Push(KeyCode::T)) {
		SceneManager::ChangeScene("TitleScene");
	}

	SceneBase::Update();
}


void PlayScene::Draw()
{
	SceneBase::Draw();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
