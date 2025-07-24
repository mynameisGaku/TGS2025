#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/renderer/BallRenderer.h"
#include <memory>
#include <string>

class Physics;
class ColliderCapsule;
class Chara;
class Collider;
class BallManager;
class BallAttribute;
class Trail3D;
class BallTarget;

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

	void SetAttribute(BallAttribute* attribute);

	void Throw(Chara* owner, float chargeRate);
	void ThrowDirection(const Vector3& direction, Chara*owner, float chargeRate);
	void ThrowHoming(const std::shared_ptr<BallTarget>& target, Chara* owner,  float chargeRate, float curveAngle, float curveScale);

	State GetState() const { return m_State; }

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

	std::string GetCharaTag() const { return m_CharaTag; }

	Chara* GetLastOwner() const { return m_LastOwner; }

	/// <summary>
	/// �L�����H
	/// </summary>
	/// <returns>�L���Ȃ�true</returns>
	bool IsActive() const { return m_IsActive; }

	void SetIsActive(bool flag) { m_IsActive = flag; }

	void SetTexture(const BallTexture& texture, const std::string& mapKey);

	void SetTrailImage(int hImage);

	void SetOwner(Chara* pChara);

	void PickUp();

	void SetChargeRate(float rate) { m_ChargeRate = rate; }
	float GetChargeRate() const { return m_ChargeRate; }

	void ChangeState(const State& state) { changeState(state); }

	void Knockback(const Vector3& other, float force_vertical, float force_horizontal);

	void SetUniqueID(const std::string& id) { m_UniqueID = id; }

	const std::string& GetUniqueID() const { return m_UniqueID; }

	const uint32_t GetIndex() const { return m_Index; }

	BallRenderer& GetBallRenderer() { return *GetComponent<BallRenderer>(); }

	void SetCharaTag(const std::string& charaTag) { m_CharaTag = charaTag; };
private:
	friend class BallManager;
	BallManager*		m_pManager;
	std::vector<BallAttribute*> m_Attributes;
	Trail3D*			m_pTrail;

	Physics*			m_Physics;
	ColliderCapsule*	m_Collider;
	State				m_State;
	State				m_StatePrev;
	Chara*				m_Owner;
	Chara*				m_LastOwner;
	std::string			m_CharaTag;
	std::string			m_UniqueID;
	uint32_t			m_Index;
	float				m_LifeTime;
	float				m_LifeTimeMax;
	float				m_AlphaRate;
	int					m_hTrailImage;
	bool				m_IsActive;
	bool				m_IsPickedUp;

	// �z�[�~���O�n
	std::shared_ptr<BallTarget> m_HomingTarget;	// �z�[�~���O���̃g�����X�t�H�[���̃|�C���^

	Vector3	m_HomingOrigin;			// �z�[�~���O�J�n�n�_
	Vector3	m_HomingTargetPos;		// �z�[�~���O�Ώۂ̍��W

	bool	m_IsHoming;				// �z�[�~���O����
	bool	m_DoRefreshHoming;		// �z�[�~���O����X�V���邩

	float	m_HomingProgress;		// �z�[�~���O�i�s�x(0..1)
	float	m_HomingSpeed;			// �z�[�~���O�̐i�ޑ���
	float	m_HormingCurveAngle;	// �J�[�u���������߂�p�x
	float	m_HormingCurveScale;	// �J�[�u�̋Ȃ���ʂ̑傫��(0..1)

	void collisionToGround();
	// �n�`�Ƃ̉����o�������A����������true
	bool collisionToStage();
	void changeState(const State& s);
	void effectUpdate();
	void homingProcess();
	void homingDeactivate();

	float				m_ChargeRate;
};