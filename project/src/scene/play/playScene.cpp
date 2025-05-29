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

using namespace KeyDefine;

PlayScene::PlayScene(std::string name) : SceneBase(true, name)
{
	auto gameM = SceneManager::CommonScene()->FindGameObject<GameManager>();
	// gameM->SetGameModeName("FreeForAll");
	// �Q�[�����[�h�� GameRef.json �����Q�Ƃ��Ă�������
	gameM->SetGameModeName("Debug");
	
	Instantiate<CollisionManager>();

	Instantiate<MatchManager>();

	// �u���[��
	m_BloomManager = Instantiate<BloomManager>();
	SetDrawOrder(m_BloomManager, 10000);

	CameraManager::SetIsScreenDivision(true);
	CameraManager::MainCamera()->ChangeState(&Camera::ChaseState);
	CameraManager::GetCamera(1)->ChangeState(&Camera::ChaseState);
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
	
	DrawSphere3D(Vector3(0, 150, 1000), 50, 32, 0xffffff, 0x001fff, true);

	SceneBase::Draw();

	if (not CameraManager::IsScreenDivision())
		Settings_json::Inst()->RenderImGuiFileManager();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
