#pragma once

// ◇継承元
#include "Component.h"

#include <string>
#include "../TinyFSM.h"

class CharaBase;

class AIController : public Component {
public:
	//=======================================================================================
	// ▼コンストラクタ・デストラクタ

	AIController();
	~AIController();

	//=======================================================================================
	// ▼各種関数

	void Reset();
	void Update() override;

	/// <summary>
	/// <para>ステートを変更する処理</para>
	/// <para>引数の先頭に"＆"をつけるのを忘れないでください。</para>
	/// </summary>
	/// <param name="state">ステートの関数ポインタ</param>
	void ChangeState(void(AIController::* state)(FSMSignal));

	void IdleState(FSMSignal sig);
	void MoveState(FSMSignal sig);
	void AttackState(FSMSignal sig);

	//=======================================================================================
	// ▼セッター

	//=======================================================================================
	// ▼ゲッター

private:
	CharaBase* m_Chara;	// 保有者(CharaBase)

	bool m_DoMove;	// 移動するかどうか
	bool m_DoAttack;	// 攻撃するかどうか

	float m_ActionTimer;	// 行動切り替えタイマー

	TinyFSM<AIController>* m_FSM;
};
