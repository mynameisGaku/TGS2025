#include "BallTargetManager.h"
#include "src/util/pool/Pool.h"
#include "src/scene/play/ball/BallTarget.h"

namespace
{
    static const int COUNT_MAX = 128;
}

BallTargetManager::BallTargetManager()
{

    m_Pool = new Pool<BallTarget>(COUNT_MAX);
}

BallTargetManager::~BallTargetManager()
{
	for (auto& item : m_Pool->GetAllItems())
	{
		delete item->m_pObject;
		delete item;
	}

	delete m_Pool;
}

void BallTargetManager::Update()
{
	auto items = m_Pool->GetAllItems();
	for (auto& item : items)
	{
		BallTarget* ballTarget = item->m_pObject;

		if (ballTarget == nullptr)
			continue;

		if (ballTarget->IsActive())
		{
			ballTarget->Update();
		}
		else
		{
			m_Pool->DeActive(item->m_Index);
		}
	}
}

void BallTargetManager::Draw()
{
	auto items = m_Pool->GetAllItems();
	for (auto& item : items)
	{
		BallTarget* ballTarget = item->m_pObject;

		if (ballTarget == nullptr)
			continue;

		if (ballTarget->IsActive())
		{
			ballTarget->Draw();
		}
	}
}

BallTarget* BallTargetManager::Create(const Vector3& position)
{
	// 既にアクティブが最大数なら生成しない
	if (m_Pool->CheckActiveObjectByCapacity()) return nullptr;

	uint32_t index = m_Pool->GetIndex();
	BallTarget* ballTarget = m_Pool->Alloc();

	ballTarget->SetPosition(position);

	m_Pool->SetObjectPointer(index, ballTarget);

	return ballTarget;
}

BallTarget* BallTargetManager::GetBallTarget(uint32_t index)
{
	if (not m_Pool)
		return nullptr;

	if (index > m_Pool->GetCapacity())
		return nullptr;

	auto obj = m_Pool->GetItem(index);

	if (obj == nullptr)
		return nullptr;

	auto ballTarget = obj->m_pObject;
	if (ballTarget == nullptr)
		return nullptr;

	return ballTarget;
}
