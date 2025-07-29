#include "Enemy.h"
#include "src/common/component/animator/Animator.h"
#include "src/util/input/InputManager.h"
#include "src/scene/play/ball/BallTarget.h"
#include "src/scene/play/ball/BallTargetManager.h"

namespace
{
	static const Vector3 TARGET_OFFSET = Vector3::SetY(60);	// ターゲットのオフセット
}

Enemy::Enemy()
{
	m_Animator = nullptr;
	m_pBallTarget = nullptr;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	m_Animator = AddComponent<Animator>();
	m_Animator->Init("root", 30.0f, 1.0f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_pBallTarget = FindGameObject<BallTargetManager>()->Create();
	m_pBallTarget->SetCharaTag("Enemy");

	m_Animator->LoadAnimsFromJson("data/Json/Enemy/BatAnim.json");
	m_Animator->Play("Flap");
}

void Enemy::Update()
{
	Object3D::Update();

	if (InputManager::Push("Reload"))
	{
		m_Animator->Play("Bite");
		m_Animator->DeleteAnimInfos();
		m_Animator->LoadAnimsFromJson("data/Json/Enemy/BatAnim.json");
		m_Animator->Play("Flap");
	}

	m_pBallTarget->SetPositionWithParent(TARGET_OFFSET, transform);
}

void Enemy::Draw()
{
	Object3D::Draw();
}
