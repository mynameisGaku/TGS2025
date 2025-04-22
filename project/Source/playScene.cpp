#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"
#include "Util/Utils.h"

#include "InputManager.h"
#include "CharaManager.h"
#include "CameraManager.h"
#include "Component/PlayerController.h"
#include "Component/AIController.h"
#include "Component/CollisionManager.h"

//=== ボール ===
#include "BallManager.h"
#include "Ball.h"

//=== ポストエフェクト ===
#include "BloomManager.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	Instantiate<CollisionManager>();

	CharaManager* charaM = Instantiate<CharaManager>();
	CharaBase* player = charaM->Create(CharaDefine::CharaTag::tPlayer, Transform(Vector3(0.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	CharaBase* enemy = charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));

	player->SetMoveSpeed(500.0f);
	player->SetRotSpeed(Math::DegToRad(10.0f));
	player->AddComponent<PlayerController>()->Init(DX_INPUT_PAD1);

	enemy->SetMoveSpeed(200.0f);
	enemy->SetRotSpeed(Math::DegToRad(10.0f));
	enemy->AddComponent<AIController>()->Init();

	BallManager* ballM = Instantiate<BallManager>();
	Ball* ball = ballM->CreateBall(Vector3(0.0f, 500.0f, -50.0f));
	ball->Throw(Vector3(20.0f, 50.0f, 5.0f));

	// ブルーム
	m_BloomManager = Instantiate<BloomManager>();
	SetDrawOrder(m_BloomManager, 10000);

	CameraManager::MainCamera()->ChangeState(&Camera::ChaseState);
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
