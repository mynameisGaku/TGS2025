#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/renderer/BallRenderer.h"
#include <string>

class Physics;
class ColliderCapsule;
class Chara;
class Collider;
class BallManager;
class BallAttribute;
class Trail3D;

namespace
{
	static const float BALL_MODEL_RADIUS = 83.951f;
	static const float BALL_RADIUS = 30.0f;
	static const float BALL_SCALE = BALL_RADIUS / BALL_MODEL_RADIUS;
	static const float BALL_COLOR_RADIUS = 90.0f;
}

/// <summary>
/// キャラクターが投げるボール
/// </summary>
/// <author>佐藤紘斗</author>
class Ball : public Object3D
{
public:
	enum State
	{
		S_OWNED = 0,
		S_THROWN,
		S_LANDED
	};

	Ball();
	~Ball();
	void Reset(std::string charaTag);
	void Spawn();
	void Init(std::string charaTag = "None");
	void Update() override;
	void Draw() override;

	void SetAttribute(const BallAttribute& attribute);

	void Throw(Chara* owner, float chargeRate);
	void ThrowDirection(const Vector3& direction, Chara*owner, float chargeRate);
	void ThrowHoming(const Chara* target, Chara* owner,  float chargeRate, float curveAngle, float curveScale);

	State GetState() const { return m_State; }

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void CollisionEvent(const CollisionData& colData) override;

	std::string GetCharaTag() const { return m_CharaTag; }

	Chara* GetLastOwner() const { return m_LastOwner; }

	/// <summary>
	/// 有効か？
	/// </summary>
	/// <returns>有効ならtrue</returns>
	bool IsActive() const { return m_IsActive; }

	void SetIsActive(bool flag) { m_IsActive = flag; }

	void SetTexture(const BallTexture& texture);

	void SetTrailImage(int hImage);

	void SetOwner(Chara* pChara);

	void PickUp();

	void SetChargeRate(float rate) { m_ChargeRate = rate; }
	float GetChargeRate() const { return m_ChargeRate; }

	void ChangeState(const State& state) { changeState(state); }

	void Knockback(const Vector3& other, float force_vertical, float force_horizontal);
private:
	friend class BallManager;
	BallManager*		m_pManager;
	std::vector<BallAttribute*> m_Attributes;
	Trail3D*			m_pTrail;

	Physics*			m_Physics;
	ColliderCapsule*	m_Collider;
	State				m_State;
	State				m_StatePrev;
	Chara*			m_Owner;
	Chara*			m_LastOwner;
	std::string			m_CharaTag;
	uint32_t			m_Index;
	float				m_LifeTime;
	float				m_LifeTimeMax;
	float				m_AlphaRate;
	int					m_hTrailImage;
	bool				m_IsActive;
	bool				m_IsPickedUp;

	// ホーミング系
	const Chara*	m_HomingTargetChara;	// ホーミング中のキャラのポインタ
	Vector3				m_HomingOrigin;			// ホーミング開始地点
	Vector3				m_HomingTargetPos;		// ホーミング対象の座標
	bool				m_IsHoming;	// ホーミング中か
	bool				m_DoRefreshHoming;	// ホーミング先を更新するか
	float				m_HomingProgress;
	float				m_HomingSpeed;
	float				m_HormingCurveAngle;	// カーブ方向を決める角度
	float				m_HormingCurveScale;	// カーブの曲がり量の大きさ(0..1)

	void collisionToGround();
	// 地形との押し出し処理、当たったらtrue
	bool collisionToStage();
	void changeState(const State& s);
	void effectUpdate();
	void homingProcess();
	void homingDeactivate();

	float				m_ChargeRate;
};