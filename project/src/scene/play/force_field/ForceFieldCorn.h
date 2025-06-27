#pragma once
#include "src/scene/play/force_field/ForceFieldSphere.h"

/// <summary>
/// �R�[���͈̗͂͏�
/// </summary>
/// <author>�����h�l</author>
class ForceFieldCorn : public ForceFieldSphere
{
public:
	ForceFieldCorn(float radius, float cornAngle) : ForceFieldSphere(radius), m_CornAngle(cornAngle) {}
	~ForceFieldCorn() {}

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;
private:
	float m_CornAngle;	// �R�[���̊p�x
};