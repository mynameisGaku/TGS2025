#include "SpawnerObjectQueue.h"
#include <vendor/uuid4/uuid4.h>

void BallSpawnerObjectQueue::Push(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc)
{
	BALL_SPAWNER_QUEUE_OBJECT_DESC queueElem{};
	queueElem.desc			= desc;
	queueElem.modelHandle	= hModel;
	queueElem.transform		= trs;
	char BUF[UUID4_LEN];
	uuid4_init();
	uuid4_generate(BUF);
	queueElem.id = BUF;
	m_Queue.push_back(queueElem);
}

std::vector<BALL_SPAWNER_QUEUE_OBJECT_DESC> BallSpawnerObjectQueue::Get()
{
	return m_Queue;
}
