#pragma once
#include "Object3D.h"
#include "Component/Physics.h"

/// <summary>
/// �L�����N�^�[��������{�[��
/// </summary>
/// <author>�����h�l</author>
class Ball : public Object3D
{
public:
	Ball();
	~Ball();
	void Update() override;
	void Draw() override;
private:
};