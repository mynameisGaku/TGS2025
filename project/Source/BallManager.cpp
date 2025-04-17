#include "BallManager.h"
#include "Library/resourceLoader.h"
#include "Ball.h"
#include "BallManager.h"
#include "BallRef.h"

BallManager::BallManager()
{
	m_Model = ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1");

	BallRef::Inst().Load();
}

BallManager::~BallManager()
{
	for (Ball* ball : m_Balls)
	{
		ball->DestroyMe();
	}
	m_Balls.clear();

	ResourceLoader::MV1DeleteModel(m_Model);
}

Ball* BallManager::CreateBall(const Vector3& position)
{
	Ball* ball = Instantiate<Ball>();
	ball->transform->position = position;
	m_Balls.push_back(ball);

	return ball;
}
