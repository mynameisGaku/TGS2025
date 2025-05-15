#pragma once
#include "Object3D.h"
#include "Component/ColliderCapsule.h"
#include <string>
#include <Library/time.h>
#include "TinyFSM.h"
#include "nlohmann/json_fwd.hpp"

class CharaHP;
class CharaStamina;
class Ball;
class BallManager;
class Physics;
class Catcher;
class Animator;
template <class C>
class Timeline;

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

	/// <summary>
	/// ���̗͂ŃW�����v
	/// </summary>
	void Jump();

	/// <summary>
	/// �Ăяo�����X���C�f�B���O����
	/// </summary>
	void Slide();

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
    /// �z�[�~���O�{�[���𓊂���
    /// </summary>
    /// <param name="param">�z�[�~���O�T�v</param>
    void ThrowHomingBall();

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

	Ball* LastBall() const { return m_pLastBall; }
	void SetLastBall(Ball* ball) { m_pLastBall = ball; }

	/// <summary>
	/// �Ō�ɓ������{�[���Ƀe���|�[�g���A������L���b�`����
	/// </summary>
	void TeleportToLastBall();

	//=======================================================================================
	// ���L���b�`

	/// <summary>
	/// ���̊֐����Ăяo���Ă���ԁA�z���L���b�`���s����B
	/// </summary>
	void Catch();

	//=======================================================================================
	// ���e�X�e�[�g
	void StateActionIdle(FSMSignal sig);
	void StateActionIdleEmote(FSMSignal sig);
	void StateActionIdleToJump(FSMSignal sig);
	void StateActionIdleToRun(FSMSignal sig);
	void StateActionIdleToStandingIdle(FSMSignal sig);

	void StateAirSpin(FSMSignal sig);

	void StateCatch(FSMSignal sig);

	void StateCrouchToActionIdle(FSMSignal sig);
	void StateCrouchToRun(FSMSignal sig);

	void StateDamageToDown(FSMSignal sig);

	void StateFall(FSMSignal sig);
	void StateFallToCrouch(FSMSignal sig);
	void StateFallToRoll(FSMSignal sig);

	void StateGetBall(FSMSignal sig);

	void StateRoll(FSMSignal sig);
	void StateRollToActionIdle(FSMSignal sig);
	void StateRollToRun(FSMSignal sig);

	void StateRun(FSMSignal sig);
	void StateRunToActionIdle(FSMSignal sig);
	void StateRunToJump(FSMSignal sig);
	void StateRunToSlide(FSMSignal sig);

	void StateSlide(FSMSignal sig);
	void StateSlideToRun(FSMSignal sig);

	void StateStandingIdle(FSMSignal sig);
	void StateStandingIdleEmote(FSMSignal sig);
	void StateStandingIdleToActionIdle(FSMSignal sig);

	void StateAimToThrow(FSMSignal sig);

private:
	friend class CharaManager;
	bool			m_IsCharging;			// �{�[�����`���[�W���Ă��邩�ǂ���
	float			m_BallChargeRate;		// �{�[���̃`���[�W�����x
	float			m_ChargeRateWatchDog;	// �`���[�W�I�����牽�b�o���������Ď�����Ԍ�
	Ball*			m_pBall;				// ���L���Ă���{�[���̃|�C���^�[
	Ball*			m_pLastBall;			// �Ō�ɓ������{�[���̃|�C���^�[
    BallManager*	m_pBallManager;			// �{�[���}�l�[�W���[�̃|�C���^�[
	CharaStamina*	m_pStamina;				// �X�^�~�i�̃|�C���^�[
	CharaHP*		m_pHP;					// HP�̃|�C���^�[
	Physics*		m_pPhysics;				// ���������̃|�C���^�[
	float			m_MoveSpeed;			// �ړ����x
	float			m_RotSpeed;				// ��]���x
	int				m_Index;				// ���g�̃C���f�b�N�X
	float			m_CatchTimer;			// �L���b�`�c�莞�ԃ^�C�}�[
	std::string		m_CharaTag;				// �L�����N�^�[�̃`�[���̃^�O
	Catcher*		m_Catcher;				// �L���b�`�̓����蔻��
	TinyFSM<CharaBase>* m_FSM;				// �X�e�[�g�}�V��
	Animator*		m_Animator;				// �A�j���[�V����
	float			m_EmoteTimer;			// ���u�A�j���[�V�����܂ł̎���
	bool			m_IsLanding;			// ���n��
	float			m_SlideTimer;			// �X���C�f�B���O�c�莞�ԃ^�C�}�[
	Transform*		m_EffectTransform;		// �G�t�F�N�g�o���g�����X�t�H�[��
	Timeline<CharaBase>* m_Timeline;		// �A�j���[�V�����ɍ��킹�ē����^�C�����C��

	void idleUpdate();
	void runUpdate();
	void slideUpdate();

	void getHit(Ball* hit);
	
	//=== �^�C�����C���p ===
	void setAnimationSpeed(const nlohmann::json& argument);

	void moveToPosition(const nlohmann::json& argument);
};