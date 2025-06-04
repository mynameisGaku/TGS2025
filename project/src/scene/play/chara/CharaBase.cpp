#include "src/scene/play/chara/CharaBase.h"
#include "src/util/input/InputManager.h"
#include "src/util/fx/effect/EffectManager.h"

#include "src/common/component/physics/Physics.h"
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/stage/Stage.h"
#include "src/common/stage/StageObjectManager.h"
#include "src/scene/play/chara/CharaStamina.h"
#include "src/scene/play/chara/CharaHP.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/catcher/Catcher.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/common/component/animator/Animator.h"
#include "src/common/timeline/Timeline.h"
#include "src/util/file/json/VectorJson.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/ptr/PtrUtil.h"
#include "src/util/math/MathUtil.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include "src/util/fx/trail/trail3D/Trail3D.h"

using namespace KeyDefine;

namespace
{
	static const float CATCH_STAMINA_USE = 50.0f;	// �L���b�`�Ɏg���X�^�~�i�i���b�j
	static const float CATCH_STAMINA_MIN = 20.0f;	// �L���b�`���J�n����̂ɕK�v�Ȏc�X�^�~�i
	static const float CATCH_TIME = 0.05f;	// ���͈��̃L���b�`�p������
	static const float SLIDE_TIME = 0.05f;	// ���͈��̃X���C�f�B���O�p������
	static const float CHARGE_TIME = 1.0f;
	static const float CHARGE_BALLSPEED = 1.5f;
}

CharaBase::CharaBase()
{
	m_pStamina			= Instantiate<CharaStamina>();
	m_pHP				= Instantiate<CharaHP>();
	m_pBall				= nullptr;
	m_pLastBall			= nullptr;
	m_pPhysics			= nullptr;
	m_BallChargeRate	= 0.0f;
	m_IsCharging		= false;
	m_MoveSpeed			= 0.0f;
	m_RotSpeed			= 0.0f;
	m_SpeedScale		= 0.0f;
	m_CharaTag			= CHARADEFINE_REF.Tags[0];
	m_Catcher			= nullptr;
	m_Index				= 0;
	m_Animator			= nullptr;
	m_EmoteTimer		= 0.0f;
	m_IsLanding			= true;
	m_SlideTimer		= 0.0f;
	m_EffectTransform	= nullptr;
	m_pBallManager		= nullptr;
	m_Timeline			= nullptr;
	m_CanMove			= true;
	m_CanRot			= true;
	m_IsMove			= false;
	m_IsJumping			= false;
	m_CanCatch			= true;
	m_CanThrow			= true;
	m_IsCatching		= false;
	m_CanHold			= true;
	m_pHitBall			= nullptr;
	m_pStatusTracker	= nullptr;
	m_pCatchReadyEffect	= nullptr;
	m_pCatchDustEffect	= nullptr;
	m_CatchTimer		= 0.0f;

	m_FSM = new TinyFSM<CharaBase>(this);
	m_SubFSM = new TinyFSM<CharaBase>(this);

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
	m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	PtrUtil::SafeDelete(m_FSM);
	PtrUtil::SafeDelete(m_SubFSM);
	PtrUtil::SafeDelete(m_Timeline);

	m_Catcher->SetParent(nullptr);
	m_Catcher->DestroyMe();

    for (int i = 0; i < 5; ++i)
    {
        PtrUtil::SafeDelete(m_pTrail[i]);
    }

	PtrUtil::SafeDelete(m_EffectTransform);
}

void CharaBase::Init(std::string tag)
{
	m_CharaTag = tag;
	m_pStatusTracker = new StatusTracker();
	m_pPhysics = GetComponent<Physics>();
	m_Catcher = Instantiate<Catcher>();
	m_Catcher->transform->position = Vector3(0.0f, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	m_Catcher->transform->scale = Vector3::Ones * CHARADEFINE_REF.CatchRadius * 2.0f;
	m_Catcher->transform->SetParent(transform);
	m_Catcher->Init(tag);
	m_Catcher->SetColliderActive(false);
	m_Catcher->SetParent(this);

	float s = 0.35f;
	// ��
	{
		m_pTrail[0] = new Trail3D();
		m_pTrail[0]->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, s * 1.5f, 50.0f);
		m_pTrail[0]->SetInterval(1);
		m_pTrail[0]->SetSubdivisions(16);
	}
    // ����
    {
        m_pTrail[1] = new Trail3D();
        m_pTrail[1]->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, s, 20.0f);
        m_pTrail[1]->SetInterval(1);
        m_pTrail[1]->SetSubdivisions(16);
    }
    // �E��
    {
        m_pTrail[2] = new Trail3D();
        m_pTrail[2]->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, s, 20.0f);
        m_pTrail[2]->SetInterval(1);
        m_pTrail[2]->SetSubdivisions(16);
    }
	// ����
    {
        m_pTrail[3] = new Trail3D();
        m_pTrail[3]->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, s, 20.0f);
        m_pTrail[3]->SetInterval(1);
        m_pTrail[3]->SetSubdivisions(16);
    }
    // �E��
    {
        m_pTrail[4] = new Trail3D();
        m_pTrail[4]->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, s, 20.0f);
        m_pTrail[4]->SetInterval(1);
        m_pTrail[4]->SetSubdivisions(16);
    }

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 160.0f;
	m_EffectTransform->position.z = -100.0f;
	m_EffectTransform->rotation.y = MathUtil::ToRadians(180.0f);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig9:Hips", 30.0f, 0.15f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");

	m_Timeline = new Timeline<CharaBase>(this, m_Animator);
	m_Timeline->SetFunction("SetAnimationSpeed", &CharaBase::setAnimationSpeed);
	m_Timeline->SetFunction("MoveToPosition", &CharaBase::moveToPosition);
	m_Timeline->SetFunction("ChangeToRoll", &CharaBase::changeToRoll);
	m_Timeline->SetFunction("EndRoll", &CharaBase::endRoll);
	m_Timeline->SetFunction("SetCanMove", &CharaBase::setCanMove);
	m_Timeline->SetFunction("SetCanRot", &CharaBase::setCanRot);
	m_Timeline->SetFunction("SetVelocity", &CharaBase::setVelocity);
	m_Timeline->SetFunction("ThrowBall", &CharaBase::throwBall);
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
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToJump", AnimOption().SetIsFixedRoot({ false, true, false }).SetOffset(Vector3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToRun", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToStandingIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "AirSpin", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "CrouchToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "CrouchToRun", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "DamageToDown", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "Fall", AnimOption().SetIsLoop(true).SetIsFixedRoot({ false, true, false }).SetOffset(Vector3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "FallToCrouch", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "FallToRollToIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "GetBall", AnimOption());

	m_Animator->LoadAnim("data/Animation/", "Run", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "RunToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "RunToJump", AnimOption().SetIsFixedRoot({ false, true, true }).SetOffset(Vector3::SetY(100.0f)));
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

	// �f�o�b�O�@�\
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_Timeline->LoadJsons("data/Json/Chara/State");
		m_Animator->DeleteAnimInfos();
		m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");
	}

	m_FSM->Update();
	m_SubFSM->Update();
	m_Timeline->Update();

	//if (m_pCatchReadyEffect)
	//{
	//	m_pCatchReadyEffect->SetPosition3D(transform->Global().position + VTransform(m_EffectTransform->position, m_EffectTransform->Global().RotationMatrix()));
	//	m_pCatchReadyEffect->SetRotation3D(transform->Global().rotation + m_EffectTransform->rotation);
	//}
	//if (m_pCatchDustEffect)
	//{
	//	m_pCatchDustEffect->SetPosition3D(transform->Global().position + VTransform(m_EffectTransform->position, m_EffectTransform->Global().RotationMatrix()));
	//	m_pCatchDustEffect->SetRotation3D(transform->Global().rotation + m_EffectTransform->rotation);
	//}

	// �{�[���̍X�V
	if (m_pBall)
	{
		m_pBall->transform->position = VTransform(Vector3(0.0f, BALL_RADIUS, -BALL_RADIUS), MV1GetFrameLocalWorldMatrix(Model(), MV1SearchFrame(Model(), "mixamorig9:RightHand")));
	}

	static const float MOVE_ACCEL = 0.03f;

	// �ړ����x�{���̍X�V
	if (m_IsMove)
	{
		m_SpeedScale += MOVE_ACCEL;
		if (m_SpeedScale > 1.0f)
		{
			m_SpeedScale = 1.0f;
		}
	}
	else
	{
		m_SpeedScale = 0.0f;
	}

	m_IsMove = false;
	m_IsCatching = false;

	Object3D::Update();

	Vector3 chestPos = MV1GetFramePosition(Model(), MV1SearchFrame(Model(), "mixamorig9:Spine2"));
	m_pTrail[0]->Add(chestPos);

    Vector3 leftShoulderPos = MV1GetFramePosition(Model(), MV1SearchFrame(Model(), "mixamorig9:LeftShoulder"));
    m_pTrail[1]->Add(leftShoulderPos);

    Vector3 rightShoulderPos = MV1GetFramePosition(Model(), MV1SearchFrame(Model(), "mixamorig9:RightShoulder"));
    m_pTrail[2]->Add(rightShoulderPos);

    Vector3 leftHipPos = MV1GetFramePosition(Model(), MV1SearchFrame(Model(), "mixamorig9:LeftUpLeg"));
    m_pTrail[3]->Add(leftHipPos);

    Vector3 rightHipPos = MV1GetFramePosition(Model(), MV1SearchFrame(Model(), "mixamorig9:RightUpLeg"));
    m_pTrail[4]->Add(rightHipPos);
	
	for (int i = 0; i < 5; i++)
	{
        m_pTrail[i]->Update();
	}

}

void CharaBase::Draw()
{
	Object3D::Draw();

	for (int i = 0; i < 5; i++)
	{
		m_pTrail[i]->Draw();
	}

	if (m_pHP->IsDead())
	{
		DrawFormatString(300, 300 + m_Index * 40, 0xff0000, std::string("Dead [index:" + std::to_string(m_Index) + "]").c_str());
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
			if (toVec.GetLengthSquared() == 0)
				toVec = Vector3(0, 0, 1);

			// �����͂̋������`
			constexpr float REPELLENT_FORCE_SCALE_MAX = 2.0f;
			constexpr float REPELLENT_FORCE_SCALE_MIN = 1.0f;
			const float REPELLENT_RADIUS = collider->Radius() * 4.0f;

			// ������
			const Vector3 repellentForce = toVec.Normalize() * (REPELLENT_FORCE_SCALE_MIN + (REPELLENT_FORCE_SCALE_MAX - REPELLENT_FORCE_SCALE_MIN) * max(0.0f, 1.0f - toVec.GetLength() / REPELLENT_RADIUS));

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
			if (ball->GetState() == Ball::S_LANDED)
				return;

			if (m_CharaTag == "Blue")
			{
				EffectManager::Play3D("Hit_Blue.efk", *transform->Copy(), "Hit_Blue" + m_CharaTag);
			}
			else
			{
				EffectManager::Play3D("Hit_Red.efk", *transform->Copy(), "Hit_Red" + m_CharaTag);
			}

			getHit(ball);
			m_pHP->Damage_UseDefault();
			m_pStatusTracker->AddGetHitCount(1);

			ball->GetLastOwner()->GetStatusTracker()->AddHitCount(1);
			if (m_pHP->IsDead())
			{
				m_pStatusTracker->AddDeathCount(1);
				ball->GetLastOwner()->GetStatusTracker()->AddKillCount(1);
				Respawn(Vector3::Zero, Vector3::Zero);
			}
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

		static const float DOWN_OFFSET = 10.0f;

		p1 = transform->Global().position + Vector3::SetY(37);
		p2 = capsuleCol->OffsetWorld() - Vector3::SetY(DOWN_OFFSET);

		const float radius = capsuleCol ? capsuleCol->Radius() : 20.0f;

		Vector3 pushVec;
		if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
		{
			// �����o���x�N�g���ňʒu��␳
			transform->position += pushVec;

			// Y������������Ȃ�ڒn�Ƃ݂Ȃ�
			if ((m_pPhysics->velocity.y <= 0.0f) and (pushVec.y > 0))
			{
				land();
				transform->position -= pushVec.Normalize() * DOWN_OFFSET;
				m_pPhysics->velocity.y = 0.0f;
				m_pPhysics->resistance.y = 0.0f;
			}
			else
			{
				m_IsLanding = false;

				// Y�������������Ȃ瓪��q�b�g�i�V��ɂԂ������j
				if (pushVec.y < -0.1f)
				{
					m_pPhysics->velocity.y = min(m_pPhysics->velocity.y, 0.0f);
					m_IsJumping = false;
				}
			}
		}
		else
		{
			m_IsLanding = false;
		}

		// �Փ˂��Ă��Ȃ���΁A�ʏ�̋󒆋�����
		if (not m_IsLanding)
		{
			m_pPhysics->SetGravity(GRAVITY);
			m_pPhysics->SetFriction(Vector3::Zero);
		}
	}
}

void CharaBase::Move(const Vector3& dir)
{
	m_IsMove = dir.GetLengthSquared() > 0;

	if (m_CanRot)
	{
		float currentRot = transform->rotation.y;	// ���݂̌���
		float terminusRot = atan2f(dir.x, dir.z);		// �I�_�̌���

		// ���X�ɏI�_�̌����֍��킹��
		transform->rotation.y = MathUtil::RotAngle(currentRot, terminusRot, m_RotSpeed);
	}

	if (m_CanMove)
	{
		const Vector3 normDir = Vector3::Normalize(dir);
		float deltaTimeMoveSpeed = m_MoveSpeed * m_SpeedScale * GTime.deltaTime;	// ���Ԍo�ߗ���K�������ړ����x

		Vector3 velocity = normDir * deltaTimeMoveSpeed * Vector3::Horizontal;	// �X�e�B�b�N�̌X���̕����ւ̑��x

		// ���x��K��������
		m_pPhysics->velocity.x = velocity.x;
		m_pPhysics->velocity.z = velocity.z;
	}
}

void CharaBase::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_IsJumping = true;
}

void CharaBase::Slide()
{
	if (m_FSM->GetCurrentState() == &CharaBase::StateSlideToRun)
	{
		return;
	}

	m_SlideTimer = SLIDE_TIME;
}

void CharaBase::GenerateBall()
{
	if (not m_CanCatch) return;

	if (m_pBall != nullptr)
		return;

	if (m_pBallManager == nullptr)
		m_pBallManager = FindGameObject<BallManager>();

	SetBall(m_pBallManager->CreateBall(transform->Global().position));
}

void CharaBase::SetBall(Ball* ball)
{
	m_pBall = ball;

	if (m_pBall == nullptr)
		return;

	m_pBall->transform->position = transform->Global().position;
	m_pBall->transform->rotation = transform->Global().rotation;
	m_pBall->Reset(m_CharaTag);

	m_IsCharging = false;
}

void CharaBase::StartBallCharge()
{
	m_IsCharging = true;
	m_SubFSM->ChangeState(&CharaBase::SubStateHoldToAim); // �X�e�[�g��ύX
}

void CharaBase::StartThrow()
{
	if (m_pBall == nullptr)
		return;
	if (not m_CanThrow)
		return;

	m_FSM->ChangeState(&CharaBase::StateAimToThrow); // �X�e�[�g��ύX
}

void CharaBase::ThrowBall(const Vector3& velocity)
{
	if (m_pBall == nullptr)
		return;

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate * CHARGE_BALLSPEED), this);

	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
}

void CharaBase::ThrowBallForward()
{
	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + Vector3::SetY(0.15f);	// Magic:)

	ThrowBall(velocity);
}

void CharaBase::ThrowHomingBall()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = (forward * 35.0f) + Vector3::SetY(0.3f);	// Magic:)
	
	const CharaBase* targetChara = nullptr;
	Camera* camera = CameraManager::GetCamera(m_Index);

	if (camera != nullptr)
		targetChara = camera->TargetChara();	// �J�����̃^�[�Q�b�g�L�������擾

	m_pBall->ThrowHoming(velocity * (1.0f + m_BallChargeRate * CHARGE_BALLSPEED), this, targetChara);
	m_pStatusTracker->AddThrowCount(1);
	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
}

void CharaBase::Feint()
{
	if (not m_CanThrow)
		return;
	m_FSM->ChangeState(&CharaBase::StateFeint); // �X�e�[�g��ύX
}

void CharaBase::TeleportToLastBall()
{
	if (m_pLastBall == nullptr) return;
	transform->position = m_pLastBall->transform->position;

	// ToDo:�����鉉�o
	m_pLastBall->SetIsActive(false);

	GenerateBall();

	m_pLastBall = nullptr;
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_FSM->ChangeState(&CharaBase::StateAirSpin);
}

void CharaBase::Catch()
{
	if (not m_CanCatch) return;

	if (m_pStamina->GetCurrent() > CATCH_STAMINA_MIN)
	{
		m_IsCatching = true;
	}
}

void CharaBase::Respawn(const Vector3& pos, const Vector3& rot)
{
	m_pHP->Reset();
	m_pStamina->Reset();
	m_pBall = nullptr;
	m_pLastBall = nullptr;
	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
	m_IsJumping = false;
	m_IsLanding = true;
	transform->position = pos;
	transform->rotation = rot;
	// �L���b�`���[�̈ʒu���X�V
	if (m_Catcher)
	{
		m_Catcher->transform->position = Vector3(0.0f, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	}
}

void CharaBase::SetTrailImage(int hImage)
{
    m_hTrailImage = hImage;
}

void CharaBase::CatchSuccess(const Vector3& velocity)
{
	m_CanCatch = false;
	m_CanMove = false;
	m_CanRot = false;
	m_Catcher->SetColliderActive(false);

	m_pPhysics->SetGravity(Vector3::Zero);
	m_pPhysics->SetFriction(Vector3::Zero);

	transform->rotation.y = atan2f(transform->position.x - velocity.x, transform->position.z - velocity.z);
	m_pPhysics->velocity.y = 10.0f;
	m_pPhysics->velocity += Vector3::Normalize(velocity) * Vector3(1, 0, 1) * 10.0f;

	m_SubFSM->ChangeState(&CharaBase::SubStateGetBall); // �X�e�[�g��ύX
}

//========================================================================
// ���C���X�e�[�g

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

		m_EmoteTimer -= GTime.deltaTime;
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
		m_Timeline->Stop();
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
		m_Timeline->Play("ActionIdleEmote");
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
		m_Timeline->Stop();
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
		m_Timeline->Play("ActionIdleToJump");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		jumpUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Timeline->Stop();
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
		m_Timeline->Play("ActionIdleToStandingIdle");
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
		m_Timeline->Stop();
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
		m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
		m_Timeline->Play("AimToThrow");
		m_CanCatch = false;
		m_CanHold = false;
		m_CanThrow = false;
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
		m_CanCatch = true;
		m_CanHold = true;
		m_CanThrow = true;
		m_Timeline->Stop();
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

		m_CanCatch	= false;
		m_CanMove	= false;
		m_CanRot	= false;
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
		m_CanCatch	= true;
		m_CanMove	= true;
		m_CanRot	= true;
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
		m_Timeline->Play("Fall");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_IsLanding)
		{
			if (m_IsMove)
			{
				m_FSM->ChangeState(&CharaBase::StateFallToRoll); // �X�e�[�g��ύX
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
		m_Timeline->Stop();
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
		if (m_pPhysics->FlatVelocity().GetLengthSquared() > 0)
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

void CharaBase::StateFallToRoll(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Timeline->Play("FallToRoll");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Timeline->Stop();

		m_CanMove = true;
		m_CanRot = true;
	}
	break;
	}
}

void CharaBase::StateFeint(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
		m_Timeline->Play("AimToThrow");
		m_CanCatch = false;
		m_CanHold = false;
		m_CanThrow = false;
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
		m_SubFSM->ChangeState(&CharaBase::SubStateHoldToAim); // �X�e�[�g��ύX

		m_CanCatch = true;
		m_CanHold = true;
		m_CanThrow = true;

		m_Timeline->Stop();
	}
	break;
	}
}

void CharaBase::StateRoll(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Timeline->Play("Roll");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Timeline->Stop();
		m_CanMove = true;
		m_CanRot = true;
	}
	break;
	}
}

void CharaBase::StateRollToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		float frame = m_Animator->CurrentFrame();
		m_Timeline->Play("RollToActionIdle");
		m_Animator->SetCurrentFrame(frame);
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
		m_Timeline->Stop();
	}
	break;
	}
}

void CharaBase::StateRollToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		float frame = m_Animator->CurrentFrame();
		m_Timeline->Play("RollToRun");
		m_Animator->SetCurrentFrame(frame);
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // �X�e�[�g��ύX

			m_Animator->Play("Run");

			static const float RUN_FROM_ROLL_START = 10.0f;
			m_Animator->SetCurrentFrame(RUN_FROM_ROLL_START);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Timeline->Stop();
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
		m_Timeline->Play("Run");
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
		m_Timeline->Stop();
		m_Animator->SetPlaySpeed(1.0f);
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
		m_Timeline->Play("RunToActionIdle");
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
		m_Timeline->Stop();
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
		jumpUpdate();
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
		m_CanMove = false;
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
		m_CanMove = true;
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
		m_Timeline->Play("StandingIdleToActionIdle");
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
		m_Timeline->Stop();
	}
	break;
	}
}

//========================================================================
// �T�u�X�e�[�g

void CharaBase::SubStateNone(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_CanCatch && m_IsCatching)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateCatch); // �X�e�[�g��ύX
		}
		else if (m_CanHold && m_pBall != nullptr)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateHold); // �X�e�[�g��ύX
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

void CharaBase::SubStateGetBall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->PlaySub("mixamorig9:Spine", "GetBall");
		m_CanMove = false;
		m_CanRot = false;
		m_pPhysics->SetGravity(Vector3::Zero);
		m_pPhysics->SetFriction(Vector3::Zero);
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_Animator->IsFinishedSub("mixamorig9:Spine"))
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateHold); // �X�e�[�g��ύX
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_CanMove = true;
		m_CanRot = true;
		m_pPhysics->SetGravity(GRAVITY);
		m_pPhysics->SetGravity(FRICTION);
	}
	break;
	}
}

void CharaBase::SubStateHold(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->PlaySub("mixamorig9:Spine", "Hold");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_pBall == nullptr)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
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

void CharaBase::SubStateHoldToAim(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->PlaySub("mixamorig9:Spine", "HoldToAim");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		m_BallChargeRate += GTime.deltaTime / CHARGE_TIME;
		if (m_BallChargeRate > 1.0f)
		{
			m_BallChargeRate = 1.0f;
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

void CharaBase::SubStateCatch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->PlaySub("mixamorig9:Spine", "Catch");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (not m_IsCatching)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
		}

		catchUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Catcher->SetColliderActive(false);

		if (m_CharaTag == "Blue")
		{
			EffectManager::Stop("Catch_Dust_Blue.efk", "Catch_Dust_Blue" + m_CharaTag);
			EffectManager::Stop("Catch_Ready_Blue.efk", "Catch_Ready_Blue" + m_CharaTag);
		}
		else if (m_CharaTag == "Red")
		{
			EffectManager::Stop("Catch_Dust_Red.efk", "Catch_Dust_Red" + m_CharaTag);
			EffectManager::Stop("Catch_Ready_Red.efk", "Catch_Ready_Red" + m_CharaTag);
		}
		//m_pCatchReadyEffect = nullptr;
		//m_pCatchDustEffect = nullptr;
	}
	break;
	}
}

//========================================================================
// private�֐�

void CharaBase::land()
{
	m_IsLanding = true;
	m_IsJumping = false;
	m_pPhysics->SetGravity(Vector3::Zero);
	m_pPhysics->SetFriction(FRICTION);
}

void CharaBase::idleUpdate()
{
	if (m_IsJumping)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToJump); // �X�e�[�g��ύX
	}
	else if (not m_IsLanding)
	{
		m_FSM->ChangeState(&CharaBase::StateFall); // �X�e�[�g��ύX
	}
	else if (m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToRun); // �X�e�[�g��ύX
	}
}

void CharaBase::runUpdate()
{
	if (m_IsJumping)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToJump); // �X�e�[�g��ύX
	}
	else if (not m_IsLanding)
	{
		m_FSM->ChangeState(&CharaBase::StateFall); // �X�e�[�g��ύX
	}
	else if (not m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToActionIdle); // �X�e�[�g��ύX
	}
	else if (m_SlideTimer > 0.0f)
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

	if (m_SlideTimer > 0.0f)
	{
		m_SlideTimer -= GTime.deltaTime;
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
}

void CharaBase::catchUpdate()
{
	if (m_IsCatching)
	{
		//if (not m_Catcher->IsColliderActive())
		//{
			if (m_EffectTransform != nullptr)
			{
				if (m_CharaTag == "Blue")
				{
					//m_pCatchDustEffect = EffectManager::Play3D_Loop("Catch_Dust_Blue.efk", m_EffectTransform->Global(), "Catch_Dust_Blue" + m_CharaTag);
					//m_pCatchReadyEffect = EffectManager::Play3D_Loop("Catch_Ready_Blue.efk", m_EffectTransform->Global(), "Catch_Ready_Blue" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Dust_Blue.efk", m_EffectTransform->Global(), "Catch_Dust_Blue" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Ready_Blue.efk", m_EffectTransform->Global(), "Catch_Ready_Blue" + m_CharaTag);
				}
				else if(m_CharaTag == "Red")
				{
					//m_pCatchDustEffect = EffectManager::Play3D_Loop("Catch_Dust_Red.efk", m_EffectTransform->Global(), "Catch_Dust_Red" + m_CharaTag);
					//m_pCatchReadyEffect = EffectManager::Play3D_Loop("Catch_Ready_Red.efk", m_EffectTransform->Global(), "Catch_Ready_Red" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Dust_Red.efk", m_EffectTransform->Global(), "Catch_Dust_Red" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Ready_Red.efk", m_EffectTransform->Global(), "Catch_Ready_Red" + m_CharaTag);
				}
			}
		//}

		m_Catcher->SetColliderActive(true);
		m_pStamina->Use(CATCH_STAMINA_USE * GTime.deltaTime);
	}
	else
	{
		m_Catcher->SetColliderActive(false);
		//m_pCatchReadyEffect = nullptr;
		//m_pCatchDustEffect = nullptr;
	}
}

void CharaBase::jumpUpdate()
{
	if (m_Animator->IsFinished())
	{
		m_IsJumping = false;
	}

	if (not m_IsJumping)
	{
		m_FSM->ChangeState(&CharaBase::StateFall); // �X�e�[�g��ύX
	}
}

void CharaBase::getHit(Ball* hit)
{
	m_FSM->ChangeState(&CharaBase::StateDamageToDown);

	Vector3 dif = hit->transform->position - transform->position;

	m_pHitBall = hit;

	float forwardRad = atan2f(dif.x, dif.z);
	transform->rotation.y = forwardRad;
	m_pPhysics->velocity += transform->Forward() * -50.0f;	// ToDo:�O����
}

void CharaBase::setAnimationSpeed(const nlohmann::json& argument)
{
	m_Animator->SetPlaySpeed(argument.at("Speed").get<float>());
}

void CharaBase::moveToPosition(const nlohmann::json& argument)
{
	float progress = argument.at("Progress").get<float>();
	float lastProgress = argument.at("LastProgress").get<float>();
	Vector3 dest = argument.at("Position").get<Vector3>();
	dest = VTransform(dest, transform->RotationMatrix());

	Vector3 add = dest * progress;
	Vector3 sub = dest * lastProgress;

	transform->position += add;
	transform->position -= sub;
}

void CharaBase::changeToRoll(const nlohmann::json& argument)
{
	m_FSM->ChangeState(&CharaBase::StateRoll); // �X�e�[�g��ύX
}

void CharaBase::endRoll(const nlohmann::json& argument)
{
	if (m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateRollToRun); // �X�e�[�g��ύX
	}
	else
	{
		m_FSM->ChangeState(&CharaBase::StateRollToActionIdle); // �X�e�[�g��ύX
	}
}

void CharaBase::setCanMove(const nlohmann::json& argument)
{
	m_CanMove = argument.at("CanMove").get<bool>();
	m_CanRot = m_CanMove;
}

void CharaBase::setCanRot(const nlohmann::json& argument)
{
	m_CanRot = argument.at("CanRot").get<bool>();
}

void CharaBase::setVelocity(const nlohmann::json& argument)
{
	m_pPhysics->velocity = argument.at("Velocity").get<Vector3>();
}

void CharaBase::throwBall(const nlohmann::json& argument)
{
	if (m_FSM->GetCurrentState() == &CharaBase::StateAimToThrow)
	{
		ThrowHomingBall();
	}
}
