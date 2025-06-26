#pragma once
#include "src/scene/play/force_field/ForceFieldSphere.h"

class ForceFieldCorn : public ForceFieldSphere
{
public:
	ForceFieldCorn(float radius, float cornAngle) : ForceFieldSphere(radius), m_CornAngle(cornAngle) {}
	~ForceFieldCorn() {}

	/// <summary>
	/// “–‚½‚è”»’èˆ—
	/// </summary>
	/// <param name="colData">“–‚½‚è”»’èî•ñ</param>
	void CollisionEvent(const CollisionData& colData) override;
private:
	float m_CornAngle;	// ƒR[ƒ“‚ÌŠp“x
};