#include "src/common/component/controller/AIController.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/util/utils.h"
#include "src/util/input/InputManager.h"

namespace
{
	static const float CHANGE_STATE_TIME = 2.0f;	// ステート変更時間
}

AIController::AIController()
{
	m_FSM = new TinyFSM<AIController>(this);
	m_FSM->RegisterStateName(&AIController::IdleState, "IdleState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&AIController::MoveState, "MoveState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&AIController::AttackState, "AttackState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&AIController::CatchState, "CatchState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。

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
	m_FSM->ChangeState(&AIController::IdleState); // ステートを変更
	m_DoMove = true;
	m_DoAttack = true;
	m_ActionTimer = 0.0f;
}

void AIController::Update()
{
	m_FSM->Update();

	if (m_FSM != nullptr)
		m_FSM->ImGuiDebugRender();

	// デバッグ切り替え
	if (InputManager::Push(KeyDefine::KeyCode::Alpha1))
	{
		m_FSM->ChangeState(&AIController::IdleState);
		m_ActionTimer = 0.0f; // タイマーリセット
		return;
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha2))
	{
		m_FSM->ChangeState(&AIController::MoveState);
		m_ActionTimer = 0.0f; // タイマーリセット
		return;
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha3))
	{
		m_FSM->ChangeState(&AIController::AttackState);
		m_ActionTimer = 0.0f; // タイマーリセット
		return;
	}
	if (InputManager::Push(KeyDefine::KeyCode::Alpha4))
	{
		m_FSM->ChangeState(&AIController::CatchState);
		m_ActionTimer = 0.0f; // タイマーリセット
		return;
	}

	// 行動切り替えタイマー
	m_ActionTimer += GTime.deltaTime;
	// 行動切り替え
	if (m_ActionTimer >= CHANGE_STATE_TIME)
	{
		float rand = GetRand(10000) / 10000.0f; // 乱数取得
		if (rand < 0.25f)
		{
			ChangeState(&AIController::IdleState); // 移動
		}
		if (rand < 0.5f)
		{
			ChangeState(&AIController::MoveState); // 移動
		}
		else if (rand < 0.75f)
		{
			ChangeState(&AIController::AttackState); // 攻撃
		}
		else
		{
			ChangeState(&AIController::CatchState); // 捕まえる
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
		m_Chara->Move(Vector3(0, 2, 0));
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
		m_Chara->Move(Vector3(0, 0, 1));
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
		m_Chara->GenerateBall();
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		m_Chara->Move(Vector3(0, 0, -1));
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
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
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		m_Chara->Catch();
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
