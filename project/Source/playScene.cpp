#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

#include "CameraManager.h"

#include "Component/CollisionManager.h"
#include "Component/PlayerController.h"

#include "InputManager.h"
#include "CharaManager.h"
#include "BallManager.h"

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	Instantiate<CollisionManager>();

	CharaManager* charaM = Instantiate<CharaManager>();
	CharaBase* player00 = charaM->Create(CharaDefine::CharaTag::tPlayer, Transform(Vector3(0.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));

	player00->AddComponent<PlayerController>()->Init(DX_INPUT_PAD1, 100.0f, Math::DegToRad(10.0f));

	BallManager* ballM = Instantiate<BallManager>();
	ballM->CreateBall(Vector3(0.0f, 0.0f, -50.0f));

	Camera* mainCamera = CameraManager::MainCamera();

	mainCamera->SetHolderCharaIndex(0);
	mainCamera->ChangeState(&Camera::ChaseState);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (InputManager::Push(KeyDefine::KeyCode::T)) {
		SceneManager::ChangeScene("TitleScene");
	}

	SceneBase::Update();
}


void PlayScene::Draw()
{
	SceneBase::Draw();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
