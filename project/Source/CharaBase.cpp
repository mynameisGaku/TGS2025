#include "CharaBase.h"
#include "InputManager.h"
#include "EffectManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Stage.h"
#include "StageObjectManager.h"
#include "CharaStamina.h"
#include "CharaHP.h"
#include "Ball.h"
#include "BallManager.h"
#include "Catcher.h"
#include "CharaDefineRef.h"
#include "Component/Animator.h"
#include "Util/Utils.h"
#include "Timeline.h"

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
	m_pHP					= Instantiate<CharaHP>();
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
	m_EffectTransform		= nullptr;
	m_pBallManager			= nullptr;

	m_FSM = new TinyFSM<CharaBase>(this);

#if FALSE
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
#endif // FALSE

	m_FSM->ChangeState(&CharaBase::StateActionIdle); // �X�e�[�g��ύX
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	Function::DeletePointer(m_FSM);
	Function::DeletePointer(m_Timeline);

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

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 100.0f;
	m_EffectTransform->position.z = 100.0f;
	m_EffectTransform->rotation.y = Math::DegToRad(180.0f);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig9:Hips", 30.0f, 0.15f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");

	m_Timeline = new Timeline<CharaBase>(this, m_Animator);
	m_Timeline->SetFunction("SetAnimationSpeed", &CharaBase::setAnimationSpeed);
	m_Timeline->LoadJsons("data/Json/Chara/State");

#if FALSE

	//=== �r�A�j���[�V�����i�{�[�������A������j ===
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToHold", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "AimToThrow", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "Catch", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "Hold", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "HoldToAim", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "ThrowToActionIdle", AnimOption());

	//=== ���C���A�j���[�V���� ===
	m_Animator->LoadAnim("data/Animation/", "ActionIdle", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleEmote", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToJump", AnimOption().SetIsFixedRoot({ false, true, false }).SetOffset(V3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToRun", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToStandingIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "AirSpin", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "CrouchToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "CrouchToRun", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "DamageToDown", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "Fall", AnimOption().SetIsLoop(true).SetIsFixedRoot({ false, true, false }).SetOffset(V3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "FallToCrouch", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "FallToRollToIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "GetBall", AnimOption());

	m_Animator->LoadAnim("data/Animation/", "Run", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "RunToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "RunToJump", AnimOption().SetIsFixedRoot({ false, true, true }).SetOffset(V3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "RunToSlide", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "Slide", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "SlideToRun", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "StandingIdle", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "StandingIdleEmote", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "StandingIdleToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

#endif // FALSE
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
			if (m_EffectTransform != nullptr)
			{
				EffectManager::Play3D("Catch_Ready_Single_Dust.efk", m_EffectTransform->Global(), "Catch_Ready_Single_Dust" + m_CharaTag);
				EffectManager::Play3D("Catch_Ready_Single_Tornado.efk", m_EffectTransform->Global(), "Catch_Ready_Single_Tornado" + m_CharaTag);
			}
		}

		m_pStamina->Use(CATCH_STAMINA_USE * Time::DeltaTimeLapseRate());
	}
	else
	{
		EffectManager::Stop("Catch_Ready_Single_Dust.efk", "Catch_Ready_Single_Dust" + m_CharaTag);
		EffectManager::Stop("Catch_Ready_Single_Tornado.efk", "Catch_Ready_Single_Tornado" + m_CharaTag);
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

	if (m_pHP->IsDead())
	{
		DrawFormatString(300, 300, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 320, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 340, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 360, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 380, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 400, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 320, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 340, 0xffffff, "��������������������������������������������������");
		DrawFormatString(300, 360, 0xffffff, "��������������������������������������������������");
	}
}

void CharaBase::CollisionEvent(const CollisionData& colData) {

	// �����������肪�L�����N�^�[�̏ꍇ
	if (colData.Other()->Tag() == ColDefine::Tag::tCharaRed) {

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
	// �{�[���̏ꍇ
	else if (colData.Other()->Tag() == ColDefine::Tag::tBallBlue || colData.Other()->Tag() == ColDefine::Tag::tBallRed)
	{
		// ����̏��
		Ball* ball = colData.Other()->Parent<Ball>();

		if (ball->GetCharaTag() != m_CharaTag)
		{
			getHit(ball);
			m_pHP->Damage_UseDefault();
		}
	}
}

void CharaBase::HitGroundProcess() {

	// ��������
	Physics* physics = m_pPhysics;
	if (physics == nullptr)
		return;

	// �n�`(StageObject)�Ɠ����蔻�肷��
	{
		Vector3 p1, p2;

		ColliderCapsule* capsuleCol = GetComponent<ColliderCapsule>();

		p1 = transform->Global().position + V3::SetY(37);
		p2 = capsuleCol->OffsetWorld();

		const float radius = capsuleCol ? capsuleCol->Radius() : 20.0f;

		Vector3 pushVec;
		if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
		{
			// �����o���x�N�g���ňʒu��␳
			transform->position += pushVec;

			// Y������������Ȃ�ڒn�Ƃ݂Ȃ�
			if (pushVec.y > 0.1f)
			{
				m_IsLanding = true;
				m_pPhysics->velocity.y = 0.0f;
				m_pPhysics->resistance.y = 0.0f;
				m_pPhysics->SetGravity(V3::ZERO);
				m_pPhysics->SetFriction(FRICTION);
			}
			// Y�������������Ȃ瓪��q�b�g�i�V��ɂԂ������j
			else if (pushVec.y < -0.1f)
			{
				m_pPhysics->velocity.y = min(m_pPhysics->velocity.y, 0.0f);
			}
		}
		else
		{
			// �Փ˂��Ă��Ȃ���΁A�ʏ�̋󒆋�����
			m_IsLanding = false;
			m_pPhysics->SetGravity(GRAVITY);
			m_pPhysics->SetFriction(V3::ZERO);
		}
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

	if (physics->velocity.y > 0)
	{
		physics->SetGravity(GRAVITY);
		physics->SetFriction(V3::ZERO);
		m_IsLanding = false;
		return;
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

	m_FSM->ChangeState(&CharaBase::StateAimToThrow); // �X�e�[�g��ύX
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

	m_FSM->ChangeState(&CharaBase::StateAimToThrow); // �X�e�[�g��ύX
}

void CharaBase::ThrowHomingBall()
{
    if (m_pBall == nullptr)
        return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + V3::SetY(0.4f);
    //m_pBall->ThrowHoming(velocity * (1.0f + m_BallChargeRate), this);
    m_pBall->ThrowHoming(velocity * 3000.0f, this);
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

	if(m_pBallManager == nullptr)
	{
		m_pBallManager = FindGameObject<BallManager>();
	}

	m_pBall = m_pBallManager->CreateBall(transform->Global().position);

	if (m_pBall == nullptr)
		return;

	m_pBall->transform->position = transform->Global().position;
	m_pBall->transform->rotation = transform->Global().rotation;
	//m_pBall->SetParent(this);
	m_pBall->Init(m_CharaTag);

	m_FSM->ChangeState(&CharaBase::StateGetBall); // �X�e�[�g��ύX
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
		m_FSM->ChangeState(&CharaBase::StateCatch); // �X�e�[�g��ύX
	}
}

//========================================================================

void CharaBase::StateActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Timeline->Play("ActionIdle");
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

void CharaBase::StateCatch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("Catch");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_CatchTimer <= 0.0f)
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

void CharaBase::StateGetBall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("GetBall");
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

void CharaBase::StateAimToThrow(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("AimToThrow");
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

void CharaBase::getHit(Ball* hit)
{
	m_FSM->ChangeState(&CharaBase::StateDamageToDown);

	Vector3 dif = hit->transform->position - transform->position;

	float forwardRad = atan2f(dif.x, dif.z);
	transform->rotation.y = forwardRad;
	m_pPhysics->velocity += transform->Forward() * -50.0f;	// ToDo:�O����
}

void CharaBase::setAnimationSpeed(const nlohmann::json& argument)
{
	m_Animator->SetPlaySpeed(argument.at("Speed").get<float>());
}
