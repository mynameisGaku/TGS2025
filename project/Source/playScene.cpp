#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"
#include "Util/Utils.h"

#include "InputManager.h"
#include "CharaManager.h"
#include "CameraManager.h"
#include "Component/PlayerController.h"
#include "Component/AIController.h"
#include "Component/DebugController.h"
#include "Component/CollisionManager.h"

//=== ボール ===
#include "BallManager.h"
#include "Ball.h"

//=== ポストエフェクト ===
#include "BloomManager.h"
#include "EffectManager.h"

//=== 破片 ===
#include "CrystalFragmentManager.h"
#include "settings_json.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	Instantiate<CollisionManager>();

	CharaManager* charaM = Instantiate<CharaManager>();

	CharaBase* player = charaM->Create("Red", Transform(Vector3(0.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));
	CharaBase* enemy = charaM->Create("Blue", Transform(Vector3(150.0f, 0.0f, 0.0f), V3::ZERO, V3::ONE));

	player->SetMoveSpeed(600.0f);
	player->SetRotSpeed(Math::DegToRad(10.0f));
	player->AddComponent<PlayerController>()->Init(DX_INPUT_PAD1);

	enemy->SetMoveSpeed(600.0f);
	enemy->SetRotSpeed(Math::DegToRad(10.0f));
	//enemy->AddComponent<AIController>()->Init();
	enemy->AddComponent<DebugController>()->Init(DX_INPUT_PAD1);

	BallManager* ballM = Instantiate<BallManager>();
	Ball* ball = ballM->CreateBall(Vector3(0.0f, 500.0f, -50.0f));
	ball->Throw(Vector3(20.0f, 50.0f, 5.0f));

	fragM = Instantiate<CrystalFragmentManager>();
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eFIRE, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eEARTH, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eICE, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eLIGHTNING, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eWIND, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eNONE, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eFIRE, CrystalSize::sSMALL);
	fragM->CreateFragment(Vector3(0, 0, -150), CrystalElement::eFIRE, CrystalSize::sSMALL);

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
	if (InputManager::Hold(KeyCode::N)) {
		int r = GetRand(3);
		int r2 = GetRand(CrystalElement::eMAX);
		fragM->CreateFragment(Vector3(0, 0, -150), (CrystalElement)r2, (CrystalSize)r);
	}

	SceneBase::Update();
}


void PlayScene::Draw()
{
	// ToDo:レイヤー管理
	m_BloomManager->SetDrawScreenToEmitter();
	EffectManager::Draw();
	m_BloomManager->SetDrawScreenToBack();

	SceneBase::Draw();

	DrawSphere3D(Vector3(0, 150, 1000), 50, 32, 0xffffff, 0x001fff, true);

	Settings_json::Inst()->RenderImGuiFileManager();
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
