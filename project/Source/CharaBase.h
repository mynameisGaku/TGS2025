#pragma once
#include "Object3D.h"
#include <Library/time.h>

class CharaStamina;
class Ball;

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

	/// <summary>
	/// ボールを投げる
	/// </summary>
	/// <param name="velocity"></param>
	void ThrowBall(const Vector3& velocity);

	/// <summary>
	/// ボールを前方に投げる
	/// </summary>
	/// <param name="speed"></param>
	void ThrowBallForward();

	/// <summary>
	/// ボールを生成する。
	/// この関数を呼び出している間、ボールのチャージ率が上がる。
	/// </summary>
	void GenerateBall();

	/// <summary>
	/// ボールを持っているか？
	/// </summary>
	/// <returns></returns>
	bool IsHoldingBall() const { return nullptr != m_pBall; }

	/// <summary>
	/// チャージ中か？
	/// </summary>
	/// <returns></returns>
	bool IsChargingBall() const { return not m_IsCharging; }

private:
	bool			m_IsCharging;			// ボールをチャージしているかどうか
	float			m_BallChargeRate;		// ボールのチャージ加速度
	float			m_ChargeRateWatchDog;	// チャージ終了から何秒経ったかを監視する番犬
	Ball*			m_pBall;				// 所有しているボールのポインター
	CharaStamina*	m_pStamina;				// スタミナのポインター
};