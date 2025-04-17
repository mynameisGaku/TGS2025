#pragma once
#include "Object3D.h"

class Physics;
class ColliderSphere;

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
	void Update() override;
	void Draw() override;

	void Throw(const Vector3& velocity);
private:
	Physics* m_Physics;
	ColliderSphere* m_Collider;
	State m_State;

	void collisionToGround();
	void setVelocity(const Vector3& velocity);
};