#include "src/scene/play/chara/Chara.h"
#include "src/scene/play/ui/UI_Fade.h"
#include "src/scene/play/ball/BallTarget.h"

void Chara::RespawnStateNone(FSMSignal sig)
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

void Chara::RespawnStateFadeOut(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		transform->scale = Vector3(0.5f);
		if (m_pUI_Fade)
			m_pUI_Fade->StartFadeOut();

		m_pBallTarget->SetCanRockOn(false);
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (not m_pUI_Fade)
			m_RespawnFSM->ChangeState(&Chara::RespawnStateFadeIn);
		else if (m_pUI_Fade->IsFadeEnd())
		{
			m_RespawnFSM->ChangeState(&Chara::RespawnStateFadeIn);
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

		m_pBallTarget->SetCanRockOn(true);
	}
	break;
	}
}

void Chara::RespawnStateFadeIn(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		transform->scale = Vector3(1.0f);

		if (m_pUI_Fade)
			m_pUI_Fade->StartFadeIn();
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (not m_pUI_Fade)
			m_RespawnFSM->ChangeState(&Chara::RespawnStateNone);
		else if (m_pUI_Fade->IsFadeEnd())
		{
			m_RespawnFSM->ChangeState(&Chara::RespawnStateNone);
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