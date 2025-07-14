#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/collider/CollisionDefine.h"

class MovableArea : public Object3D {
public:
	MovableArea();
	MovableArea(const ColDefine::AABB& aabb);
	~MovableArea();

	void Update() override;
	void Draw() override;

	ColDefine::AABB GetAABB() const { return m_AABB; }

private:
	ColDefine::AABB m_AABB;

};