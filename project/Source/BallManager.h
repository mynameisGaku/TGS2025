#pragma once
#include "Library/gameObject.h"
#include <list>

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

	Ball* CreateBall(const Vector3& position);
private:
	int m_Model;
	std::list<Ball*> m_Balls;
};