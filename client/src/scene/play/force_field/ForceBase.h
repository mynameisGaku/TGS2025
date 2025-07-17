#pragma once

class ForceFieldBase;
class Object3D;

/// <summary>
/// �͏���ŃI�u�W�F�N�g���ǂ�����������`������N���X
/// </summary>
/// <author>�����h�l</author>
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
	const ForceFieldBase* m_pForceField; // �͏�̃|�C���^
};