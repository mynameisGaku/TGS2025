#pragma once
#include "src/util/object3D/Object3D.h"

class Animator;

/// <summary>
/// “GƒLƒƒƒ‰
/// </summary>
class Enemy : public Object3D
{
public:
	Enemy();
	~Enemy();
	void Init();
	void Update() override;
	void Draw() override;
private:
	Animator* m_Animator;
};
