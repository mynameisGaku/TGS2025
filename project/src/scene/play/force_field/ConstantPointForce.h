#pragma once
#include "src/scene/play/force_field/ForceBase.h"

/// <summary>
/// ๊่ฌxลSษ฿รญorฃ๊้
/// </summary>
/// <author>ฒกhl</author>
class ConstantPointForce : public ForceBase
{
public:
	ConstantPointForce(float speed) : m_Speed(speed) {}
	void AddForce(Object3D* target) override;
private:
	float m_Speed;	// ฺฎส(cm/s)
};