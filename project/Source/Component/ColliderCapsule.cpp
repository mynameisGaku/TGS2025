#include "colliderCapsule.h"

ColliderCapsule::ColliderCapsule() {

	shape = ColDefine::Shape::sCapsule;
}

void ColliderCapsule::Draw() {

	if (isDraw == false)
		return;

	DrawCapsule3D(transform->Global().position, transform->Global().position + offset, Radius(), 16, 0xFF0000, 0x000000, false);

	if (lastGlobal != nullptr)
		DrawCapsule3D(lastGlobal->Global().position, lastGlobal->Global().position + offset, Radius(), 16, 0xFF0000, 0x000000, false);
}
