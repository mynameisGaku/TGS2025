#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include <string>
#include "src/util/time/GameTime.h"
#include "src/util/fsm/TinyFSM.h"
#include <vendor/nlohmann/json_fwd.hpp>

#include "src/util/math/Vector2.h"
#include "src/util/alarm/Alarm.h"

class CharaHP;
class CharaStamina;
class Ball;
class BallManager;
class Physics;
class Catcher;
class Tackler;
class Animator;
template <class C>
class Timeline;
class StatusTracker;
class EffectBase;
class Trail3D;

class UI_CrossHair;
class UI_HitPoint_Icon;

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
	/// �{�[���𐶐�����B�f�o�b�O�p�B
	/// </summary>
	void GenerateBall();

	void SetBall(Ball* ball);

	void StartBallCharge();

	void StartThrow();

	/// <summary>
	/// �����郂�[�V���������Đ�
	/// </summary>
	void Feint();

	/// <summary>
	/// �{�[���������Ă��邩�H
	/// </summary>
	/// <returns></returns>
	bool IsHoldingBall() const { return nullptr != m_pBall; }

	/// <summary>
	/// �`���[�W�����H
	/// </summary>
	/// <returns></returns>
	bool IsChargingBall() const { return m_IsCharging; }

	Ball* LastBall() const { return m_pLastBall; }
	void SetLastBall(Ball* ball) { m_pLastBall = ball; }
	Ball* HitBall() const { return m_pHitBall; }

	/// <summary>
	/// �Ō�ɓ������{�[���Ƀe���|�[�g���A������L���b�`����
	/// </summary>
	void TeleportToLastBall();

	void DropBall(const Vector3& other, float force_vertical, float force_horizontal);

	//=======================================================================================
	// ���L���b�`

	/// <summary>
	/// ���̊֐����Ăяo���Ă���ԁA�z���L���b�`���s����B
	/// </summary>
	void Catch();

	void CatchSuccess(const Vector3& velocity);

	//=======================================================================================
	// ���^�b�N��
	void Tackle();

	void GetTackle(const Vector3& other, float force_horizontal, float force_vertical, bool isForceKnockback);

	void Knockback(const Vector3& other, float force_vertical, float force_horizontal);

	//=======================================================================================
	// �����G
	void SetInvincible(float duration_sec, bool isOverride);
private:
	void invincibleUpdate();
public:
	//=======================================================================================
	// �����X�|�[��
	
	/// <summary>
	/// ���̊֐����Ăяo���ƁA�p�����[�^�����Z�b�g����A�w��ʒu�Ƀ��X�|�[������B
	/// ToDo : ���X�|�[���X�e�[�g�����
	/// </summary>
	/// <param name="pos">���X�|�[�����ʒu</param>
	/// <param name="rot">���X�|�[������]</param>
	void Respawn(const Vector3& pos, const Vector3& rot);

	void SetTrailImage(int hImage);

	//=======================================================================================
	// �����o

	Vector2 Target(const Ball* ball);

	//=======================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// HP���擾����
	/// </summary>
	inline const CharaHP* GetHP() const { return m_pHP; }

	/// <summary>
	/// �C���f�b�N�X���擾
	/// </summary>
	inline const int GetIndex() const { return m_Index; }

	/// <summary>
	/// �L�����^�O(�����`�[����)���擾
	/// </summary>
	inline const std::string GetCharaTag() const { return m_CharaTag; }

	/// <summary>
	/// �X�e�[�^�X�g���b�J�[���擾
	/// </summary>
	inline StatusTracker* GetStatusTracker() const { return m_pStatusTracker; }

	inline Ball* GetHaveBall() const { return m_pBall; }
	inline const Ball* GetTargetBall() const { return m_pTargetBall; }

	// �{�[�����`���[�W���Ă��邩�ǂ���
	inline bool	IsCharging()	const { return m_IsCharging;}			
	// ���n��
	inline bool	IsLanding()		const { return m_IsLanding;}			
	// �ړ��\��
	inline bool	CanMove()		const { return m_CanMove;}				
	// ��]�\��
	inline bool	CanRot()		const { return m_CanRot;}				
	// �ړ����悤�Ƃ��Ă��邩
	inline bool	IsMove()		const { return m_IsMove;}				
	// �W�����v����
	inline bool	IsJumping()		const { return m_IsJumping;}			
	// �L���b�`�\��
	inline bool	CanCatch()		const { return m_CanCatch;}				
	// �{�[�������Ă邩
	inline bool	CanHold()		const { return m_CanHold;}				
	// �{�[���𓊂����邩
	inline bool	CanThrow()		const { return m_CanThrow;}				
	// �L���b�`����
	inline bool	IsCatching()	const { return m_IsCatching;}	
	// �^�[�Q�b�g��_���Ă��邩
	inline bool IsTargeting() const { return m_IsTargeting; }
	// �^�[�Q�b�g����Ă��邩
	inline bool IsTargeted() const { return m_IsTargeted; }
	// �^�b�N������
	inline bool IsTackling() const { return m_IsTackling; }
	// �^�b�N���\��
	inline bool CanTackle() const { return m_CanTackle; }
	// ���G����
	inline bool IsInvincible() const { return m_IsInvincible; }

	//=======================================================================================
	// ���e�X�e�[�g
	void StateActionIdle(FSMSignal sig);
	void StateActionIdleEmote(FSMSignal sig);
	void StateActionIdleToJump(FSMSignal sig);
	void StateActionIdleToRun(FSMSignal sig);
	void StateActionIdleToStandingIdle(FSMSignal sig);
	void StateActionIdleToTackle(FSMSignal sig);

	void StateAimToThrow(FSMSignal sig);

	void StateAirSpin(FSMSignal sig);

	void StateCrouchToActionIdle(FSMSignal sig);
	void StateCrouchToRun(FSMSignal sig);

	void StateDamageToDown(FSMSignal sig);

	void StateFall(FSMSignal sig);
	void StateFallToCrouch(FSMSignal sig);
	void StateFallToRoll(FSMSignal sig);

	void StateFeint(FSMSignal sig);

	void StateRoll(FSMSignal sig);
	void StateRollToActionIdle(FSMSignal sig);
	void StateRollToRun(FSMSignal sig);

	void StateRun(FSMSignal sig);
	void StateRunToActionIdle(FSMSignal sig);
	void StateRunToJump(FSMSignal sig);
	void StateRunToSlide(FSMSignal sig);
	void StateRunToTackle(FSMSignal sig);

	void StateSlide(FSMSignal sig);
	void StateSlideToRun(FSMSignal sig);

	void StateStandingIdle(FSMSignal sig);
	void StateStandingIdleEmote(FSMSignal sig);
	void StateStandingIdleToActionIdle(FSMSignal sig);

	void StateTackle(FSMSignal sig);

	void SubStateNone(FSMSignal sig);
	void SubStateGetBall(FSMSignal sig);
	void SubStateHold(FSMSignal sig);
	void SubStateHoldToAim(FSMSignal sig);
	void SubStateCatch(FSMSignal sig);

private:
	friend class CharaManager;
	friend class UI_CrossHair;


	Ball*			m_pBall;				// ���L���Ă���{�[���̃|�C���^�[
	Ball*			m_pLastBall;			// �Ō�ɓ������{�[���̃|�C���^�[
	Ball*			m_pHitBall;				// ���Ă�ꂽ�{�[���̃|�C���^�[
	const Ball*		m_pTargetBall;			// �_���Ă���{�[���̃|�C���^�[
	BallManager*	m_pBallManager;			// �{�[���}�l�[�W���[�̃|�C���^�[
	CharaStamina*	m_pStamina;				// �X�^�~�i�̃|�C���^�[
	CharaHP*		m_pHP;					// HP�̃|�C���^�[
	Physics*		m_pPhysics;				// ���������̃|�C���^�[
	std::string		m_CharaTag;				// �L�����N�^�[�̃`�[���̃^�O
	Catcher*		m_Catcher;				// �L���b�`�̓����蔻��
	Tackler*		m_Tackler;				// �^�b�N���̓����蔻��
	EffectBase*		m_pCatchReadyEffect;	// �L���b�`�̏����G�t�F�N�g
	EffectBase*		m_pCatchDustEffect;		// �L���b�`�̕��G�t�F�N�g
	TinyFSM<CharaBase>* m_FSM;				// �X�e�[�g�}�V��
	TinyFSM<CharaBase>* m_SubFSM;			// �X�e�[�g�}�V��
	Animator*		m_Animator;				// �A�j���[�V����
	Transform*		m_EffectTransform;		// �G�t�F�N�g�o���g�����X�t�H�[��
	Timeline<CharaBase>* m_Timeline;		// �A�j���[�V�����ɍ��킹�ē����^�C�����C��
	StatusTracker*	m_pStatusTracker;		// �X�e�[�^�X�̓��v
	Alarm*			m_Alarm;				// �A���[��
	int				m_hTrailImage;			// �g���C���̉摜�n���h��
	int				m_Index;				// ���g�̃C���f�b�N�X
	float			m_HitPoint;
	float			m_BallChargeRate;		// �{�[���̃`���[�W�����x
	float			m_MoveSpeed;			// �ړ����x
	float			m_RotSpeed;				// ��]���x
	float			m_SpeedScale;			// ���x�{��
	float			m_EmoteTimer;			// ���u�A�j���[�V�����܂ł̎���
	float			m_SlideTimer;			// �X���C�f�B���O�c�莞�ԃ^�C�}�[
	float			m_CatchTimer;			// �L���b�`�̎c�莞�ԃ^�C�}�[
	float			m_InvincibleTimer;		// ���G�c�莞��
	float			m_Stamina;				// 
	bool			m_IsCharging;			// �{�[�����`���[�W���Ă��邩�ǂ���
	bool			m_IsLanding;			// ���n��
	bool			m_CanMove;				// �ړ��\��
	bool			m_CanRot;				// ��]�\��
	bool			m_IsMove;				// �ړ����悤�Ƃ��Ă��邩
	bool			m_IsJumping;			// �W�����v����
	bool			m_CanCatch;				// �L���b�`�\��
	bool			m_CanHold;				// �{�[�������Ă邩
	bool			m_CanThrow;				// �{�[���𓊂����邩
	bool			m_IsCatching;			// �L���b�`����
	bool 			m_IsTargeting;			// �^�[�Q�b�g��_���Ă��邩
	bool 			m_IsTargeted;			// �^�[�Q�b�g����Ă��邩
	bool			m_CanTackle;			// �^�b�N���\��
	bool			m_IsTackling;			// �^�b�N������
	bool			m_IsInvincible;			// ���G��
	bool			m_IsDamage;				// �_���[�W��炢����

	UI_CrossHair* m_UI_CrossHair;			// �N���X�w�A��UI
	UI_CrossHair* m_UI_BallChargeMeter;		// �{�[���`���[�W�ʂ�UI
	UI_HitPoint_Icon* m_UI_HitPointIcon;	// �̗͂�UI

	void land();

	void idleUpdate();
	void runUpdate();
	void slideUpdate();
	void catchUpdate();
	void jumpUpdate();
	void tackleUpdate();

	void getHit(Ball* hit);

	// �L�����̐��ʂɕ����w��{�[���𓊂���
	void throwBallForward();
	// �z�[�~���O�{�[���𓊂���
	void throwBallHoming();
	// �{�[�������������
	void releaseBall();

	//=== �T�E���h�Đ� ===
	void playThrowSound();
	void playGetHitSound();
	void playFootStepSound();
	void playCatchBallSound();
	void playPickupBallSound();
	void playVacuumSound();

	//=== �^�C�����C���p ===
	void setAnimationSpeed(const nlohmann::json& argument);
	void moveToPosition(const nlohmann::json& argument);
	void changeToRoll(const nlohmann::json& argument);
	void endRoll(const nlohmann::json& argument);
	void setCanMove(const nlohmann::json& argument);
	void setCanRot(const nlohmann::json& argument);
	void setVelocity(const nlohmann::json& argument);
	void throwBall(const nlohmann::json& argument);
};