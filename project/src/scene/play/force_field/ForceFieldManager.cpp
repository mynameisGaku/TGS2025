#include "ForceFieldManager.h"
#include "ForceFieldBase.h"
#include "ForceFieldSphere.h"
#include "ForceFieldCorn.h"

ForceFieldManager::ForceFieldManager()
{
}

ForceFieldManager::~ForceFieldManager()
{
	// すべての力場を削除
	for (ForceFieldBase* forceField : m_ForceFields)
	{
		delete forceField;
	}
	// リストをクリア
	m_ForceFields.clear();
}

void ForceFieldManager::Update()
{
	// 力場の更新処理
	for (ForceFieldBase* forceField : m_ForceFields)
	{
		if (forceField)
		{
			forceField->Update();
		}
	}
}

void ForceFieldManager::Draw()
{
	// 力場の描画処理
	for (ForceFieldBase* forceField : m_ForceFields)
	{
		if (forceField)
		{
			forceField->Draw();
		}
	}
}

ForceFieldSphere* const ForceFieldManager::CreateForceFieldSphere(const Transform& trs, float radius)
{
	ForceFieldSphere* forceField = new ForceFieldSphere(radius);

	forceField->transform->position = trs.position;
	forceField->transform->rotation = trs.rotation;
	forceField->transform->scale = trs.scale;

	m_ForceFields.push_back(forceField);
	return forceField;
}

ForceFieldCorn* const ForceFieldManager::CreateForceFieldCorn(const Transform& trs, float radius, float cornAngle)
{
	ForceFieldCorn* forceField = new ForceFieldCorn(radius, cornAngle);

	forceField->transform->position = trs.position;
	forceField->transform->rotation = trs.rotation;
	forceField->transform->scale = trs.scale;

	m_ForceFields.push_back(forceField);
	return forceField;
}
