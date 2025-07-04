#include "src/scene/play/chara/CharaBase.h"
#include "src/scene/play/ui/UI_Fade.h"

void CharaBase::RespawnStateNone(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::RespawnStateFadeOut(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		transform->scale = Vector3(0.5f);
		m_pUI_Fade->StartFadeOut();
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_pUI_Fade->IsFadeEnd())
		{
			m_RespawnFSM->ChangeState(&CharaBase::RespawnStateFadeIn);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
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
	case FSMSignal::SIG_Enter: // 開始
	{
		transform->scale = Vector3(1.0f);
		m_pUI_Fade->StartFadeIn();
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_pUI_Fade->IsFadeEnd())
		{
			m_RespawnFSM->ChangeState(&CharaBase::RespawnStateNone);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}