#pragma once
#include "Object3D.h"
#include <string>

class Physics;
class ColliderCapsule;
class CharaBase;
class Collider;
class BallManager;

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
	void Reset();
	void Init(std::string charaTag);
	void Update() override;
	void Draw() override;

	void Throw(const Vector3& velocity);
	void Throw(const Vector3& velocity, CharaBase*owner);
	void ThrowHoming(const Vector3& velocity, CharaBase* owner);
	void HomingProcess();

	State GetState() const { return m_State; }

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

	std::string GetCharaTag() const { return m_CharaTag; }

	/// <summary>
	/// �L�����H
	/// </summary>
	/// <returns>�L���Ȃ�true</returns>
	bool IsActive() const { return m_IsActive; }

	void SetIsActive(bool flag) { m_IsActive = flag; }
private:
	friend class BallManager;
	BallManager*	 m_pManager;

	Physics*		 m_Physics;
	ColliderCapsule* m_Collider;
	State			 m_State;
	CharaBase*		 m_Owner;
	std::string		 m_CharaTag;
	uint32_t		 m_Index;
	float			 m_LifeTime;
	float			 m_LifeTimeMax;
	float			 m_AlphaRate;
	bool			 m_IsActive;

	// �z�[�~���O�n
	Vector3			m_HomingVelocity;
	Vector3			m_HomingPosition;
	Vector3			m_HomingTarget;
	float			m_HomingPeriod;
	bool			m_IsHoming;

	void collisionToGround();
	void HomingDeactivate();
	void setVelocity(const Vector3& velocity);
};