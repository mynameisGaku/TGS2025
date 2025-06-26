#pragma once
#include <memory>

class ForceFieldBase;

class ForceBase
{
public:
	ForceBase(const std::unique_ptr<ForceFieldBase>* pForceField) : m_pForceField(pForceField) {}
	virtual void AddForce() {}
private:
	const std::unique_ptr<ForceFieldBase>* m_pForceField; // —Íê‚Ìƒ|ƒCƒ“ƒ^
};