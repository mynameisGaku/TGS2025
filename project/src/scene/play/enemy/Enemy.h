#pragma once
#include "src/util/object3D/Object3D.h"

class Animator;
class BallTarget;

/// <summary>
/// �G�L����
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
	BallTarget* m_pBallTarget;			// ���b�N�I���ł���^�[�Q�b�g
};
