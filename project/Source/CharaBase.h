#pragma once
#include "Object3D.h"
#include "Component/ColliderCapsule.h"
#include <string>
#include <Library/time.h>
#include "TinyFSM.h"
#include "nlohmann/json_fwd.hpp"

class CharaHP;
class CharaStamina;
class Physics;
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

	void Block();

	//=======================================================================================
	// ���e�X�e�[�g
	void StateBlock(FSMSignal sig);
	void StateBlockToIdle(FSMSignal sig);
	void StateBlockWalk(FSMSignal sig);
	void StateIdle(FSMSignal sig);
	void StateIdleEmote_CheckSword(FSMSignal sig);
	void StateIdleEmote_Stretch(FSMSignal sig);
	void StateIdleToBlock(FSMSignal sig);
	void StateRun(FSMSignal sig);
	void SubStateNone(FSMSignal sig);
	void SubStateFall(FSMSignal sig);

private:
	friend class CharaManager;
	CharaStamina*	m_pStamina;				// �X�^�~�i�̃|�C���^�[
	CharaHP*		m_pHP;					// HP�̃|�C���^�[
	Physics*		m_pPhysics;				// ���������̃|�C���^�[
	float			m_MoveSpeed;			// �ړ����x
	float			m_RotSpeed;				// ��]���x
	float			m_SpeedScale;			// ���x�{��
	int				m_Index;				// ���g�̃C���f�b�N�X
	std::string		m_CharaTag;				// �L�����N�^�[�̃`�[���̃^�O
	TinyFSM<CharaBase>* m_FSM;				// �X�e�[�g�}�V��
	TinyFSM<CharaBase>* m_SubFSM;			// �X�e�[�g�}�V��
	Animator*		m_Animator;				// �A�j���[�V����
	float			m_EmoteTimer;			// ���u�A�j���[�V�����܂ł̎���
	bool			m_IsLanding;			// ���n��
	float			m_SlideTimer;			// �X���C�f�B���O�c�莞�ԃ^�C�}�[
	Transform*		m_EffectTransform;		// �G�t�F�N�g�o���g�����X�t�H�[��
	Timeline<CharaBase>* m_Timeline;		// �A�j���[�V�����ɍ��킹�ē����^�C�����C��
	bool			m_CanMove;				// �ړ��\��
	bool			m_CanRot;				// ��]�\��
	bool			m_IsMove;				// �ړ����悤�Ƃ��Ă��邩
	bool			m_IsBlocking;			// �u���b�N��

	void idleUpdate();
	void runUpdate();
	void blockUpdate();
	
	//=== �^�C�����C���p ===
	void setAnimationSpeed(const nlohmann::json& argument);
	void moveToPosition(const nlohmann::json& argument);
	void changeToRoll(const nlohmann::json& argument);
	void endRoll(const nlohmann::json& argument);
	void setCanMove(const nlohmann::json& argument);
	void setCanRot(const nlohmann::json& argument);
	void setVelocity(const nlohmann::json& argument);
};