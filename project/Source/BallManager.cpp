#include "BallManager.h"
#include "Library/resourceLoader.h"
#include "Ball.h"
#include "BallManager.h"
#include "BallRef.h"

BallManager::BallManager()
{
	m_Model = ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1");

	BALL_REF.Load();

	m_pPool = new Pool<Ball>(BALL_REF.Max);
}

BallManager::~BallManager()
{
#ifdef USE_POOL
	for (auto& item : m_pPool->GetAllItems())
	{
		if (item == nullptr)
			continue;

		if (item->m_pObject != nullptr)
		{
			item->m_pObject->DestroyMe();
			item->m_pObject = nullptr;
		}

		delete item;
		item = nullptr;
	}
	delete m_pPool;
#else
	for (Ball* ball : m_Balls)
	{
		ball->DestroyMe();
	}
	m_Balls.clear();
#endif

	ResourceLoader::MV1DeleteModel(m_Model);
}

void BallManager::Update()
{
#ifdef USE_POOL
	if (m_pPool->GetActiveItemNum() == 0)
		return;

	BALL_REF.Load();

	for (auto& item : m_pPool->GetAllItems())
	{
		auto ball = item->m_pObject;

		if (not ball)
			continue;

		if (not ball->IsActive())
		{
			m_pPool->DeActive(ball->m_Index);
		}
	}
#else
	for (auto it = m_Balls.begin(); it != m_Balls.end(); )
	{
		auto ball = (*it);
		if (not ball->IsActive())
		{
			// 無効化されたら削除
			ball->DestroyMe();
			ball = nullptr;
			it = m_Balls.erase(it);
		}
		else
		{
			++it;
		}
	}
#endif
}

void BallManager::Draw()
{
	m_pPool->PoolImGuiRendererBegin("ball pool debug");

	ImGui::Text(("Capacity: " + std::to_string(m_pPool->GetCapacity())).c_str());
	ImGui::Text(("Active Items: " + std::to_string(m_pPool->GetActiveItemNum())).c_str());
	ImGui::Spacing();
	ImGui::Text("====================================");
	ImGui::Spacing();

	for (auto& item : m_pPool->GetAllItems())
	{
		if (item->m_pObject == nullptr)
			continue;

		ImGui::Text(("Lifetime :" + std::to_string(item->m_pObject->m_LifeTime)).c_str());
	}

	m_pPool->PoolImguiRendererEnd();
}

Ball* BallManager::CreateBall(const Vector3& position)
{

	auto& ref = BALL_REF;
	// 各種リファレンスを元に初期化

	// 追加
#ifdef USE_POOL
	if (m_pPool->CheckActiveObjectByCount(m_pPool->GetCapacity()))
	{
		return nullptr;
	}
#endif


#ifdef USE_POOL
	uint32_t index = m_pPool->GetIndex();

	auto obj = m_pPool->Alloc([&](uint32_t i, Ball* p) { return initfunc(i, p); });
	obj->transform->position = position;
	obj->SetModel(m_Model);
	m_pPool->SetObjectPointer(index, obj);

	return obj;
#else
	Ball* ball = Instantiate<Ball>();
	ball->transform->position = position;
	m_Balls.push_back(ball);
	return ball;
#endif
	return nullptr;
}

Ball* BallManager::GetBall(uint32_t index)
{
#ifdef USE_POOL
	if (not m_pPool)
		return nullptr;

	if (index > m_pPool->GetCapacity())
		return nullptr;

    auto obj = m_pPool->GetItem(index);

    if (obj == nullptr)
        return nullptr;

    auto ball = obj->m_pObject;
    if (ball == nullptr)
        return nullptr;

	return ball;

#else
	// Pool使わない版 未実装

#endif

}

Ball* BallManager::initfunc(uint32_t index, Ball* pBall)
{
	pBall = Instantiate<Ball>();
	pBall->m_Index = index;
	return pBall;
}
