#pragma once
#include "src/scene/play/force_field/ForceFieldSphere.h"

/// <summary>
/// コーン範囲の力場
/// </summary>
/// <author>佐藤紘斗</author>
class ForceFieldCorn : public ForceFieldSphere
{
public:
	ForceFieldCorn(float radius, float cornAngle) : ForceFieldSphere(radius), m_CornAngle(cornAngle) {}
	~ForceFieldCorn() {}

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void CollisionEvent(const CollisionData& colData) override;
private:
	float m_CornAngle;	// コーンの角度
};