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
#include "src/scene/play/ball/BallTargetManager.h"

//=== ステージ ===
#include "src/common/stage/StageObjectManager.h"

//=== 力場 ===
#include "src/scene/play/force_field/ForceFieldManager.h"
#include "src/scene/play/force_field/ForceFieldSphere.h"
#include "src/scene/play/force_field/ForceFieldCorn.h"
#include "src/scene/play/force_field/ConstantPointForce.h"

//=== UI ===
#include "src/scene/play/ui/UI_Setter_PlayScene.h"
#include "src/util/debug/imgui/imGuiManager.h"
#include <src/reference/network/NetworkRef.h>
#include "src/scene/play/enemy/EnemyManager.h"

//=== サウンド ===
#include "src/util/sound/SoundManager.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	CameraManager::SetIsScreenDivision(true);

    auto& net = NetworkRef::Inst();
	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	int w, h;
	GetWindowSize(&w, &h);

	for (int i = 0; i < CAMERA_NUM; i++) {
		Camera* camera = CameraManager::GetCamera(i);
		if (camera == nullptr)
			continue;

		camera->SetDefinedDrawArea((w / CAMERA_NUM) * i, 0, w / CAMERA_NUM, h);
		camera->ApplyDefinedDrawArea();
		camera->ChangeState(&Camera::ChaseState);
	}

#ifdef _DEBUG
#ifdef IMGUI
	ImGuiManager::AddNode(new ImGuiNode_Button("DebugCamera",
		[CAMERA_NUM]() {
			for (int i = 0; i < CAMERA_NUM; i++)
				CameraManager::GetCamera(i)->ChangeState(&Camera::DebugState);
		}));
	ImGuiManager::AddNode(new ImGuiNode_Button("ChaseCamera",
		[CAMERA_NUM]() {
			for (int i = 0; i < CAMERA_NUM; i++)
				CameraManager::GetCamera(i)->ChangeState(&Camera::ChaseState);
		}));
#endif
#endif

	auto gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();

#ifdef _DEBUG
	gameM->SetGameModeName("1v1");
#else
	gameM->SetGameModeName("FreeForAll");
#endif // _DEBUG

	// ゲームモードは GameRef.json 内を参照してください
	//gameM->SetGameModeName("Debug");

	Instantiate<CollisionManager>();

	// オフラインプレイのときはいいけど、オンラインのときにカメラ生成がキャラと同時に行われるので
	// ここでの生成はオンラインのときは無意味。あとで直す

	if (not net.IsNetworkEnable)
		Instantiate<UI_Setter_PlayScene>();

	BallTargetManager* ballTargetManager = Instantiate<BallTargetManager>();

	Instantiate<MatchManager>();

	EnemyManager* enemyManager = Instantiate<EnemyManager>();
	//enemyManager->Create();

	TargetManager* targetManager = Instantiate<TargetManager>();
	SetDrawOrder(targetManager, 1000);

	ForceFieldManager* forceFieldManager = Instantiate<ForceFieldManager>();

	// ブルーム
	m_BloomManager = Instantiate<BloomManager>();
	SetDrawOrder(m_BloomManager, 10000);

	//StageObjectManager::LoadFromJson("data/json/Stage/Stage_4.json");
	StageObjectManager::LoadFromJson("data/json/Stage/" + gameM->GetCurrentStageName() + ".json");

    if (net.IsNetworkEnable)
        CameraManager::SetIsScreenDivision(false);

	SoundManager::FadeIn("BGM_PlayScene.mp3", "BGM", 1.0f, EasingType::Linear);
}

PlayScene::~PlayScene()
{
	CameraManager::SetIsScreenDivision(false);

	SoundManager::Stop("BGM_PlayScene.mp3", "BGM");
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

#ifdef _DEBUG
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
	//DrawString(100, 500, "Push [R]Key To Result", GetColor(255, 255, 255));
#endif
}
