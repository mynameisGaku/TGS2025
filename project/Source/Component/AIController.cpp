#include "AIController.h"
#include "../CharaBase.h"
#include "../Util/Utils.h"

namespace
{
	static const float CHANGE_STATE_TIME = 2.0f;	// ステート変更時間
}

AIController::AIController()
{
	m_Chara = Parent<CharaBase>();

	m_FSM = new TinyFSM<AIController>(this);
	m_FSM->RegisterStateName(&AIController::IdleState, "IdleState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&AIController::MoveState, "MoveState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&AIController::AttackState, "AttackState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。

	Reset();
}

AIController::~AIController()
{
	Function::DeletePointer(m_FSM);
}

void AIController::Reset()
{
	m_FSM->ChangeState(&AIController::IdleState); // ステートを変更
	m_DoMove = true;
	m_DoAttack = true;
	m_ActionTimer = 0.0f;
}

void AIController::Update()
{
	m_FSM->Update();

	// 行動切り替えタイマー
	m_ActionTimer += Time::DeltaTimeLapseRate();
	// 行動切り替え
	if (m_ActionTimer >= CHANGE_STATE_TIME)
	{
		float rand = GetRand(10000) / 10000.0f; // 乱数取得
		if (rand < 0.5f)
		{
			// 移動
			m_FSM->ChangeState(&AIController::MoveState);
		}
		else if (rand < 0.8f)
		{
			// 攻撃
			m_FSM->ChangeState(&AIController::AttackState);
		}
		else
		{
			// 待機
			m_FSM->ChangeState(&AIController::IdleState);
		}

		m_ActionTimer = 0.0f; // タイマーリセット
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
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
	}
	break;
	}
}

void AIController::MoveState(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
	}
	break;
	}
}

void AIController::AttackState(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
	}
	break;
	}
}
