#pragma once

// ���p����
#include "Component.h"

#include <string>
#include "../TinyFSM.h"

class CharaBase;

class AIController : public Component {
public:
	//=======================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	AIController();
	~AIController();

	//=======================================================================================
	// ���e��֐�

	void Reset();
	void Update() override;

	/// <summary>
	/// <para>�X�e�[�g��ύX���鏈��</para>
	/// <para>�����̐擪��"��"������̂�Y��Ȃ��ł��������B</para>
	/// </summary>
	/// <param name="state">�X�e�[�g�̊֐��|�C���^</param>
	void ChangeState(void(AIController::* state)(FSMSignal));

	void IdleState(FSMSignal sig);
	void MoveState(FSMSignal sig);
	void AttackState(FSMSignal sig);

	//=======================================================================================
	// ���Z�b�^�[

	//=======================================================================================
	// ���Q�b�^�[

private:
	CharaBase* m_Chara;	// �ۗL��(CharaBase)

	bool m_DoMove;	// �ړ����邩�ǂ���
	bool m_DoAttack;	// �U�����邩�ǂ���

	float m_ActionTimer;	// �s���؂�ւ��^�C�}�[

	TinyFSM<AIController>* m_FSM;
};
