#include "playScene.h"
#include "framework/SceneManager.h"

#include "src/util/input/InputManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/component/collider/CollisionManager.h"

//=== �G�t�F�N�g ===

//=== �|�X�g�G�t�F�N�g ===
#include "src/util/fx/post_effect/bloom/BloomManager.h"
#include "src/util/fx/effect/EffectManager.h"

//=== �j�� ===
#include "src/scene/play/crystal/CrystalFragmentManager.h"
#include "src/util/file/json/settings_json.h"

//=== �i�s�� ===
#include "src/scene/play/match/MatchManager.h"

//=== �{�[�� ===
#include "src/scene/play/ball/BallSpawner.h"

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	auto gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();
	// gameM->SetGameModeName("FreeForAll");
	// �Q�[�����[�h�� GameRef.json �����Q�Ƃ��Ă�������
	gameM->SetGameModeName("Debug");
	
	Instantiate<CollisionManager>();

	Instantiate<MatchManager>();

	BallSpawnerPlaceByJson("data/Json/Ball/BallSpawner.json", "BallSpawner");

	// �u���[��
	m_BloomManager = Instantiate<BloomManager>();
	SetDrawOrder(m_BloomManager, 10000);

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
	// ToDo:���C���[�Ǘ�
	m_BloomManager->SetDrawScreenToEmitter();
	EffectManager::Draw();
	m_BloomManager->SetDrawScreenToBack();

	if (CameraManager::IsScreenDivision())
		CameraManager::ApplyScreenDivision();
	
	EffectManager::Draw();

	SceneBase::Draw();

	if (not CameraManager::IsScreenDivision())
		Settings_json::Inst()->RenderImGuiFileManager();

	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
