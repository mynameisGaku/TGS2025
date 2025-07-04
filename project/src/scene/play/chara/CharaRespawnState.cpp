#include "src/scene/play/chara/CharaBase.h"

void CharaBase::RespawnStateNone(FSMSignal sig)
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

void CharaBase::RespawnStateFadeOut(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		transform->scale = Vector3(0.5f);
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		m_RespawnFSM->ChangeState(&CharaBase::RespawnStateFadeIn);
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V
	{
	}
	break;
	case FSMSignal::SIG_Exit: // �I��
	{
		if (m_SpawnPointManager == nullptr)
		{
			respawn(Vector3::Zero, Vector3::Zero);
		}
		else
		{
			respawnByPoint();
		}
	}
	break;
	}
}

void CharaBase::RespawnStateFadeIn(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		transform->scale = Vector3(1.0f);
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		m_RespawnFSM->ChangeState(&CharaBase::RespawnStateNone);
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