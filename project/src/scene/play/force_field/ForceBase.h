#pragma once

class ForceFieldBase;
class Object3D;

/// <summary>
/// 力場内でオブジェクトをどう動かすか定義する基底クラス
/// </summary>
/// <author>佐藤紘斗</author>
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
	const ForceFieldBase* m_pForceField; // 力場のポインタ
};