#include "colliderSphere.h"

ColliderSphere::ColliderSphere() {

	shape = ColDefine::Shape::sSphere;
}

void ColliderSphere::Draw() {

	if (isDraw == false)
		return;

	DrawSphere3D(transform->Global().position,	Radius(), 16, 0xFF0000, 0x000000, false);

	if (lastGlobal != nullptr)
		DrawSphere3D(lastGlobal->Global().position,	Radius(), 16, 0xFFFFFF, 0x000000, false);
}
