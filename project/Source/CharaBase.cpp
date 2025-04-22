#include "CharaBase.h"
#include "InputManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Component/ColliderCapsule.h"
#include "Stage.h"
#include "CharaStamina.h"
#include "Ball.h"

using namespace KeyDefine;

CharaBase::CharaBase()
{
	m_pStamina			= Instantiate<CharaStamina>();
	m_pBall				= nullptr;
	m_pPhysics			= nullptr;
	m_BallChargeRate	= 0.0f;
	m_IsCharging		= false;
	m_MoveSpeed			= 0.0f;
	m_RotSpeed			= 0.0f;
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());
}

void CharaBase::LoadAddedComponent()
{
	m_pPhysics = GetComponent<Physics>();
}

void CharaBase::Update() {

	HitGroundProcess();

	Object3D::Update();
}

void CharaBase::CollisionEvent(const CollisionData& colData) {

	// �����������肪Player��������Enemy�̏ꍇ
	if (colData.Other()->Tag() == ColDefine::Tag::tPlayer || colData.Other()->Tag() == ColDefine::Tag::tEnemy) {

		// ����̏��
		CharaBase* chara = colData.Other()->Parent<CharaBase>();

		if (chara != nullptr) {
			Vector3 myPos = transform->Global().position;			// ���g�̍��W
			Vector3 otherPos = chara->transform->Global().position;	// ����̍��W

			Physics* otherPhysics = chara->GetComponent<Physics>();	// ����̕�������

			ColliderCapsule* collider = GetComponent<ColliderCapsule>();	// �����蔻��

			if (m_pPhysics == nullptr || otherPhysics == nullptr || collider == nullptr)
				return;

			// ����֌������x�N�g��
			Vector3 toVec = otherPos - myPos;
			if (toVec.SquareSize() == 0)
				toVec = Vector3(0, 0, 1);

			// �����͂̋������`
			constexpr float REPELLENT_FORCE_SCALE_MAX = 2.0f;
			constexpr float REPELLENT_FORCE_SCALE_MIN = 1.0f;
			const float REPELLENT_RADIUS = collider->Radius() * 4.0f;

			// ������
			const Vector3 repellentForce = toVec.Norm() * (REPELLENT_FORCE_SCALE_MIN + (REPELLENT_FORCE_SCALE_MAX - REPELLENT_FORCE_SCALE_MIN) * max(0.0f, 1.0f - toVec.Size() / REPELLENT_RADIUS));

			// �����͂�������
			m_pPhysics->resistance -= repellentForce;
			otherPhysics->velocity += repellentForce;
		}
	}
}

void CharaBase::HitGroundProcess() {

	// ��������
	Physics* physics = GetComponent<Physics>();
	if (physics == nullptr)
		return;

	Vector3 colliderGlobalPos1 = V3::ZERO;
	Vector3 colliderGlobalPos2 = transform->Global().position + V3::SetY(-0.5f);
	Vector3 hitPos;	// �ڒn�_

	// �����蔻��
	ColliderCapsule* capsuleCol = GetComponent<ColliderCapsule>();

	if (capsuleCol == nullptr)
		colliderGlobalPos1 = physics->LastTransform()->Global().position;
	else
		colliderGlobalPos1 = capsuleCol->OffsetWorld();

	// �n�_�ƏI�_�̍��W��n���āA�n�`�Ɠ������Ă��邩�𔻒�
	if (Stage::ColCheckGround(colliderGlobalPos1, colliderGlobalPos2, &hitPos)) {
		transform->position.y = hitPos.y;
		physics->velocity.y = 0.0f;
		physics->resistance.y = 0.0f;
		physics->SetGravity(V3::ZERO);
	}

	if (m_pBall)
	{
		m_ChargeRateWatchDog -= Time::DeltaTime();
		if (m_ChargeRateWatchDog < 0.0f)
		{
			m_ChargeRateWatchDog = 0.0f;
			m_IsCharging = false;
		}

		m_pBall->transform->position = transform->Global().position;
		m_pBall->transform->rotation = transform->Global().rotation;
	}
}

void CharaBase::Move(const Vector3& dir)
{
	float currentRot = transform->rotation.y;	// ���݂̌���
	float terminusRot = atan2f(dir.x, dir.z);		// �I�_�̌���

	// ���X�ɏI�_�̌����֍��킹��
	transform->rotation.y = Function::RotAngle(currentRot, terminusRot, m_RotSpeed);

	float deltaTimeMoveSpeed = m_MoveSpeed * Time::DeltaTimeLapseRate();	// ���Ԍo�ߗ���K�������ړ����x

	Vector3 velocity = dir * deltaTimeMoveSpeed * V3::HORIZONTAL;	// �X�e�B�b�N�̌X���̕����ւ̑��x

	// ���x��K��������
	m_pPhysics->velocity.x = velocity.x;
	m_pPhysics->velocity.z = velocity.z;
}

void CharaBase::ThrowBall(const Vector3& velocity)
{
	if (m_pBall == nullptr)
		return;

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	// ������͊Ǘ�����`��������
	m_pBall = nullptr;
}

void CharaBase::ThrowBallForward()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + V3::SetY(0.4f);

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	// ������͊Ǘ�����`��������
	m_pBall = nullptr;
}

void CharaBase::GenerateBall()
{
	m_ChargeRateWatchDog = 0.1f;

	if (m_pBall != nullptr)
	{
		m_IsCharging = true;
		m_BallChargeRate += Time::DeltaTime();
		return;
	}

	m_pBall = Instantiate<Ball>();
	m_pBall->transform->position = transform->Global().position;
	m_pBall->transform->rotation = transform->Global().rotation;
	m_pBall->SetParent(this);
}
