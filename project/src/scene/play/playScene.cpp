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

//=== 発光テスト用 ===
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/chara/CharaManager.h"

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

	//StageObjectManager::LoadFromJson("data/json/Stage/Stage_4.json");
	StageObjectManager::LoadFromJson("data/json/Stage/" + gameM->GetCurrentStageName() + ".json");

    if (net.IsNetworkEnable)
        CameraManager::SetIsScreenDivision(false);

	SoundManager::FadeIn("BGM_PlayScene.wav", "BGM", 1.0f, EasingType::Linear);
}

PlayScene::~PlayScene()
{
	CameraManager::SetIsScreenDivision(false);

	SoundManager::Stop("BGM_PlayScene.wav", "BGM");
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

	if (not BLOOM_MANAGER.IsUsingEmitterScreen())
	{
		BLOOM_MANAGER.SetDrawScreenToEmitter();
		{
			std::list<Ball*> ballList = FindGameObjects<Ball>();
			for (Ball* ball : ballList)
			{
				ball->Draw();
			}
			CharaManager* charaM = FindGameObject<CharaManager>();
			if (charaM != nullptr)
				charaM->Draw();
		}
		BLOOM_MANAGER.SetDrawScreenToLastScreen();
	}

	//if (CameraManager::IsScreenDivision())
	//	CameraManager::ApplyScreenDivision();


	//if (not CameraManager::IsScreenDivision())
	//	Settings_json::Inst()->RenderImGuiFileManager();

#ifdef _DEBUG
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
	//DrawString(100, 500, "Push [R]Key To Result", GetColor(255, 255, 255));
#endif
}
