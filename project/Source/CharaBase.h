#pragma once
#include "Object3D.h"
#include <Library/time.h>

class CharaStamina;

/// <summary>
/// キャラクターに関する基底クラス
/// </summary>
class CharaBase : public Object3D
{
public:

	CharaBase();
	virtual ~CharaBase();

	virtual void Update() override;

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData"></param>
	void CollisionEvent(const CollisionData& colData) override;

	/// <summary>
	/// 地面との当たり判定
	/// </summary>
	void HitGroundProcess();

private:
	CharaStamina* m_Stamina; // スタミナのポインター
};