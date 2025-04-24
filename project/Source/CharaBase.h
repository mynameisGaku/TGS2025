#pragma once
#include "Object3D.h"
#include "Component/ColliderCapsule.h"
#include <string>
#include <Library/time.h>
#include "TinyFSM.h"

class CharaStamina;
class Ball;
class Physics;
class Catcher;

/// <summary>
/// キャラクターに関する基底クラス
/// </summary>
class CharaBase : public Object3D
{
public:
	CharaBase();
	virtual ~CharaBase();

	void Init(std::string tag);
	void Update() override;
	void Draw() override;

	//=======================================================================================
	// ▼当たり判定

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData"></param>
	void CollisionEvent(const CollisionData& colData) override;

	/// <summary>
	/// 地面との当たり判定
	/// </summary>
	void HitGroundProcess();

	//=======================================================================================
	// ▼移動

	/// <summary>
	/// 移動速度を設定する
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	inline void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }

	/// <summary>
	/// 回転速度を設定する
	/// </summary>
	/// <param name="rotSpeed">回転速度</param>
	inline void SetRotSpeed(float rotSpeed) { m_RotSpeed = rotSpeed; }

	/// <summary>
	/// 指定方向に移動する
	/// </summary>
	/// <param name="dir">移動方向</param>
	void Move(const Vector3& dir);

	//=======================================================================================
	// ▼ボール

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

	//=======================================================================================
	// ▼キャッチ

	/// <summary>
	/// この関数を呼び出している間、吸引キャッチが行われる。
	/// </summary>
	void Catch();

	//=======================================================================================
	// ▼各ステート
	
	void IdleState(FSMSignal sig);
	void MoveState(FSMSignal sig);
	void ThrowState(FSMSignal sig);
	void CatchState(FSMSignal sig);

private:
	friend class CharaManager;
	bool			m_IsCharging;			// ボールをチャージしているかどうか
	float			m_BallChargeRate;		// ボールのチャージ加速度
	float			m_ChargeRateWatchDog;	// チャージ終了から何秒経ったかを監視する番犬
	Ball*			m_pBall;				// 所有しているボールのポインター
	CharaStamina*	m_pStamina;				// スタミナのポインター
	Physics*		m_pPhysics;				// 物理挙動のポインター
	float			m_MoveSpeed;			// 移動速度
	float			m_RotSpeed;				// 回転速度
	int				m_Index;				// 自身のインデックス
	float			m_CatchTimer;			// キャッチ残り時間タイマー
	std::string		m_CharaTag;				// キャラクターのチームのタグ
	Catcher*		m_Catcher;				// キャッチの当たり判定
	TinyFSM<AIController>* m_FSM;
};