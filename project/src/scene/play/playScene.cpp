#include "playScene.h"
#include "framework/SceneManager.h"

#include "src/util/input/InputManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/component/collider/CollisionManager.h"

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
	Instantiate<CollisionManager>();

	Instantiate<MatchManager>();

	// �u���[��
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
	// ToDo:���C���[�Ǘ�
	m_BloomManager->SetDrawScreenToEmitter();
	EffectManager::Draw();
	m_BloomManager->SetDrawScreenToBack();

	SceneBase::Draw();

	DrawSphere3D(Vector3(0, 150, 1000), 50, 32, 0xffffff, 0x001fff, true);

	Settings_json::Inst()->RenderImGuiFileManager();
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
