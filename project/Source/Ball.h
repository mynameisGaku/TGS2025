#pragma once
#include "Object3D.h"

class Physics;

/// <summary>
/// キャラクターが投げるボール
/// </summary>
/// <author>佐藤紘斗</author>
class Ball : public Object3D
{
public:
	Ball();
	~Ball();
	void Update() override;
	void Draw() override;

	void SetVelocity(const Vector3& velocity);
private:
	Physics* m_Physics;

	bool isHitFloor() const;
};