#pragma once
#include "Library/gameObject.h"
#include <list>
#include "Pool.h"

#define USE_POOL

class Ball;

/// <summary>
/// �{�[���̐������}�l�[�W���[
/// </summary>
/// <author>�����h�l</author>
class BallManager : public GameObject
{
public:
	BallManager();
	~BallManager();

	void Update() override;
	void Draw() override;

	Ball* CreateBall(const Vector3& position);

	Ball* GetBall(uint32_t index);

private:
	Ball* initfunc(uint32_t index, Ball* pBall);

	int m_Model;

#ifdef USE_POOL
	Pool<Ball>* m_pPool;
#else
	std::list<Ball*> m_Balls; // �j�Ѓ��X�g
#endif
};