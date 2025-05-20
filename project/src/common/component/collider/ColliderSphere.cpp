#include "src/common/component/collider/ColliderSphere.h"

ColliderSphere::ColliderSphere() {

	shape = ColDefine::Shape::sSphere;
}

void ColliderSphere::Draw() {

	if (isDraw == false)
		return;

	DrawSphere3D(transform->Global().position,	Radius(), 16, 0xFF0000, 0x000000, false);
}
