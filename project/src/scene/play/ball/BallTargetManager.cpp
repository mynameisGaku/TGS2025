#include "BallTargetManager.h"
#include "src/util/pool/Pool.h"
#include "src/scene/play/ball/BallTarget.h"
#include "src/scene/play/chara/CharaManager.h"

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
	if (not m_pCharaManager)
	{
		m_pCharaManager = FindGameObject<CharaManager>();
	}

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

BallTarget* BallTargetManager::Create()
{
	// 既にアクティブが最大数なら生成しない
	if (m_Pool->CheckActiveObjectByCapacity()) return nullptr;

	uint32_t index = m_Pool->GetIndex();
	BallTarget* ballTarget = m_Pool->Alloc();

	ballTarget->Reset();
	ballTarget->SetIndex(index);

	m_Pool->SetObjectPointer(index, ballTarget);

	return ballTarget;
}

BallTarget* BallTargetManager::Get(uint32_t index) const
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

BallTarget* BallTargetManager::GetNearest(int index, float distance) const
{
	if (not m_pCharaManager) return nullptr;

	const Chara* chara = m_pCharaManager->CharaInst(index);
	if (chara == nullptr)
		return nullptr;

	struct distPair
	{
		distPair(uint32_t _index, float _distSq)
		{
			index = _index;
			distSq = _distSq;
		}
		uint32_t index = 0;
		float distSq = 0.0f;
	};

	std::list<distPair> dists;

	for (const auto& it : m_Pool->GetAllItems())
	{
		if (it->m_pObject == nullptr)
			continue;

		// 番号が同じもしくは、チームが同じ場合
		if (it->m_pObject->GetCharaTag() == chara->GetCharaTag())
			continue;

		// 距離計算
		float distSq = (chara->transform->position - it->m_pObject->Position()).GetLengthSquared();

		if (distSq >= distance * distance)
			continue;

		// 壁判定（予定）

		dists.push_back(distPair(it->m_pObject->Index(), distSq));
	}

	if (dists.empty()) return nullptr;

	auto aaa = std::min_element(dists.begin(), dists.end(), [](const distPair& a, const distPair& b) {
		return a.distSq < b.distSq;
		});

	return Get(aaa->index);
}
