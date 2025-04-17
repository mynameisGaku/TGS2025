#include "colliderCapsule.h"

ColliderCapsule::ColliderCapsule() {

	shape = ColDefine::Shape::sCapsule;
}

void ColliderCapsule::Draw() {

	if (isDraw == false)
		return;

	const Transform globalTrs = transform->Global();
	const float radius = Radius();

	if (offset.SquareSize() != 0.0f)
		DrawCapsule3D(globalTrs.position, globalTrs.position + offset, radius, 16, 0xFF0000, 0x000000, false);
	else
		DrawSphere3D(globalTrs.position, radius, 16, 0xFF0000, 0x000000, false);

	if (lastGlobal != nullptr) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);

		if (offset.SquareSize() != 0.0f)
			DrawCapsule3D(globalTrs.position, globalTrs.position + offset, radius, 16, 0xFFFFFF, 0x000000, false);
		else
			DrawSphere3D(globalTrs.position, radius, 16, 0xFFFFFF, 0x000000, false);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
