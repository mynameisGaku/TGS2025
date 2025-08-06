#pragma once
#include "src/util/object3D/Object3D.h"

class Animator;
class BallTarget;

/// <summary>
/// 敵キャラ
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
	BallTarget* m_pBallTarget;			// ロックオンできるターゲット
};
