#include "src/scene/play/ball/Ball.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/component/physics/Physics.h"
#include "src/reference/ball/BallRef.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/common/stage/Stage.h"
#include "src/util/fx/post_effect/bloom/BloomManager.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/util/fx/effect/EffectManager.h"
#include "src/common/stage/StageObjectManager.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/common/component/renderer/BallRenderer.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include "src/scene/play/catcher/Catcher.h"
#include "src/util/fx/trail/trail3D/Trail3D.h"
#include "src/util/ptr/PtrUtil.h"

Ball::Ball()
{
	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(Vector3::Zero, Vector3::Zero);

	m_Owner = nullptr;
	m_pManager = nullptr;
	m_Index = 0;

	m_Collider = Object3D::AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;
	param.trs.scale = Vector3::Ones * BALL_RADIUS / BALL_SCALE * 2.0f;

	m_Collider->SetOffset(Vector3::Zero);
	m_Collider->BaseInit(param);

	m_pTrail = new Trail3D();

	Init();
}

Ball::~Ball()
{
	if (m_Owner != nullptr && m_Owner->LastBall() == this)
	{
		m_Owner->SetLastBall(nullptr);
	}

	PtrUtil::SafeDelete(m_pTrail);
}

void Ball::Reset(std::string charaTag)
{
	Init(charaTag);
	m_State = S_OWNED;
}

void Ball::Spawn()
{
	Init();
	m_State = S_LANDED;
}

void Ball::Init(std::string charaTag)
{
	transform->scale = Vector3::Ones * BALL_SCALE;

	m_CharaTag = CHARADEFINE_REF.Tags[0];

	m_HomingPeriod = 0.0f;

	m_Physics->velocity = Vector3::Zero;
	m_Physics->angularVelocity = Vector3::Zero;
	m_Physics->SetGravity(Vector3::Zero);
	m_Physics->SetFriction(Vector3::Zero);

	m_Collider->SetIsActive(true);

	m_LifeTimeMax = BALL_REF.LifeTimeMax;
	m_LifeTime = m_LifeTimeMax;
	m_AlphaRate = 255.0f;

	m_IsHoming = false;
	m_IsActive = true;
	m_IsPickedUp = false;

	ColDefine::Tag tag;
	std::list<ColDefine::Tag> targets;

	if (charaTag == "Red")
	{
		tag = ColDefine::Tag::tBallRed;
		targets = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tCatchRed, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain, ColDefine::Tag::tBallBlue, ColDefine::Tag::tBallRed };
	}
	else if (charaTag == "Blue")
	{
		tag = ColDefine::Tag::tBallBlue;
		targets = { ColDefine::Tag::tCharaRed, ColDefine::Tag::tCatchRed, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain, ColDefine::Tag::tBallBlue, ColDefine::Tag::tBallRed };
	}
	else
	{
		// tagが不正ならレッドってことにする
		tag = ColDefine::Tag::tNone;
		targets = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tCatchRed, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain, ColDefine::Tag::tBallBlue, ColDefine::Tag::tBallRed };
	}
	
	m_Collider->SetTag(tag);
	m_Collider->SetTargetTags(targets);
	
	m_CharaTag = charaTag;
}

void Ball::Update()
{
	Object3D::Update();

	if (not m_IsActive)
		return;

	if (m_IsHoming)
	{
		m_Physics->SetIsActive(false);
		HomingProcess();
	}
	else
	{
		if (m_Collider != nullptr && m_Collider->IsActive())
		{
			Vector3 p1 = transform->Global().position;
			Vector3 p2 = m_Collider->OffsetWorld();
			float radius = BALL_RADIUS;

			Vector3 pushVec;
			if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
			{
				transform->position += pushVec;

				// 押し出し方向
				Vector3 normal = pushVec.Normalize();

				// 速度を反射させる
				float bounciness = BALL_REF.BouncinessDefault;
				Vector3 vel = m_Physics->velocity;

				float dot = VDot(vel, normal);
				if (dot < 0.0f) // 内側からの衝突のみ反射
				{
					Vector3 reflectVel = vel - normal * (1.0f + bounciness) * dot;
					m_Physics->velocity = reflectVel;
				}

				// Y方向の反発時に転がり回転も発生
				if (abs(normal.y) > 0.5f)
				{
					float forwardRad = atan2f(m_Physics->velocity.x, m_Physics->velocity.z);
					transform->rotation.y = forwardRad;
					m_Physics->angularVelocity.x = m_Physics->FlatVelocity().GetLength() * 0.03f;
				}
				m_Physics->velocity.x *= 0.99f;
				m_Physics->velocity.z *= 0.99f;
			}
		}
	}

	if (m_State != S_OWNED)
	{
		if(m_IsPickedUp)
		{
			m_LifeTime -= GTime.deltaTime;
			if (m_LifeTime <= 0.0f)
			{
				m_AlphaRate -= 255.0f * GTime.deltaTime;

				if (m_AlphaRate <= 0.0f)
				{
					m_AlphaRate = 0.0f;
					m_IsActive = false;
				}
			}
		}
	}

	if (m_State != S_OWNED)
	{
		if(m_Physics->velocity.GetLengthSquared() >= 0.1f)
		{
			m_pTrail->Add(transform->position);
		}
	}
	m_pTrail->Update(); 
}


void Ball::Draw()
{
	if (not m_IsActive)
		return;

	m_pTrail->Draw();

	Object3D::Draw();
}

void Ball::Throw(const Vector3& velocity)
{
	m_State = S_THROWN;
	setVelocity(velocity * BALL_REF.SpeedDefault);
	m_Physics->SetGravity(BALL_REF.GravityDefault);
	m_Physics->SetFriction(BALL_REF.FrictionDefault);
	m_Collider->SetIsActive(true);
	m_pTrail->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, 1.0f, 40.0f);
	m_pTrail->SetSubdivisions(4);
	m_Owner = nullptr;
}

void Ball::Throw(const Vector3& velocity, CharaBase* owner)
{
	Throw(velocity);
	m_Owner = owner;
	m_LastOwner = m_Owner;
}

void Ball::ThrowHoming(const Vector3& velocity, CharaBase* owner, const CharaBase* target)
{
	Throw(velocity, owner);

	m_Physics->SetIsActive(false);

    m_HomingTargetChara = target;

	m_HomingPosition = transform->position;
	m_HomingTargetPos = (m_HomingTargetChara == nullptr) ? Vector3(0, 150, 1000) : m_HomingTargetChara->transform->position + Vector3::SetY(150.0f);
	m_IsHoming = true;

	// ターゲット位置と現在位置からちょうどいい時間を計算
	Vector3 diff = m_HomingTargetPos - m_HomingPosition;
	m_HomingPeriod = diff.GetLength() / m_Physics->velocity.GetLength();
}

void Ball::CollisionEvent(const CollisionData& colData)
{
	if (not m_IsActive)
		return;

	if (colData.Other()->Parent<Catcher>() != nullptr)
		return;

	if (m_State == S_THROWN)
	{
		if (m_IsHoming) HomingDeactivate();

		m_Physics->velocity = m_Physics->FlatVelocity() * -0.5f + Vector3(0, 20, 0);
		m_State = S_LANDED;

		EffectManager::Play3D("Hit_Wall.efk", transform->Global(), "Hit_Wall" + m_CharaTag);

		if (m_Owner && m_Owner->LastBall() == this)
		{
			m_Owner->SetLastBall(nullptr);
		}

		// === 他のボールとの衝突対応 ===
		Ball* otherBall = colData.Other()->Parent<Ball>();
		if (otherBall != nullptr && otherBall != this)
		{
			// --- 押し出し処理 ---
			const float minDist = BALL_RADIUS * 2.0f;
			Vector3 delta = transform->position - otherBall->transform->position;
			float dist = delta.GetLength();

			if (dist < minDist && dist > 0.0001f)
			{
				Vector3 correction = delta.Normalize() * (minDist - dist) * 0.5f;
				transform->position += correction;
				otherBall->transform->position -= correction;
			}

			// --- 運動量保存＋反発係数による反発 ---
			Vector3 normal = (transform->position - otherBall->transform->position).Normalize();
			Vector3 relVel = m_Physics->velocity - otherBall->m_Physics->velocity;
			float relVelAlongNormal = VDot(relVel, normal);

			if (relVelAlongNormal > 0.0f) return;

			float e = BALL_REF.BouncinessDefault;
			float j = -(1.0f + e) * relVelAlongNormal / 2.0f;
			Vector3 impulse = normal * j;

			m_Physics->velocity += impulse;
			otherBall->m_Physics->velocity -= impulse;

			// --- 回転のトルク反映 ---
			Vector3 tangent = relVel - normal * relVelAlongNormal;
			if (tangent.GetLength() > 0.001f)
			{
				tangent = tangent.Normalize();
				float torque = VDot(relVel, tangent) * 0.1f;

				m_Physics->angularVelocity.x += torque;
				otherBall->m_Physics->angularVelocity.x -= torque;
			}
		}
	}
}

void Ball::SetTexture(const BallTexture& texture)
{
	BallRenderer* ballRenderer = Object3D::GetComponent<BallRenderer>();
	if (ballRenderer == nullptr)
	{
		ballRenderer = Object3D::AddComponent<BallRenderer>();
		ballRenderer->InitVertices();
	}

	ballRenderer->SetTexture(texture);
}

void Ball::SetTrailImage(int hImage)
{
	m_hTrailImage = hImage;
}

void Ball::SetOwner(CharaBase* pChara)
{
	m_Owner = pChara;
	m_LastOwner = m_Owner;
}

void Ball::PickUp()
{
	m_IsPickedUp = true;
}

void Ball::collisionToGround()
{
	if (m_State == S_OWNED) return;

	Vector3 hitPos;
	bool hit = Stage::ColCheckGround(transform->position + Vector3::SetY(BALL_RADIUS), transform->position - Vector3::SetY(BALL_RADIUS), &hitPos);
	if (hit)
	{
		if (m_IsHoming) HomingDeactivate();

		// Y方向に跳ね返る
		transform->position = hitPos + Vector3::SetY(BALL_RADIUS);
		m_Physics->velocity.y *= -BALL_REF.BouncinessDefault;

		// 転がっていく処理
		float forwardRad = atan2f(m_Physics->velocity.x, m_Physics->velocity.z);
		transform->rotation.y = forwardRad;

		m_Physics->velocity.x *= 0.99f;
		m_Physics->velocity.z *= 0.99f;
		m_Physics->angularVelocity.x = m_Physics->FlatVelocity().GetLength() * 0.01f;

		m_State = S_LANDED;
	}
}

void Ball::HomingProcess()
{
	// ---- ホーミング補間 ----
	m_HomingTargetPos = (m_HomingTargetChara == nullptr) ? (Vector3::UnitZ * 200.0f) * m_Owner->transform->RotationMatrix() : m_HomingTargetChara->transform->position + Vector3::SetY(150.0f);
	Vector3 acceleration = m_HomingTargetPos - transform->position;
	Vector3 diff = m_HomingTargetPos - m_HomingPosition;

	acceleration += (diff - m_Physics->velocity * m_HomingPeriod) * 2.0f / (m_HomingPeriod * m_HomingPeriod);

	m_HomingPeriod -= GTime.deltaTime;
	if (m_HomingPeriod < 0.0f)
	{
		HomingDeactivate();
		return;
	}

	m_Physics->velocity += acceleration * GTime.deltaTime;
	m_HomingPosition += m_Physics->velocity * GTime.deltaTime;

	// ---- 押し出し + 跳ね返り処理 ----
	if (m_Collider)
	{
		Vector3 p1 = m_HomingPosition;
		Vector3 p2 = m_Collider->OffsetWorld();  // ホーミング中は本来位置からズレるが、近似可

		float radius = BALL_RADIUS;
		Vector3 pushVec;

		if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
		{
			HomingDeactivate();

			m_HomingPosition += pushVec;

			Vector3 normal = pushVec.Normalize();
			float dot = VDot(m_Physics->velocity, normal);

			if (dot < 0.0f)
			{
				const float bounciness = BALL_REF.BouncinessDefault;
				const float damping = 0.85f; // ホーミング中の減衰

				Vector3 reflectVel = m_Physics->velocity - normal * (1.0f + bounciness) * dot;
				m_Physics->velocity = reflectVel * damping;
			}

			m_State = S_LANDED;
		}
	}

	transform->position = m_HomingPosition;
}

void Ball::HomingDeactivate()
{
	m_Physics->velocity *= GTime.deltaTime;
	m_Physics->SetIsActive(true);
	m_Physics->SetGravity(BALL_REF.GravityDefault);
	// 物理にホーミングの情報を引き継ぐ
	m_IsHoming = false;
}

void Ball::setVelocity(const Vector3& velocity)
{
	m_Physics->velocity = velocity;
}
