#include "src/scene/play/chara/Chara.h"
#include "src/scene/play/ui/UI_Fade.h"

void Chara::RespawnStateNone(FSMSignal sig)
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

void Chara::RespawnStateFadeOut(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		transform->scale = Vector3(0.5f);
		m_pUI_Fade->StartFadeOut();
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_pUI_Fade->IsFadeEnd())
		{
			m_RespawnFSM->ChangeState(&Chara::RespawnStateFadeIn);
		}
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

void Chara::RespawnStateFadeIn(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // �J�n
	{
		transform->scale = Vector3(1.0f);
		m_pUI_Fade->StartFadeIn();
	}
	break;
	case FSMSignal::SIG_Update: // �X�V
	{
		if (m_pUI_Fade->IsFadeEnd())
		{
			m_RespawnFSM->ChangeState(&Chara::RespawnStateNone);
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