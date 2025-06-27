#pragma once
#include "src/scene/play/force_field/ForceBase.h"

/// <summary>
/// ˆê’è‘¬“x‚Å’†S‚É‹ß‚Ã‚­or—£‚ê‚é
/// </summary>
/// <author>²“¡h“l</author>
class ConstantPointForce : public ForceBase
{
public:
	ConstantPointForce(float speed) : m_Speed(speed) {}
	void AddForce(Object3D* target) override;
private:
	float m_Speed;	// ˆÚ“®—Ê(cm/s)
};