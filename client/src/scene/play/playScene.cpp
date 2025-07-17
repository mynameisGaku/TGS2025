#include "playScene.h"
#include "framework/SceneManager.h"

#include "src/util/input/InputManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/component/collider/CollisionManager.h"
#include "src/util/math/MathUtil.h"

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

//=== 力場 ===
#include "src/scene/play/force_field/ForceFieldManager.h"
#include "src/scene/play/force_field/ForceFieldSphere.h"
#include "src/scene/play/force_field/ForceFieldCorn.h"
#include "src/scene/play/force_field/ConstantPointForce.h"

#include "src/scene/play/ui/UI_Setter_PlayScene.h"
#include <src/reference/network/NetworkRef.h>

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	CameraManager::SetIsScreenDivision(true);

	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();
	for (int i = 0; i < CAMERA_NUM; i++)
		CameraManager::GetCamera(i)->ChangeState(&Camera::ChaseState);

	auto gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();
	gameM->SetGameModeName("FreeForAll");
	// ゲームモードは GameRef.json 内を参照してください
	//gameM->SetGameModeName("Debug");

	Instantiate<CollisionManager>();

	Instantiate<UI_Setter_PlayScene>();
	Instantiate<MatchManager>();

	TargetManager* targetManager = Instantiate<TargetManager>();
	SetDrawOrder(targetManager, 1000);

	ForceFieldManager* forceFieldManager = Instantiate<ForceFieldManager>();
	ForceFieldCorn* forceField = forceFieldManager->CreateForceFieldCorn(Transform(Vector3(0, 500, 0), Vector3::Zero, Vector3::Ones), 1000.0f, MathUtil::ToRadians(30.0f));
	forceField->SetColTag(ColDefine::Tag::tWindArea);
	forceField->SetColTargetTags({ ColDefine::Tag::tBall, ColDefine::Tag::tChara });

	std::unique_ptr<ConstantPointForce> force = std::make_unique<ConstantPointForce>(-500.0f);
	forceField->SetForce(std::move(force));

	// ブルーム
	m_BloomManager = Instantiate<BloomManager>();
	SetDrawOrder(m_BloomManager, 10000);

	//StageObjectManager::LoadFromJson("data/json/Stage/Stage_4.json");
	StageObjectManager::LoadFromJson("data/json/Stage/" + gameM->GetCurrentStageName() + ".json");

    auto& net = NetworkRef::Inst();
    if (net.IsNetworkEnable)
        CameraManager::SetIsScreenDivision(false);
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
	//m_BloomManager->SetDrawScreenToEmitter();
	EffectManager::Draw();
	//m_BloomManager->SetDrawScreenToBack();

	//if (CameraManager::IsScreenDivision())
	//	CameraManager::ApplyScreenDivision();

	SceneBase::Draw();

	//if (not CameraManager::IsScreenDivision())
	//	Settings_json::Inst()->RenderImGuiFileManager();

#ifdef DEBUG
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
	DrawString(100, 500, "Push [R]Key To Result", GetColor(255, 255, 255));
#endif
}
