#include "MovableArea.h"

#include "src/common/component/collider/ColliderBase.h"

MovableArea::MovableArea() : MovableArea(ColDefine::AABB())
{
}

MovableArea::MovableArea(const ColDefine::AABB& aabb)
{
	m_AABB = aabb;
}

MovableArea::~MovableArea()
{
}

void MovableArea::Update()
{

}

void MovableArea::Draw()
{
	// デバッグ表示
	return;

	std::list<Vector3> pos;

	pos.push_back(m_AABB.p + m_AABB.hl * Vector3( 1, 0,  1));
	pos.push_back(m_AABB.p + m_AABB.hl * Vector3( 1, 0, -1));
	pos.push_back(m_AABB.p + m_AABB.hl * Vector3(-1, 0,  1));
	pos.push_back(m_AABB.p + m_AABB.hl * Vector3(-1, 0, -1));

	pos.push_back(m_AABB.p + m_AABB.hl * Vector3( 1, 1,  1));
	pos.push_back(m_AABB.p + m_AABB.hl * Vector3( 1, 1, -1));
	pos.push_back(m_AABB.p + m_AABB.hl * Vector3(-1, 1,  1));
	pos.push_back(m_AABB.p + m_AABB.hl * Vector3(-1, 1, -1));

	for (const auto& p : pos)
	{
		for (const auto& q : pos)
		{
			Vector3 dif = p - q;

			if (p == q)
				continue;

			DrawLine3D(p, q, 0xFF0000);
		}
	}
}
