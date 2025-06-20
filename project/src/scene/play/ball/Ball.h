#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/renderer/BallRenderer.h"
#include <string>

class Physics;
class ColliderCapsule;
class CharaBase;
class Collider;
class BallManager;
class Trail3D;

namespace
{
	static const float BALL_MODEL_RADIUS = 83.951f;
	static const float BALL_RADIUS = 30.0f;
	static const float BALL_SCALE = BALL_RADIUS / BALL_MODEL_RADIUS;
	static const float BALL_COLOR_RADIUS = 90.0f;
}

/// <summary>
/// �L�����N�^�[��������{�[��
/// </summary>
/// <author>�����h�l</author>
class Ball : public Object3D
{
public:
	enum State
	{
		S_OWNED = 0,
		S_THROWN,
		S_LANDED
	};

	Ball();
	~Ball();
	void Reset(std::string charaTag);
	void Spawn();
	void Init(std::string charaTag = "None");
	void Update() override;
	void Draw() override;

	void Throw(CharaBase* owner, float chargeRate);
	void ThrowDirection(const Vector3& direction, CharaBase*owner, float chargeRate);
	void ThrowHoming(const CharaBase* target, CharaBase* owner,  float chargeRate);

	State GetState() const { return m_State; }

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

	std::string GetCharaTag() const { return m_CharaTag; }

	CharaBase* GetLastOwner() const { return m_LastOwner; }

	/// <summary>
	/// �L�����H
	/// </summary>
	/// <returns>�L���Ȃ�true</returns>
	bool IsActive() const { return m_IsActive; }

	void SetIsActive(bool flag) { m_IsActive = flag; }

	void SetTexture(const BallTexture& texture);

	void SetTrailImage(int hImage);

	void SetOwner(CharaBase* pChara);

	void PickUp();

	void SetChargeRate(float rate) { m_ChargeRate = rate; }

	void ChangeState(const State& state) { changeState(state); }

	void Knockback(const Vector3& other, float force_vertical, float force_horizontal);
private:
	friend class BallManager;
	BallManager*		m_pManager;
	Trail3D*			m_pTrail;

	Physics*			m_Physics;
	ColliderCapsule*	m_Collider;
	State				m_State;
	State				m_StatePrev;
	CharaBase*			m_Owner;
	CharaBase*			m_LastOwner;
	std::string			m_CharaTag;
	uint32_t			m_Index;
	float				m_LifeTime;
	float				m_LifeTimeMax;
	float				m_AlphaRate;
	int					m_hTrailImage;
	bool				m_IsActive;
	bool				m_IsPickedUp;

	// �z�[�~���O�n
	const CharaBase*	m_HomingTargetChara;	// �z�[�~���O���̃L�����̃|�C���^
	Vector3				m_HomingOrigin;			// �z�[�~���O�J�n�n�_
	Vector3				m_HomingTargetPos;		// �z�[�~���O�Ώۂ̍��W
	bool				m_IsHoming;	// �z�[�~���O����
	bool				m_DoRefreshHoming;	// �z�[�~���O����X�V���邩
	float				m_HomingProgress;
	float				m_HomingSpeed;
	float				m_HormingCurveAngle;	// �J�[�u���������߂�p�x
	float				m_HormingCurveScale;	// �J�[�u�̋Ȃ���ʂ̑傫��(0..1)

	void collisionToGround();
	// �n�`�Ƃ̉����o�������A����������true
	bool collisionToStage();
	void changeState(const State& s);
	void effectUpdate();
	void homingProcess();
	void homingDeactivate();

	float				m_ChargeRate;
};