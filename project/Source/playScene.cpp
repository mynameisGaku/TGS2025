#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"
#include "Util/Utils.h"

#include "InputManager.h"
#include "CharaManager.h"
#include "Component/PlayerController.h"

//=== É{Å[Éã ===
#include "BallManager.h"
#include "Ball.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	CharaManager* charaM = Instantiate<CharaManager>();
	CharaBase* player = charaM->Create(CharaDefine::CharaTag::tPlayer, Transform(Vector3(0.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	charaM->Create(CharaDefine::CharaTag::tEnemy, Transform(Vector3(-300.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));

	player->AddComponent<PlayerController>()->Init(DX_INPUT_PAD1, 500.0f, Math::DegToRad(10.0f));

	BallManager* ballM = Instantiate<BallManager>();
	Ball* ball = ballM->CreateBall(Vector3(0.0f, 500.0f, -50.0f));
	ball->Throw(Vector3(20.0f, 50.0f, 5.0f));
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
