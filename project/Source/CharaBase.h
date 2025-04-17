#pragma once
#include "Object3D.h"

/// <summary>
/// キャラクターに関する基底クラス
/// </summary>
class CharaBase : public Object3D
{
public:

	CharaBase();
	virtual ~CharaBase();

	virtual void Update() override;

private:

};