#pragma once
#include "Object3D.h"
#include <Library/time.h>

class CharaStamina;
class Ball;

/// <summary>
/// �L�����N�^�[�Ɋւ�����N���X
/// </summary>
class CharaBase : public Object3D
{
public:

	CharaBase();
	virtual ~CharaBase();

	virtual void Update() override;

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData"></param>
	void CollisionEvent(const CollisionData& colData) override;

	/// <summary>
	/// �n�ʂƂ̓����蔻��
	/// </summary>
	void HitGroundProcess();

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

private:
	bool			m_IsCharging;			// �{�[�����`���[�W���Ă��邩�ǂ���
	float			m_BallChargeRate;		// �{�[���̃`���[�W�����x
	float			m_ChargeRateWatchDog;	// �`���[�W�I�����牽�b�o���������Ď�����Ԍ�
	Ball*			m_pBall;				// ���L���Ă���{�[���̃|�C���^�[
	CharaStamina*	m_pStamina;				// �X�^�~�i�̃|�C���^�[
};