#pragma once
#include <list>
#include <memory>
#include "framework/gameObject.h"

class ForceFieldBase;
class ForceFieldSphere;
class ForceFieldCorn;
class Transform;

/// <summary>
/// 力場マネージャー
/// </summary>
/// <author>佐藤紘斗</author>
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
