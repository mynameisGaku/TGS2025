#pragma once
#include <list>
#include <memory>
#include "framework/gameObject.h"

class ForceFieldBase;
class ForceFieldSphere;
class ForceFieldCorn;
class Transform;

/// <summary>
/// �͏�}�l�[�W���[
/// </summary>
/// <author>�����h�l</author>
class ForceFieldManager : public GameObject
{
public:
	ForceFieldManager();
	~ForceFieldManager();
	void Update() override;
	void Draw() override;

	ForceFieldSphere* const CreateForceFieldSphere(const Transform& trs, float radius);
	ForceFieldCorn* const CreateForceFieldCorn(const Transform& trs, float radius, float cornAngle);
	void DeleteForceField(ForceFieldBase* target);
private:
	std::list<ForceFieldBase*> m_ForceFields;
};
