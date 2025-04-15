#pragma once
#include "Library/gameObject.h"
#include <list>

class Ball;

/// <summary>
/// ボールの生成等マネージャー
/// </summary>
/// <author>佐藤紘斗</author>
class BallManager : public GameObject
{
public:
	BallManager();
	~BallManager();

	Ball* CreateBall(const Vector3& position);
private:
	int m_Model;
	std::list<Ball*> m_Balls;
};