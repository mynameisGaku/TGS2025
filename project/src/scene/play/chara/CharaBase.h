#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include <string>
#include "src/util/time/GameTime.h"
#include "src/util/fsm/TinyFSM.h"
#include <vendor/nlohmann/json_fwd.hpp>

class CharaHP;
class CharaStamina;
class Ball;
class BallManager;
class Physics;
class Catcher;
class Animator;
template <class C>
class Timeline;
class StatusTracker;
class EffectBase;

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
	/// ボールを生成する。デバッグ用。
	/// </summary>
	void GenerateBall();

	void SetBall(Ball* ball);

	void StartBallCharge();

	void StartThrow();

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
	/// 投げるモーションだけ再生
	/// </summary>
	void Feint();

	/// <summary>
	/// ボールを持っているか？
	/// </summary>
	/// <returns></returns>
	bool IsHoldingBall() const { return nullptr != m_pBall; }

	/// <summary>
	/// チャージ中か？
	/// </summary>
	/// <returns></returns>
	bool IsChargingBall() const { return m_IsCharging; }

	Ball* LastBall() const { return m_pLastBall; }
	void SetLastBall(Ball* ball) { m_pLastBall = ball; }
	Ball* HitBall() const { return m_pHitBall; }

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

	void CatchSuccess(const Vector3& velocity);

	//=======================================================================================
	// ▼リスポーン
	
	/// <summary>
	/// この関数を呼び出すと、パラメータがリセットされ、指定位置にリスポーンする。
	/// ToDo : リスポーンステートを作る
	/// </summary>
	/// <param name="pos">リスポーン時位置</param>
	/// <param name="rot">リスポーン時回転</param>
	void Respawn(const Vector3& pos, const Vector3& rot);

	//=======================================================================================
	// ▼ゲッター

	/// <summary>
	/// HPを取得する
	/// </summary>
	inline const CharaHP* GetHP() const { return m_pHP; }

	/// <summary>
	/// インデックスを取得
	/// </summary>
	inline const int GetIndex() const { return m_Index; }

	/// <summary>
	/// キャラタグ(所属チーム名)を取得
	/// </summary>
	inline const std::string GetCharaTag() const { return m_CharaTag; }

	/// <summary>
	/// ステータストラッカーを取得
	/// </summary>
	inline StatusTracker* GetStatusTracker() const { return m_pStatusTracker; }

	//=======================================================================================
	// ▼各ステート
	void StateActionIdle(FSMSignal sig);
	void StateActionIdleEmote(FSMSignal sig);
	void StateActionIdleToJump(FSMSignal sig);
	void StateActionIdleToRun(FSMSignal sig);
	void StateActionIdleToStandingIdle(FSMSignal sig);

	void StateAimToThrow(FSMSignal sig);

	void StateAirSpin(FSMSignal sig);

	void StateCrouchToActionIdle(FSMSignal sig);
	void StateCrouchToRun(FSMSignal sig);

	void StateDamageToDown(FSMSignal sig);

	void StateFall(FSMSignal sig);
	void StateFallToCrouch(FSMSignal sig);
	void StateFallToRoll(FSMSignal sig);

	void StateFeint(FSMSignal sig);

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

	void SubStateNone(FSMSignal sig);
	void SubStateGetBall(FSMSignal sig);
	void SubStateHold(FSMSignal sig);
	void SubStateHoldToAim(FSMSignal sig);
	void SubStateCatch(FSMSignal sig);

private:
	friend class CharaManager;


	Ball*			m_pBall;				// 所有しているボールのポインター
	Ball*			m_pLastBall;			// 最後に投げたボールのポインター
	Ball*			m_pHitBall;				// あてられたボールのポインター
	BallManager*	m_pBallManager;			// ボールマネージャーのポインター
	CharaStamina*	m_pStamina;				// スタミナのポインター
	CharaHP*		m_pHP;					// HPのポインター
	Physics*		m_pPhysics;				// 物理挙動のポインター
	std::string		m_CharaTag;				// キャラクターのチームのタグ
	Catcher*		m_Catcher;				// キャッチの当たり判定
	EffectBase*		m_pCatchReadyEffect;	// キャッチの準備エフェクト
	EffectBase*		m_pCatchDustEffect;		// キャッチの粉エフェクト
	TinyFSM<CharaBase>* m_FSM;				// ステートマシン
	TinyFSM<CharaBase>* m_SubFSM;			// ステートマシン
	Animator*		m_Animator;				// アニメーション
	Transform*		m_EffectTransform;		// エフェクト出すトランスフォーム
	Timeline<CharaBase>* m_Timeline;		// アニメーションに合わせて動くタイムライン
	StatusTracker*	m_pStatusTracker;		// ステータスの統計
	int				m_Index;				// 自身のインデックス
	float			m_BallChargeRate;		// ボールのチャージ加速度
	float			m_MoveSpeed;			// 移動速度
	float			m_RotSpeed;				// 回転速度
	float			m_SpeedScale;			// 速度倍率
	float			m_EmoteTimer;			// 放置アニメーションまでの時間
	float			m_SlideTimer;			// スライディング残り時間タイマー
    float			m_CatchTimer;			// キャッチの残り時間タイマー
	bool			m_IsCharging;			// ボールをチャージしているかどうか
	bool			m_IsLanding;			// 着地中
	bool			m_CanMove;				// 移動可能か
	bool			m_CanRot;				// 回転可能か
	bool			m_IsMove;				// 移動しようとしているか
	bool			m_IsJumping;			// ジャンプ中か
	bool			m_CanCatch;				// キャッチ可能か
	bool			m_CanHold;				// ボールを持てるか
	bool			m_CanThrow;				// ボールを投げられるか
	bool			m_IsCatching;			// キャッチ中か

	void land();

	void idleUpdate();
	void runUpdate();
	void slideUpdate();
	void catchUpdate();
	void jumpUpdate();

	void getHit(Ball* hit);
	
	//=== タイムライン用 ===
	void setAnimationSpeed(const nlohmann::json& argument);
	void moveToPosition(const nlohmann::json& argument);
	void changeToRoll(const nlohmann::json& argument);
	void endRoll(const nlohmann::json& argument);
	void setCanMove(const nlohmann::json& argument);
	void setCanRot(const nlohmann::json& argument);
	void setVelocity(const nlohmann::json& argument);
	void throwBall(const nlohmann::json& argument);
};