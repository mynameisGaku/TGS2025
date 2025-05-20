#pragma once
#include "Object3D.h"
#include "Component/ColliderCapsule.h"
#include <string>
#include <Library/time.h>
#include "TinyFSM.h"
#include "nlohmann/json_fwd.hpp"

class CharaHP;
class CharaStamina;
class Physics;
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

	void Block();

	//=======================================================================================
	// ▼各ステート
	void StateBlock(FSMSignal sig);
	void StateBlockToIdle(FSMSignal sig);
	void StateBlockWalk(FSMSignal sig);
	void StateIdle(FSMSignal sig);
	void StateIdleEmote_CheckSword(FSMSignal sig);
	void StateIdleEmote_Stretch(FSMSignal sig);
	void StateIdleToBlock(FSMSignal sig);
	void StateRun(FSMSignal sig);
	void SubStateNone(FSMSignal sig);
	void SubStateFall(FSMSignal sig);

private:
	friend class CharaManager;
	CharaStamina*	m_pStamina;				// スタミナのポインター
	CharaHP*		m_pHP;					// HPのポインター
	Physics*		m_pPhysics;				// 物理挙動のポインター
	float			m_MoveSpeed;			// 移動速度
	float			m_RotSpeed;				// 回転速度
	float			m_SpeedScale;			// 速度倍率
	int				m_Index;				// 自身のインデックス
	std::string		m_CharaTag;				// キャラクターのチームのタグ
	TinyFSM<CharaBase>* m_FSM;				// ステートマシン
	TinyFSM<CharaBase>* m_SubFSM;			// ステートマシン
	Animator*		m_Animator;				// アニメーション
	float			m_EmoteTimer;			// 放置アニメーションまでの時間
	bool			m_IsLanding;			// 着地中
	float			m_SlideTimer;			// スライディング残り時間タイマー
	Transform*		m_EffectTransform;		// エフェクト出すトランスフォーム
	Timeline<CharaBase>* m_Timeline;		// アニメーションに合わせて動くタイムライン
	bool			m_CanMove;				// 移動可能か
	bool			m_CanRot;				// 回転可能か
	bool			m_IsMove;				// 移動しようとしているか
	bool			m_IsBlocking;			// ブロック中

	void idleUpdate();
	void runUpdate();
	void blockUpdate();
	
	//=== タイムライン用 ===
	void setAnimationSpeed(const nlohmann::json& argument);
	void moveToPosition(const nlohmann::json& argument);
	void changeToRoll(const nlohmann::json& argument);
	void endRoll(const nlohmann::json& argument);
	void setCanMove(const nlohmann::json& argument);
	void setCanRot(const nlohmann::json& argument);
	void setVelocity(const nlohmann::json& argument);
};