#include "CharaBase.h"
#include "InputManager.h"
#include "EffectManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Stage.h"
#include "StageObjectManager.h"
#include "CharaStamina.h"
#include "CharaHP.h"
#include "CharaDefineRef.h"
#include "Component/Animator.h"
#include "Util/Utils.h"
#include "Timeline.h"
#include "VectorJson.h"

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
	m_pPhysics				= nullptr;
	m_MoveSpeed				= 0.0f;
	m_RotSpeed				= 0.0f;
	m_SpeedScale			= 0.0f;
	m_CharaTag				= CHARADEFINE_REF.Tags[0];
	m_Index					= 0;
	m_Animator				= nullptr;
	m_EmoteTimer			= 0.0f;
	m_IsLanding				= true;
	m_SlideTimer			= 0.0f;
	m_EffectTransform		= nullptr;
	m_Timeline				= nullptr;
	m_CanMove				= true;
	m_CanRot				= true;
	m_IsMove				= false;
	m_IsBlocking			= false;
	m_IsAttack				= false;

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

	m_FSM->ChangeState(&CharaBase::StateIdle); // �X�e�[�g��ύX
	m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	Function::DeletePointer(m_FSM);
	Function::DeletePointer(m_Timeline);
}

void CharaBase::Init(std::string tag)
{
	m_CharaTag = tag;
	m_pPhysics = GetComponent<Physics>();

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 100.0f;
	m_EffectTransform->position.z = 100.0f;
	m_EffectTransform->rotation.y = Math::DegToRad(180.0f);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig:Hips", 30.0f, 0.15f);
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
	m_IsBlocking = false;
	m_IsAttack = false;

	if (m_pPhysics->velocity.y < -1.0f)
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateFall); // �X�e�[�g��ύX
	}
	else
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateNone); // �X�e�[�g��ύX
	}

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
	m_IsMove = dir.SquareSize() > 0;

	if (m_CanRot)
	{
		float currentRot = transform->rotation.y;	// ���݂̌���
		float terminusRot = atan2f(dir.x, dir.z);		// �I�_�̌���

		// ���X�ɏI�_�̌����֍��킹��
		transform->rotation.y = Function::RotAngle(currentRot, terminusRot, m_RotSpeed);
	}

	if (m_CanMove)
	{
		const Vector3 normDir = dir.Norm();
		float deltaTimeMoveSpeed = m_MoveSpeed * m_SpeedScale * Time::DeltaTimeLapseRate();	// ���Ԍo�ߗ���K�������ړ����x

		Vector3 velocity = normDir * deltaTimeMoveSpeed * V3::HORIZONTAL;	// �X�e�B�b�N�̌X���̕����ւ̑��x

		// ���x��K��������
		m_pPhysics->velocity.x = velocity.x;
		m_pPhysics->velocity.z = velocity.z;
	}
}

void CharaBase::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
}

void CharaBase::Block()
{
	m_IsBlocking = true;
}

void CharaBase::Attack()
{
	m_IsAttack = true;
}

//========================================================================

void CharaBase::StateIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("Idle");
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
				m_FSM->ChangeState(&CharaBase::StateIdleEmote_CheckSword); // �X�e�[�g��ύX
			}
			else
			{
				m_FSM->ChangeState(&CharaBase::StateIdleEmote_Stretch); // �X�e�[�g��ύX
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

void CharaBase::StateIdleEmote_CheckSword(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("IdleEmote_CheckSword");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateIdle); // �X�e�[�g��ύX
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

void CharaBase::StateIdleEmote_Stretch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("IdleEmote_Stretch");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateIdle); // �X�e�[�g��ύX
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

void CharaBase::StateIdleToBlock(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("IdleToBlock");
		SetMoveSpeed(90.0f);
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		blockUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateBlock); // �X�e�[�g��ύX
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
		m_Timeline->Play("Run");
		SetMoveSpeed(700.0f);
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

void CharaBase::StateHeavyAttack1(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("AirSpin");// �_�E�����[�h�������I
		m_Animator->PlaySub("mixamorig:Spine", "Stab");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_IsAttack)
		{
			m_FSM->ChangeState(&CharaBase::StateIdleToBlock); // �X�e�[�g��ύX
		}
		else if (m_Animator->IsFinishedSub("mixamorig:Spine"))
		{
			m_FSM->ChangeState(&CharaBase::StateIdle);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Animator->StopSub("mixamorig:Spine");
	}
	break;
	}
}

void CharaBase::StateBlock(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("Block");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		blockUpdate();

		if (not m_IsBlocking)
		{
			m_FSM->ChangeState(&CharaBase::StateBlockToIdle); // �X�e�[�g��ύX
		}
		else if (m_IsMove)
		{
			m_FSM->ChangeState(&CharaBase::StateBlockWalk);
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

void CharaBase::StateBlockToIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->Play("BlockToIdle");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		idleUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateIdle); // �X�e�[�g��ύX
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

void CharaBase::StateBlockWalk(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->PlaySub("mixamorig:Spine", "Block");
		m_Timeline->Play("BlockWalk");
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		blockUpdate();

		if (not m_IsBlocking)
		{
			m_FSM->ChangeState(&CharaBase::StateBlockToIdle); // �X�e�[�g��ύX
		}
		else if (not m_IsMove)
		{
			m_FSM->ChangeState(&CharaBase::StateBlock);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		m_Animator->StopSub("mixamorig:Spine");
		m_Timeline->Stop();
	}
	break;
	}
}

void CharaBase::SubStateNone(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
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
	}
	break;
	}
}

void CharaBase::SubStateFall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		m_Animator->PlaySub("mixamorig:LeftUpLeg", "Fall");
		m_Animator->PlaySub("mixamorig:RightUpLeg", "Fall");
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
		m_Animator->StopSub("mixamorig:LeftUpLeg");
		m_Animator->StopSub("mixamorig:RightUpLeg");
	}
	break;
	}
}

//========================================================================

void CharaBase::idleUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		//m_FSM->ChangeState(&CharaBase::StateActionIdleToJump); // �X�e�[�g��ύX
	}
	if (m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateRun); // �X�e�[�g��ύX
	}
	if (m_IsBlocking)
	{
		m_FSM->ChangeState(&CharaBase::StateIdleToBlock);
	}
	canAttackUpdate();
}

void CharaBase::runUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		//m_FSM->ChangeState(&CharaBase::StateRunToJump); // �X�e�[�g��ύX
	}
	if (not m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateIdle); // �X�e�[�g��ύX
	}
	if (m_SlideTimer > 0.0f)
	{
		//m_FSM->ChangeState(&CharaBase::StateRunToSlide); // �X�e�[�g��ύX
	}
	if (m_IsBlocking)
	{
		m_FSM->ChangeState(&CharaBase::StateIdleToBlock);
	}
	canAttackUpdate();
}

void CharaBase::blockUpdate()
{
}

void CharaBase::canAttackUpdate()
{
	if (m_IsAttack)
	{
		m_FSM->ChangeState(&CharaBase::StateHeavyAttack1);
	}
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
	//m_FSM->ChangeState(&CharaBase::StateRoll); // �X�e�[�g��ύX
}

void CharaBase::endRoll(const nlohmann::json& argument)
{
	if (m_IsMove)
	{
		//m_FSM->ChangeState(&CharaBase::StateRollToRun); // �X�e�[�g��ύX
	}
	else
	{
		//m_FSM->ChangeState(&CharaBase::StateRollToActionIdle); // �X�e�[�g��ύX
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
