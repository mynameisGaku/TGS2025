#pragma once
#include <memory>

class ForceFieldBase;
class Object3D;

class ForceBase
{
public:
	ForceBase() : m_pForceField(nullptr) {}
	virtual ~ForceBase() {}

	void SetForceField(ForceFieldBase* pForceField)
	{
		m_pForceField = pForceField;
	}

	virtual void AddForce(Object3D* target) {}
private:
	const ForceFieldBase* m_pForceField; // óÕèÍÇÃÉ|ÉCÉìÉ^
};