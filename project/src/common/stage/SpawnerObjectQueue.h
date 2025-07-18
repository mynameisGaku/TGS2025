#pragma once
#include <vector>
#include <string>
#include <src/scene/play/spawner/SpawnerDescs.h>
#include <src/util/transform/Transform.h>

typedef struct
{
	std::string			id;				// 固有id
	BALL_SPAWNER_DESC	desc;			// 記述
	int					modelHandle;	// モデル
	Transform			transform;		// トランスフォーム
	
}BALL_SPAWNER_QUEUE_OBJECT_DESC;

// ボールスポナー生成情報を溜めておくキュー
class BallSpawnerObjectQueue
{
public:
	BallSpawnerObjectQueue() : m_Queue() {}
	~BallSpawnerObjectQueue() { m_Queue.clear(); }

	void Push(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc);
	std::vector<BALL_SPAWNER_QUEUE_OBJECT_DESC> Get();

private:
	std::vector<BALL_SPAWNER_QUEUE_OBJECT_DESC> m_Queue;
};