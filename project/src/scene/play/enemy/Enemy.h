#pragma once
#include "src/util/object3D/Object3D.h"

/// <summary>
/// �G�L����
/// </summary>
class Enemy : public Object3D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
private:
};
