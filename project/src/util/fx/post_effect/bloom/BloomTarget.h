#pragma once
#include "framework/gameObject.h"
#include "src/util/getset/GetSet.h"

class BloomTarget
{
public:
	virtual void Draw() {}
	const Getter<float> EffectRate = (&m_EffectRate);
protected:
	float m_EffectRate = 0.0f;	// 0..1
};

class BloomObject : public BloomTarget
{
public:
	BloomObject(GameObject* pObject, float effectRate)
	{
		m_pObject = pObject;
		m_EffectRate = effectRate;
	}
	void Draw() override
	{
		m_pObject->Draw();
	}
private:
	GameObject* m_pObject = nullptr;
};

class BloomDrawFunc : public BloomTarget
{
public:
	BloomDrawFunc(void(*pDrawFunc)(), float effectRate)
	{
		m_pDrawFunc = pDrawFunc;
		m_EffectRate = effectRate;
	}
	void Draw() override
	{
		m_pDrawFunc();
	}
private:
	void(*m_pDrawFunc)() = nullptr;
};
