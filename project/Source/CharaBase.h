#pragma once
#include "Object3D.h"

/// <summary>
/// �L�����N�^�[�Ɋւ�����N���X
/// </summary>
class CharaBase : public Object3D
{
public:

	CharaBase();
	virtual ~CharaBase();

	virtual void Update() override;

private:

};