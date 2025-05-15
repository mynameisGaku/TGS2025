#pragma once
#include "Object3D.h"
#include "Component/ColliderCapsule.h"
#include <string>
#include <Library/time.h>
#include "TinyFSM.h"
#include "nlohmann/json_fwd.hpp"

class CharaHP;
class CharaStamina;
class Ball;
class BallManager;
class Physics;
class Catcher;
class Animator;
template <class C>
class Timeline;

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

	/// <summary>
	/// 一定の力でジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 呼び出し中スライディングする
	/// </summary>
	void Slide();

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
    /// ホーミングボールを投げる
    /// </summary>
    /// <param name="param">ホーミング概要</param>
    void ThrowHomingBall();

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

	Ball* LastBall() const { return m_pLastBall; }
	void SetLastBall(Ball* ball) { m_pLastBall = ball; }

	/// <summary>
	/// 最後に投げたボールにテレポートし、それをキャッチする
	/// </summary>
	void TeleportToLastBall();

	//=======================================================================================
	// ▼キャッチ

	/// <summary>
	/// この関数を呼び出している間、吸引キャッチが行われる。
	/// </summary>
	void Catch();

	//=======================================================================================
	// ▼各ステート
	void StateActionIdle(FSMSignal sig);
	void StateActionIdleEmote(FSMSignal sig);
	void StateActionIdleToJump(FSMSignal sig);
	void StateActionIdleToRun(FSMSignal sig);
	void StateActionIdleToStandingIdle(FSMSignal sig);

	void StateAirSpin(FSMSignal sig);

	void StateCatch(FSMSignal sig);

	void StateCrouchToActionIdle(FSMSignal sig);
	void StateCrouchToRun(FSMSignal sig);

	void StateDamageToDown(FSMSignal sig);

	void StateFall(FSMSignal sig);
	void StateFallToCrouch(FSMSignal sig);
	void StateFallToRoll(FSMSignal sig);

	void StateGetBall(FSMSignal sig);

	void StateRoll(FSMSignal sig);
	void StateRollToActionIdle(FSMSignal sig);
	void StateRollToRun(FSMSignal sig);

	void StateRun(FSMSignal sig);
	void StateRunToActionIdle(FSMSignal sig);
	void StateRunToJump(FSMSignal sig);
	void StateRunToSlide(FSMSignal sig);

	void StateSlide(FSMSignal sig);
	void StateSlideToRun(FSMSignal sig);

	void StateStandingIdle(FSMSignal sig);
	void StateStandingIdleEmote(FSMSignal sig);
	void StateStandingIdleToActionIdle(FSMSignal sig);

	void StateAimToThrow(FSMSignal sig);

private:
	friend class CharaManager;
	bool			m_IsCharging;			// ボールをチャージしているかどうか
	float			m_BallChargeRate;		// ボールのチャージ加速度
	float			m_ChargeRateWatchDog;	// チャージ終了から何秒経ったかを監視する番犬
	Ball*			m_pBall;				// 所有しているボールのポインター
	Ball*			m_pLastBall;			// 最後に投げたボールのポインター
    BallManager*	m_pBallManager;			// ボールマネージャーのポインター
	CharaStamina*	m_pStamina;				// スタミナのポインター
	CharaHP*		m_pHP;					// HPのポインター
	Physics*		m_pPhysics;				// 物理挙動のポインター
	float			m_MoveSpeed;			// 移動速度
	float			m_RotSpeed;				// 回転速度
	int				m_Index;				// 自身のインデックス
	float			m_CatchTimer;			// キャッチ残り時間タイマー
	std::string		m_CharaTag;				// キャラクターのチームのタグ
	Catcher*		m_Catcher;				// キャッチの当たり判定
	TinyFSM<CharaBase>* m_FSM;				// ステートマシン
	Animator*		m_Animator;				// アニメーション
	float			m_EmoteTimer;			// 放置アニメーションまでの時間
	bool			m_IsLanding;			// 着地中
	float			m_SlideTimer;			// スライディング残り時間タイマー
	Transform*		m_EffectTransform;		// エフェクト出すトランスフォーム
	Timeline<CharaBase>* m_Timeline;		// アニメーションに合わせて動くタイムライン

	void idleUpdate();
	void runUpdate();
	void slideUpdate();

	void getHit(Ball* hit);
	
	//=== タイムライン用 ===
	void setAnimationSpeed(const nlohmann::json& argument);

	void moveToPosition(const nlohmann::json& argument);
};