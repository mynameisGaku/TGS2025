#include "ForceFieldManager.h"
#include "ForceFieldBase.h"
#include "ForceFieldSphere.h"
#include "ForceFieldCorn.h"

ForceFieldManager::ForceFieldManager()
{
}

ForceFieldManager::~ForceFieldManager()
{
	// ���ׂĂ̗͏���폜
	for (ForceFieldBase* forceField : m_ForceFields)
	{
		delete forceField;
	}
	// ���X�g���N���A
	m_ForceFields.clear();
}

void ForceFieldManager::Update()
{
	// �͏�̍X�V����
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
	// �͏�̕`�揈��
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
