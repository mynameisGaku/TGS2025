#pragma once

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
protected:
	const ForceFieldBase* m_pForceField; // óÕèÍÇÃÉ|ÉCÉìÉ^
};