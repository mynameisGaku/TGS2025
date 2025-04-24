#pragma once
#include "Object3D.h"
#include "Component/ColliderCapsule.h"
#include <string>
#include <Library/time.h>
#include "TinyFSM.h"

class CharaStamina;
class Ball;
class Physics;
class Catcher;

/// <summary>
/// �L�����N�^�[�Ɋւ�����N���X
/// </summary>
class CharaBase : public Object3D
{
public:
	CharaBase();
	virtual ~CharaBase();

	void Init(std::string tag);
	void Update() override;
	void Draw() override;

	//=======================================================================================
	// �������蔻��

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData"></param>
	void CollisionEvent(const CollisionData& colData) override;

	/// <summary>
	/// �n�ʂƂ̓����蔻��
	/// </summary>
	void HitGroundProcess();

	//=======================================================================================
	// ���ړ�

	/// <summary>
	/// �ړ����x��ݒ肷��
	/// </summary>
	/// <param name="moveSpeed">�ړ����x</param>
	inline void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }

	/// <summary>
	/// ��]���x��ݒ肷��
	/// </summary>
	/// <param name="rotSpeed">��]���x</param>
	inline void SetRotSpeed(float rotSpeed) { m_RotSpeed = rotSpeed; }

	/// <summary>
	/// �w������Ɉړ�����
	/// </summary>
	/// <param name="dir">�ړ�����</param>
	void Move(const Vector3& dir);

	//=======================================================================================
	// ���{�[��

	/// <summary>
	/// �{�[���𓊂���
	/// </summary>
	/// <param name="velocity"></param>
	void ThrowBall(const Vector3& velocity);

	/// <summary>
	/// �{�[����O���ɓ�����
	/// </summary>
	/// <param name="speed"></param>
	void ThrowBallForward();

	/// <summary>
	/// �{�[���𐶐�����B
	/// ���̊֐����Ăяo���Ă���ԁA�{�[���̃`���[�W�����オ��B
	/// </summary>
	void GenerateBall();

	/// <summary>
	/// �{�[���������Ă��邩�H
	/// </summary>
	/// <returns></returns>
	bool IsHoldingBall() const { return nullptr != m_pBall; }

	/// <summary>
	/// �`���[�W�����H
	/// </summary>
	/// <returns></returns>
	bool IsChargingBall() const { return not m_IsCharging; }

	//=======================================================================================
	// ���L���b�`

	/// <summary>
	/// ���̊֐����Ăяo���Ă���ԁA�z���L���b�`���s����B
	/// </summary>
	void Catch();

	//=======================================================================================
	// ���e�X�e�[�g
	
	void IdleState(FSMSignal sig);
	void MoveState(FSMSignal sig);
	void ThrowState(FSMSignal sig);
	void CatchState(FSMSignal sig);

private:
	friend class CharaManager;
	bool			m_IsCharging;			// �{�[�����`���[�W���Ă��邩�ǂ���
	float			m_BallChargeRate;		// �{�[���̃`���[�W�����x
	float			m_ChargeRateWatchDog;	// �`���[�W�I�����牽�b�o���������Ď�����Ԍ�
	Ball*			m_pBall;				// ���L���Ă���{�[���̃|�C���^�[
	CharaStamina*	m_pStamina;				// �X�^�~�i�̃|�C���^�[
	Physics*		m_pPhysics;				// ���������̃|�C���^�[
	float			m_MoveSpeed;			// �ړ����x
	float			m_RotSpeed;				// ��]���x
	int				m_Index;				// ���g�̃C���f�b�N�X
	float			m_CatchTimer;			// �L���b�`�c�莞�ԃ^�C�}�[
	std::string		m_CharaTag;				// �L�����N�^�[�̃`�[���̃^�O
	Catcher*		m_Catcher;				// �L���b�`�̓����蔻��
	TinyFSM<AIController>* m_FSM;
};