#include "AIController.h"
#include "../CharaBase.h"
#include "../Util/Utils.h"

namespace
{
	static const float CHANGE_STATE_TIME = 2.0f;	// �X�e�[�g�ύX����
}

AIController::AIController()
{
	m_Chara = Parent<CharaBase>();

	m_FSM = new TinyFSM<AIController>(this);
	m_FSM->RegisterStateName(&AIController::IdleState, "IdleState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_FSM->RegisterStateName(&AIController::MoveState, "MoveState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_FSM->RegisterStateName(&AIController::AttackState, "AttackState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B

	Reset();
}

AIController::~AIController()
{
	Function::DeletePointer(m_FSM);
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

	// �s���؂�ւ��^�C�}�[
	m_ActionTimer += Time::DeltaTimeLapseRate();
	// �s���؂�ւ�
	if (m_ActionTimer >= CHANGE_STATE_TIME)
	{
		float rand = GetRand(10000) / 10000.0f; // �����擾
		if (rand < 0.5f)
		{
			// �ړ�
			m_FSM->ChangeState(&AIController::MoveState);
		}
		else if (rand < 0.8f)
		{
			// �U��
			m_FSM->ChangeState(&AIController::AttackState);
		}
		else
		{
			// �ҋ@
			m_FSM->ChangeState(&AIController::IdleState);
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
