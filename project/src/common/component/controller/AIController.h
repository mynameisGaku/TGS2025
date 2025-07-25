#pragma once

// ◇継承元
#include "src/common/component/Component.h"

#include <string>
#include "src/util/fsm/TinyFSM.h"

class Chara;

/// <summary>
/// キャラクターを操作するクラス（AIが操作する）
/// </summary>
/// <author>佐藤紘斗</author>
class AIController : public Component {
public:
	//=======================================================================================
	// ▼コンストラクタ・デストラクタ

	AIController();
	~AIController();

	//=======================================================================================
	// ▼各種関数

	void Init();
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
	void CatchState(FSMSignal sig);

	//=======================================================================================
	// ▼セッター

	//=======================================================================================
	// ▼ゲッター

private:
	Chara* m_Chara;	// 保有者(CharaBase)

	bool m_DoMove;	// 移動するかどうか
	bool m_DoAttack;	// 攻撃するかどうか

	float m_ActionTimer;	// 行動切り替えタイマー

	TinyFSM<AIController>* m_FSM;
};
