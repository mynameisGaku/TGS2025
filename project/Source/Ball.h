#pragma once
#include "Object3D.h"
#include "CharaDefine.h"

class Physics;
class ColliderCapsule;
class CharaBase;
class Collider;

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
		S_THROWN
	};

	Ball();
	~Ball();
	void Init(CharaDefine::CharaTag charaTag);
	void Update() override;
	void Draw() override;

	void Throw(const Vector3& velocity);
	void Throw(const Vector3& velocity, CharaBase*owner);

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;
private:
	Physics*		m_Physics;
	ColliderCapsule* m_Collider;
	State			m_State;
	CharaBase*		m_Owner;
	CharaDefine::CharaTag m_CharaTag;

	void collisionToGround();
	void setVelocity(const Vector3& velocity);
};