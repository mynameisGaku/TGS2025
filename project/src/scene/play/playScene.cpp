#include "playScene.h"
#include "framework/SceneManager.h"
#include "src/util/Utils.h"

#include "src/util/input/InputManager.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/component/controller/PlayerController.h"
#include "src/common/component/controller/AIController.h"
#include "src/common/component/controller/DebugController.h"
#include "src/common/component/collider/CollisionManager.h"

//=== ボール ===
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"

//=== ポストエフェクト ===
#include "src/util/fx/post_effect/bloom/BloomManager.h"
#include "src/util/fx/effect/EffectManager.h"

//=== 破片 ===
#include "src/scene/play/crystal/CrystalFragmentManager.h"
#include "src/util/file/json/settings_json.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	Instantiate<CollisionManager>();

	CharaManager* charaM = Instantiate<CharaManager>();

	CharaBase* player = charaM->Create("Red", Transform(Vector3(0.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones));
	CharaBase* enemy = charaM->Create("Blue", Transform(Vector3(150.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones));

	player->SetMoveSpeed(700.0f);
	player->SetRotSpeed(Math::DegToRad(10.0f));
	player->AddComponent<PlayerController>()->Init(DX_INPUT_PAD1);

	enemy->SetMoveSpeed(700.0f);
	enemy->SetRotSpeed(Math::DegToRad(10.0f));

	// デバッグによってコントローラーを変える。
#if FALSE
	enemy->AddComponent<AIController>()->Init();
#elif FALSE
	enemy->AddComponent<DebugController>()->Init(DX_INPUT_PAD1);
#elif TRUE
	enemy->AddComponent<PlayerController>()->Init(DX_INPUT_PAD2);
#endif

	BallManager* ballM = Instantiate<BallManager>();

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
	// ToDo:レイヤー管理
	m_BloomManager->SetDrawScreenToEmitter();
	EffectManager::Draw();
	m_BloomManager->SetDrawScreenToBack();

	SceneBase::Draw();

	DrawSphere3D(Vector3(0, 150, 1000), 50, 32, 0xffffff, 0x001fff, true);

	Settings_json::Inst()->RenderImGuiFileManager();
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
