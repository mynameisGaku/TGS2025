#include "playScene.h"
#include "framework/SceneManager.h"

#include "src/util/input/InputManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/component/collider/CollisionManager.h"

//=== エフェクト ===
#include "src/scene/play/targetting/TargetManager.h"

//=== ポストエフェクト ===
#include "src/util/fx/post_effect/bloom/BloomManager.h"
#include "src/util/fx/effect/EffectManager.h"

//=== 破片 ===
#include "src/scene/play/crystal/CrystalFragmentManager.h"
#include "src/util/file/json/settings_json.h"

//=== 進行役 ===
#include "src/scene/play/match/MatchManager.h"

//=== ボール ===
#include "src/scene/play/ball/BallSpawner.h"

//=== ステージ ===
#include "src/common/stage/StageObjectManager.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	auto gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();
	// gameM->SetGameModeName("FreeForAll");
	// ゲームモードは GameRef.json 内を参照してください
	gameM->SetGameModeName("Debug");
	
	Instantiate<CollisionManager>();

	Instantiate<MatchManager>();

	TargetManager* targetManager = Instantiate<TargetManager>();
	SetDrawOrder(targetManager, 1000);

	// ブルーム
	m_BloomManager = Instantiate<BloomManager>();
	SetDrawOrder(m_BloomManager, 10000);

	StageObjectManager::LoadFromJson("data/json/Stage/Stage_4.json");

	CameraManager::SetIsScreenDivision(true);

	const int camNum = (int)CameraManager::AllCameras().size();

	for (int i = 0; i < camNum; i++) {
		CameraManager::GetCamera(i)->ChangeState(&Camera::ChaseState);
	}
}

PlayScene::~PlayScene()
{
	CameraManager::SetIsScreenDivision(false);
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
	// ToDo:レイヤー管理
	m_BloomManager->SetDrawScreenToEmitter();
	EffectManager::Draw();
	m_BloomManager->SetDrawScreenToBack();

	if (CameraManager::IsScreenDivision())
		CameraManager::ApplyScreenDivision();
	
	EffectManager::Draw();

	SceneBase::Draw();

	if (not CameraManager::IsScreenDivision())
		Settings_json::Inst()->RenderImGuiFileManager();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
