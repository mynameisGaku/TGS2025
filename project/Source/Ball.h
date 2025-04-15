#pragma once
#include "Object3D.h"
#include "Component/Physics.h"

/// <summary>
/// キャラクターが投げるボール
/// </summary>
/// <author>佐藤紘斗</author>
class Ball : public Object3D
{
public:
	Ball();
	~Ball();
	void Update() override;
	void Draw() override;
private:
};