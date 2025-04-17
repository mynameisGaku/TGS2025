#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

#include "InputManager.h"
#include "CharaManager.h"

#include "CrystalFragmentManager.h"
#include "CrystalFragmentSpawner.h"

//=== ボール ===
#include "BallManager.h"
#include "Ball.h"

// using
using namespace KeyDefine;

static CrystalFragmentManager* fragmentM = nullptr;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	CharaManager* charaM = Instantiate<CharaManager>();
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(0.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));

	BallManager* ballM = Instantiate<BallManager>();
	Ball* ball = ballM->CreateBall(Vector3(0.0f, 500.0f, -50.0f));
	ball->Throw(Vector3(20.0f, 50.0f, 5.0f));

    // クリスタルの破片を生成
    fragmentM = Instantiate<CrystalFragmentManager>();
    CrystalFragmentSpawner* spawner = Instantiate<CrystalFragmentSpawner>();
    spawner->transform->position = Vector3(0.0f, 0.0f, 500.0f);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (InputManager::Push(KeyCode::T)) {
		SceneManager::ChangeScene("TitleScene");
	}

    if (InputManager::Push(KeyCode::N)) 
	{
		fragmentM->CreateFragment(Vector3(0.0f, 0.0f, 500.0f));
		fragmentM->CreateFragment(Vector3(0.0f, 0.0f, 500.0f));
		fragmentM->CreateFragment(Vector3(0.0f, 0.0f, 500.0f));
		fragmentM->CreateFragment(Vector3(0.0f, 0.0f, 500.0f));
		fragmentM->CreateFragment(Vector3(0.0f, 0.0f, 500.0f));
		fragmentM->CreateFragment(Vector3(0.0f, 0.0f, 500.0f));
    }

	SceneBase::Update();
}


void PlayScene::Draw()
{
	SceneBase::Draw();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
