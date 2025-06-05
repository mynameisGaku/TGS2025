#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/renderer/BallRenderer.h"
#include <string>

class Physics;
class ColliderCapsule;
class CharaBase;
class Collider;
class BallManager;
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

	void Throw(const Vector3& velocity);
	void Throw(const Vector3& velocity, CharaBase*owner);
	void ThrowHoming(const Vector3& velocity, CharaBase* owner, const CharaBase* target);
	void HomingProcess();

	State GetState() const { return m_State; }

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void CollisionEvent(const CollisionData& colData) override;

	std::string GetCharaTag() const { return m_CharaTag; }

	CharaBase* GetLastOwner() const { return m_LastOwner; }

	/// <summary>
	/// 有効か？
	/// </summary>
	/// <returns>有効ならtrue</returns>
	bool IsActive() const { return m_IsActive; }

	void SetIsActive(bool flag) { m_IsActive = flag; }

	void SetTexture(const BallTexture& texture);

	void SetTrailImage(int hImage);

	void SetOwner(CharaBase* pChara);

	void PickUp();
private:
	friend class BallManager;
	BallManager*		m_pManager;
	Trail3D*			m_pTrail;

	Physics*			m_Physics;
	ColliderCapsule*	m_Collider;
	State				m_State;
	CharaBase*			m_Owner;
	CharaBase*			m_LastOwner;
	std::string			m_CharaTag;
	uint32_t			m_Index;
	float				m_LifeTime;
	float				m_LifeTimeMax;
	float				m_AlphaRate;
	int					m_hTrailImage;
	bool				m_IsActive;
	bool				m_IsPickedUp;

	// ホーミング系
	const CharaBase*	m_HomingTargetChara;
	Vector3				m_HomingVelocity;
	Vector3				m_HomingPosition;
	Vector3				m_HomingTargetPos;
	float				m_HomingPeriod;
	bool				m_IsHoming;

	void collisionToGround();
	void HomingDeactivate();
	void setVelocity(const Vector3& velocity);
};