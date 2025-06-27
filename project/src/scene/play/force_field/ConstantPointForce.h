#pragma once
#include "src/scene/play/force_field/ForceBase.h"

/// <summary>
/// ��葬�x�Œ��S�ɋ߂Â�or�����
/// </summary>
class ConstantPointForce : public ForceBase
{
public:
	ConstantPointForce(float speed) : m_Speed(speed) {}
	void AddForce(Object3D* target) override;
private:
	float m_Speed;	// �ړ���(cm/s)
};