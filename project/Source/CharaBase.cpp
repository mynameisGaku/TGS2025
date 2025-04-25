#include "CharaBase.h"
#include "InputManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Stage.h"
#include "CharaStamina.h"
#include "Ball.h"
#include "Catcher.h"
#include "CharaDefineRef.h"
#include "Component/Animator.h"

using namespace KeyDefine;

namespace
{
	static const float CATCH_STAMINA_USE = 50.0f;	// �L���b�`�Ɏg���X�^�~�i�i���b�j
	static const float CATCH_STAMINA_MIN = 20.0f;	// �L���b�`���J�n����̂ɕK�v�Ȏc�X�^�~�i
	static const float CATCH_TIME = 0.05f;	// ���͈��̃L���b�`�p������
	static const float SLIDE_TIME = 0.05f;	// ���͈��̃X���C�f�B���O�p������
}

CharaBase::CharaBase()
{
	m_pStamina				= Instantiate<CharaStamina>();
	m_pBall					= nullptr;
	m_pLastBall				= nullptr;
	m_pPhysics				= nullptr;
	m_BallChargeRate		= 0.0f;
	m_IsCharging			= false;
	m_MoveSpeed				= 0.0f;
	m_RotSpeed				= 0.0f;
	m_ChargeRateWatchDog	= 0.0f;
	m_CatchTimer			= 0.0f;
	m_CharaTag				= CHARADEFINE_REF.Tags[0];
	m_Catcher				= nullptr;
	m_Index					= 0;
	m_Animator				= nullptr;
	m_EmoteTimer			= 0.0f;
	m_IsLanding				= true;
	m_SlideTimer			= 0.0f;

	m_FSM = new TinyFSM<CharaBase>(this);

	// ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_FSM->RegisterStateName(&CharaBase::StateActionIdle, "StateActionIdle");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleEmote, "StateActionIdleEmote");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleToJump, "StateActionIdleToJump");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleToRun, "StateActionIdleToRun");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleToStandingIdle, "StateActionIdleToStandingIdle");

	m_FSM->RegisterStateName(&CharaBase::StateAirSpin, "StateAirSpin");

	m_FSM->RegisterStateName(&CharaBase::StateCrouchToActionIdle, "StateCrouchToActionIdle");
	m_FSM->RegisterStateName(&CharaBase::StateCrouchToRun, "StateCrouchToRun");

	m_FSM->RegisterStateName(&CharaBase::StateDamageToDown, "StateDamageToDown");

	m_FSM->RegisterStateName(&CharaBase::StateFall, "StateFall");
	m_FSM->RegisterStateName(&CharaBase::StateFallToCrouch, "StateFallToCrouch");
	m_FSM->RegisterStateName(&CharaBase::StateFallToRollToIdle, "StateFallToRollToIdle");

	m_FSM->RegisterStateName(&CharaBase::StateRun, "StateRun");
	m_FSM->RegisterStateName(&CharaBase::StateRunToActionIdle, "StateRunToActionIdle");
	m_FSM->RegisterStateName(&CharaBase::StateRunToJump, "StateRunToJump");
	m_FSM->RegisterStateName(&CharaBase::StateRunToSlide, "StateRunToSlide");

	m_FSM->RegisterStateName(&CharaBase::StateSlide, "StateSlide");
	m_FSM->RegisterStateName(&CharaBase::StateSlideToRun, "StateSlideToRun");

	m_FSM->RegisterStateName(&CharaBase::StateStandingIdle, "StateStandingIdle");
	m_FSM->RegisterStateName(&CharaBase::StateStandingIdleEmote, "StateStandingIdleEmote");
	m_FSM->RegisterStateName(&CharaBase::StateStandingIdleToActionIdle, "StateStandingIdleToActionIdle");

	m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	m_Catcher->SetParent(nullptr);
	m_Catcher->DestroyMe();
}

void CharaBase::Init(std::string tag)
{
	m_CharaTag = tag;
	m_pPhysics = GetComponent<Physics>();
	m_Catcher = Instantiate<Catcher>();
	m_Catcher->transform->position = Vector3(0, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	m_Catcher->transform->scale = V3::ONE * CHARADEFINE_REF.CatchRadius * 2;
	m_Catcher->transform->SetParent(transform);
	m_Catcher->Init(tag);
	m_Catcher->SetColliderActive(false);
	m_Catcher->SetParent(this);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig9:Hips", 30.0f, 0.15f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_Animator->LoadAnim("data/Animation/ActionIdle", "ActionIdle", true);
	m_Animator->LoadAnim("data/Animation/ActionIdleEmote", "ActionIdleEmote", false);
	m_Animator->LoadAnim("data/Animation/ActionIdleToJump", "ActionIdleToJump", false);
	m_Animator->LoadAnim("data/Animation/ActionIdleToRun", "ActionIdleToRun", false, true);
	m_Animator->LoadAnim("data/Animation/ActionIdleToStandingIdle", "ActionIdleToStandingIdle", false, true);

	m_Animator->LoadAnim("data/Animation/AirSpin", "AirSpin", false, true);

	m_Animator->LoadAnim("data/Animation/CrouchToActionIdle", "CrouchToActionIdle", false, true);
	m_Animator->LoadAnim("data/Animation/CrouchToRun", "CrouchToRun", false, true);

	m_Animator->LoadAnim("data/Animation/DamageToDown", "DamageToDown", false);

	m_Animator->LoadAnim("data/Animation/Fall", "Fall", true);
	m_Animator->LoadAnim("data/Animation/FallToCrouch", "FallToCrouch", false, true);
	m_Animator->LoadAnim("data/Animation/FallToRollToIdle", "FallToRollToIdle", false, true);

	m_Animator->LoadAnim("data/Animation/Run", "Run", true);
	m_Animator->LoadAnim("data/Animation/RunToActionIdle", "RunToActionIdle", false, true);
	m_Animator->LoadAnim("data/Animation/RunToJump", "RunToJump", false, true);
	m_Animator->LoadAnim("data/Animation/RunToSlide", "RunToSlide", false, true);

	m_Animator->LoadAnim("data/Animation/Slide", "Slide", false, true);
	m_Animator->LoadAnim("data/Animation/SlideToRun", "SlideToRun", false, true);

	m_Animator->LoadAnim("data/Animation/StandingIdle", "StandingIdle", true);
	m_Animator->LoadAnim("data/Animation/StandingIdleEmote", "StandingIdleEmote", false);
	m_Animator->LoadAnim("data/Animation/StandingIdleToActionIdle", "StandingIdleToActionIdle", false, true);
}

void CharaBase::Update() {

	HitGroundProcess();

	if (m_CatchTimer > 0.0f)
	{
		m_CatchTimer -= Time::DeltaTimeLapseRate();
		if (m_CatchTimer < 0.0f)
		{
			m_CatchTimer = 0.0f;
			m_Catcher->SetColliderActive(false);
		}
		else
		{
			m_Catcher->SetColliderActive(true);
		}

		m_pStamina->Use(CATCH_STAMINA_USE * Time::DeltaTimeLapseRate());
	}
	if (m_SlideTimer > 0.0f)
	{
		m_SlideTimer -= Time::DeltaTimeLapseRate();
		if (m_SlideTimer < 0.0f)
		{
			m_SlideTimer = 0.0f;
			m_pPhysics->SetFriction(FRICTION);
		}
		else
		{
			m_pPhysics->SetFriction(FRICTION / 10.0f);
		}
	}

	m_FSM->Update();

	Object3D::Update();
}

void CharaBase::Draw()
{
	Object3D::Draw();
}

void CharaBase::CollisionEvent(const CollisionData& colData) {

	// �����������肪�L�����N�^�[�̏ꍇ
	if (colData.Other()->Tag() == ColDefine::Tag::tCharaRed || colData.Other()->Tag() == ColDefine::Tag::tBallBlue) {

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
	Physics* physics = m_pPhysics;
	if (physics == nullptr)
		return;
	if (physics->velocity.y > 0)
	{
		physics->SetGravity(GRAVITY);
		physics->SetFriction(V3::ZERO);
		m_IsLanding = false;
		return;
	}

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
		physics->SetFriction(FRICTION);
		m_IsLanding = true;
	}
	else
	{
		physics->SetGravity(GRAVITY);
		physics->SetFriction(V3::ZERO);
		m_IsLanding = false;
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
	if (m_SlideTimer > 0.0f) return;

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

void CharaBase::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
}

void CharaBase::Slide()
{
	if (m_FSM->GetCurrentState() == &CharaBase::StateSlideToRun)
	{
		return;
	}

	m_SlideTimer = SLIDE_TIME;
}

void CharaBase::ThrowBall(const Vector3& velocity)
{
	if (m_pBall == nullptr)
		return;

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	m_pLastBall = m_pBall;
	m_pBall = nullptr;
}

void CharaBase::ThrowBallForward()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + V3::SetY(0.4f);

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	m_pLastBall = m_pBall;
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
	m_pBall->Init(m_CharaTag);
}

void CharaBase::TeleportToLastBall()
{
	if (m_pLastBall == nullptr) return;
	transform->position = m_pLastBall->transform->position;

	// ToDo:�����鉉�o
	m_pLastBall->DestroyMe();

	GenerateBall();

	m_pLastBall = nullptr;
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_FSM->ChangeState(&CharaBase::StateAirSpin);
}

void CharaBase::Catch()
{
	if (m_pStamina->GetCurrent() > CATCH_STAMINA_MIN)
	{
		m_CatchTimer = CATCH_TIME;
	}
}

//========================================================================

void CharaBase::StateActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("ActionIdle");
		m_EmoteTimer = CHARADEFINE_REF.EmoteInterval;
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();

		m_EmoteTimer -= Time::DeltaTimeLapseRate();
		if (m_EmoteTimer < 0)
		{
			int rand = GetRand(99);
			if (rand < 50)
			{
				m_FSM->ChangeState(&CharaBase::StateActionIdleToStandingIdle); // �X�e�[�g��ύX
			}
			else
			{
				m_FSM->ChangeState(&CharaBase::StateActionIdleEmote); // �X�e�[�g��ύX
			}
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateActionIdleEmote(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("ActionIdleEmote");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateActionIdleToJump(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("ActionIdleToJump");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateFall); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateActionIdleToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("ActionIdleToRun");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // �X�e�[�g��ύX
			m_Animator->Play("Run");
			//m_Animator->SetCurrentFrame(3.0f);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateActionIdleToStandingIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("ActionIdleToStandingIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateStandingIdleEmote); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateAirSpin(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("AirSpin");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateFall); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateCrouchToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("CrouchToActionIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateCrouchToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("CrouchToRun");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateDamageToDown(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("DamageToDown");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateCrouchToActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateFall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("Fall");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_IsLanding)
		{
			if (m_pPhysics->FlatVelocity().SquareSize() > 0)
			{
				m_FSM->ChangeState(&CharaBase::StateFallToRollToIdle); // �X�e�[�g��ύX
			}
			else
			{
				m_FSM->ChangeState(&CharaBase::StateFallToCrouch); // �X�e�[�g��ύX
			}
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateFallToCrouch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("FallToCrouch");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_pPhysics->FlatVelocity().SquareSize() > 0)
		{
			m_FSM->ChangeState(&CharaBase::StateCrouchToRun); // �X�e�[�g��ύX
		}
		else
		{
			m_FSM->ChangeState(&CharaBase::StateCrouchToActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateFallToRollToIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("FallToRollToIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("Run");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		runUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateRunToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("RunToActionIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateRunToJump(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("RunToJump");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateFall); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateRunToSlide(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("RunToSlide");
		m_pPhysics->velocity = m_pPhysics->UpVelocity() + m_pPhysics->FlatVelocity() * 2.0f;
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		slideUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateSlide); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateSlide(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("Slide");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		slideUpdate();
		if (m_SlideTimer <= 0.0f)
		{
			m_FSM->ChangeState(&CharaBase::StateSlideToRun); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateSlideToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("SlideToRun");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateStandingIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("StandingIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateStandingIdleEmote(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("StandingIdleEmote");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateStandingIdleToActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

void CharaBase::StateStandingIdleToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("StandingIdleToActionIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
	}
	break;
	}
}

//========================================================================

void CharaBase::idleUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToJump); // �X�e�[�g��ύX
	}
	else if (m_pPhysics->FlatVelocity().SquareSize() > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToRun); // �X�e�[�g��ύX
	}
}

void CharaBase::runUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToJump); // �X�e�[�g��ύX
	}
	if (m_pPhysics->FlatVelocity().SquareSize() <= 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToActionIdle); // �X�e�[�g��ύX
	}
	if (m_SlideTimer > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToSlide); // �X�e�[�g��ύX
	}
}

void CharaBase::slideUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToJump); // �X�e�[�g��ύX
	}
}
