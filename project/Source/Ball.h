#pragma once
#include "Object3D.h"

class Physics;
class ColliderSphere;
class CharaBase;

/// <summary>
/// キャラクターが投げるボール
/// </summary>
/// <author>佐藤紘斗</author>
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
	void Update() override;
	void Draw() override;

	void Throw(const Vector3& velocity);
	void Throw(const Vector3& velocity, CharaBase*owner);
private:
	Physics*		m_Physics;
	ColliderSphere* m_Collider;
	State			m_State;
	CharaBase*		m_Owner;

	void collisionToGround();
	void setVelocity(const Vector3& velocity);
};