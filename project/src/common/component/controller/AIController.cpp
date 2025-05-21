#include "src/common/component/controller/AIController.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/util/utils.h"
#include "src/util/input/InputManager.h"

namespace
{
	static const float CHANGE_STATE_TIME = 2.0f;	// �X�e�[�g�ύX����
}

AIController::AIController()
{
	m_FSM = new TinyFSM<AIController>(this);
	m_FSM->RegisterStateName(&AIController::IdleState, "IdleState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_FSM->RegisterStateName(&AIController::MoveState, "MoveState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_FSM->RegisterStateName(&AIController::AttackState, "AttackState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_FSM->RegisterStateName(&AIController::CatchState, "CatchState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B

	Reset();
}

AIController::~AIController()
{
	Function::DeletePointer(m_FSM);
}

void AIController::Init()
{
	m_Chara = Parent<CharaBase>();
}

void AIController::Reset()
{
	m_FSM->ChangeState(&AIController::IdleState); // �X�e�[�g��ύX
	m_DoMove = true;
	m_DoAttack = true;
	m_ActionTimer = 0.0f;
}

void AIController::Update()
{
	m_FSM->Update();

	if (m_FSM != nullptr)
		m_FSM->ImGuiDebugRender();

	// �f�o�b�O�؂�ւ�
	if (InputManager::Push(KeyDefine::KeyCode::Alpha1))
	{
		m_FSM->ChangeState(&AIController::IdleState);
		m_ActionTimer = 0.0f; // �^�C�}�[���Z�b�g
		return;
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha2))
	{
		m_FSM->ChangeState(&AIController::MoveState);
		m_ActionTimer = 0.0f; // �^�C�}�[���Z�b�g
		return;
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha3))
	{
		m_FSM->ChangeState(&AIController::AttackState);
		m_ActionTimer = 0.0f; // �^�C�}�[���Z�b�g
		return;
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha4))
	{
		m_FSM->ChangeState(&AIController::CatchState);
		m_ActionTimer = 0.0f; // �^�C�}�[���Z�b�g
		return;
	}

	// �s���؂�ւ��^�C�}�[
	m_ActionTimer += GTime.deltaTime;
	// �s���؂�ւ�
	if (m_ActionTimer >= CHANGE_STATE_TIME)
	{
		float rand = GetRand(10000) / 10000.0f; // �����擾
		if (rand < 0.25f)
		{
			ChangeState(&AIController::IdleState); // �ړ�
		}
		if (rand < 0.5f)
		{
			ChangeState(&AIController::MoveState); // �ړ�
		}
		else if (rand < 0.75f)
		{
			ChangeState(&AIController::AttackState); // �U��
		}
		else
		{
			ChangeState(&AIController::CatchState); // �߂܂���
		}

		m_ActionTimer = 0.0f; // �^�C�}�[���Z�b�g
	}
}

//========================================================================================

void AIController::ChangeState(void(AIController::* state)(FSMSignal))
{
	if (m_FSM == nullptr)
		return;

	m_FSM->ChangeState(state);
}

void AIController::IdleState(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
		m_Chara->Move(Vector3(0, 2, 0));
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I�� (Exit)
	{
	}
	break;
	}
}

void AIController::MoveState(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		m_Chara->Move(Vector3(0, 0, 1));
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I�� (Exit)
	{
	}
	break;
	}
}

void AIController::AttackState(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
		m_Chara->GenerateBall();
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		m_Chara->Move(Vector3(0, 0, -1));
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I�� (Exit)
	{
		m_Chara->ThrowBallForward();
	}
	break;
	}
}

void AIController::CatchState(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		m_Chara->Catch();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I�� (Exit)
	{
	}
	break;
	}
}
