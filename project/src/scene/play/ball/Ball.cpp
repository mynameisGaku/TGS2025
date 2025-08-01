#include <src/scene/play/ball/Ball.h>
#include <src/util/file/resource_loader/ResourceLoader.h>
#include <src/common/component/physics/Physics.h>
#include <src/reference/ball/BallRef.h>
#include <src/common/component/collider/ColliderCapsule.h>
#include <src/common/stage/Stage.h>
#include <src/util/fx/post_effect/bloom/BloomManager.h>
#include <src/reference/chara/CharaDefineRef.h>
#include <src/scene/play/chara/Chara.h>
#include <src/util/fx/effect/EffectManager.h>
#include <src/common/stage/StageObjectManager.h>
#include <src/scene/play/ball/BallManager.h>
#include <src/scene/play/ball/attributes/BallAttribute.h>
#include <src/scene/play/ball/attributes/BallAttribute_Explosion.h>
#include <src/common/component/renderer/BallRenderer.h>
#include <src/scene/play/status_tracker/StatusTracker.h>
#include <src/scene/play/catcher/Catcher.h>
#include <src/util/fx/trail/trail3D/Trail3D.h>
#include <src/util/ptr/PtrUtil.h>
#include <src/util/math/Random.h>
#include <src/util/math/Vector3Util.h>
#include <src/util/math/matrix.h>
#include <src/util/math/MathUtil.h>
#include <src/scene/play/ball/BallTarget.h>
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>

namespace
{
	const float DROPDUST_APPEAR_RATE_PERCENT = 95.0f / 100.0f; // %
	const float RAND_RANGE_MAX = 100.0f;
	const float RAND_RANGE_MIN = 0.0f;
}

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

	m_ChargeRate = 0.0f;

	SetAttribute(new BallAttribute_Explosion(this));

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
	changeState(S_OWNED);
}

void Ball::Spawn()
{
	Init();
	changeState(S_LANDED);
}

void Ball::Init(std::string charaTag)
{
	transform->scale = Vector3::Ones * BALL_SCALE;

	m_CharaTag = CHARADEFINE_REF.Tags[0];

	m_Physics->velocity = Vector3::Zero;
	m_Physics->angularVelocity = Vector3::Zero;
	m_Physics->SetGravity(Vector3::Zero);
	m_Physics->SetFriction(Vector3::Zero);

	m_HomingOrigin = Vector3::Zero;

	m_Collider->SetIsActive(true);

	m_LifeTimeMax = BALL_REF.LifeTimeMax;
	m_LifeTime = m_LifeTimeMax;
	m_AlphaRate = 255.0f;

	if (not m_pManager)
		m_pManager = FindGameObject<BallManager>();

	m_IsThorwing = false;
	m_IsHoming = false;
	m_DoRefreshHoming = false;
	m_IsActive = true;
	m_IsPickedUp = false;
	m_HomingSpeed = 0.0f;
	m_HomingProgress = 0.0f;
	m_HormingCurveAngle = 0.0f;
	m_HormingCurveScale = 0.0f;

	ColDefine::Tag tag;
	std::list<ColDefine::Tag> targets;
	
	if (charaTag == "Red")
	{
		tag = ColDefine::Tag::tBall;
		targets = { ColDefine::Tag::tChara, ColDefine::Tag::tCatch, ColDefine::Tag::tCatch, ColDefine::Tag::tTerrain, ColDefine::Tag::tBall, ColDefine::Tag::tBlue, ColDefine::Tag::tGimmick };
	}
	else if (charaTag == "Blue")
	{
		tag = ColDefine::Tag::tBall;
		targets = { ColDefine::Tag::tChara, ColDefine::Tag::tCatch, ColDefine::Tag::tCatch, ColDefine::Tag::tTerrain, ColDefine::Tag::tBall, ColDefine::Tag::tRed, ColDefine::Tag::tGimmick };
	}
	else
	{
		// tagが不正ならレッドってことにする
		tag = ColDefine::Tag::tBall;
		targets = { ColDefine::Tag::tChara, ColDefine::Tag::tCatch, ColDefine::Tag::tCatch, ColDefine::Tag::tTerrain, ColDefine::Tag::tBall, ColDefine::Tag::tBlue, ColDefine::Tag::tGimmick };
	}

	targets.push_back(ColDefine::Tag::tWindArea);
	
	m_Collider->SetTag(tag);
	m_Collider->SetTargetTags(targets);
	
	m_CharaTag = charaTag;
	EffectManager::Stop("Ball_Outline.efk", "Ball_Outline" + m_CharaTag);
}

void Ball::Update()
{
	if (not m_IsActive)
	{
		return;
	}
	auto& net = NetworkRef::Inst();
	
	Object3D::Update();


	// エフェクト
	effectUpdate();

	if (m_IsHoming)
	{
		m_Physics->SetIsActive(false);
		homingProcess();
	}

	bool hit = collisionToStage();

	m_pTrail->Update();


	if ((m_IsHoming || m_IsThorwing) && hit)
	{
		homingDeactivate();
		changeState(S_LANDED);
		EffectManager::Play3D("Hit_Wall.efk", transform->Global(), "Hit_Wall" + m_CharaTag);

		for (const auto& attribute : m_Attributes) {
			if (attribute != nullptr)
				attribute->OnGround();
		}
		m_IsThorwing = false;
	}

	if (net.IsNetworkEnable)
	{
		if (not net.IsHost)
			return;
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

	if (m_State == Ball::S_OWNED)
	{
		for (const auto& attribute : m_Attributes) {
			if (attribute != nullptr)
				attribute->OnHave();
		}
	}

	if (m_State == Ball::S_THROWN)
	{
		for (const auto& attribute : m_Attributes) {
			if (attribute != nullptr)
				attribute->Throwing();
		}
	}

	if (net.IsNetworkEnable)
	{
		if (net.IsHost)
		{
			if (not m_pNetworkManager)
				m_pNetworkManager = SceneManager::CommonScene()->FindGameObject<NetworkManager>();

			m_pNetworkManager->SendSetBallTransform(m_UniqueID, transform->Global());
		}
	}
}

void Ball::effectUpdate()
{
	std::string eff = "Ball_Outline_";
	eff += m_CharaTag;
	eff += "_";
	if (m_State == S_THROWN || m_State == S_OWNED)
	{
		EffectManager::Play3D_Loop(eff + "Holding.efk", transform->Global(), eff + std::to_string(m_Index));
	}
	else
	{
		EffectManager::Stop(eff + "Holding.efk", eff + std::to_string(m_Index));
	}

	if (m_State == S_THROWN)
	{
		if (Random.GetFloatRange(RAND_RANGE_MIN, RAND_RANGE_MAX) / RAND_RANGE_MAX >= DROPDUST_APPEAR_RATE_PERCENT)
		{
			EffectManager::Play3D("DropDust.efk", transform->Global(), "DropDust" + m_CharaTag);
		}
	}
}

void Ball::Draw()
{
	if (not m_IsActive)
		return;

	m_pTrail->Draw();

	Object3D::Draw();
}

void Ball::SetAttribute(BallAttribute* attribute) {

	m_Attributes.push_back(attribute);
}

void Ball::Throw(Chara* owner, float chargeRate)
{
	changeState(S_THROWN);

	m_Physics->SetGravity(GRAVITY);
	m_Physics->SetFriction(Vector3(0.01f));

	m_Collider->SetIsActive(true);

	m_hTrailImage = m_pManager->GetTrailImage(m_CharaTag);
	m_pTrail->Init(m_hTrailImage > 0 ? m_hTrailImage : DX_NONE_GRAPH, 1.0f, 40.0f);
	m_pTrail->SetSubdivisions(4);

	m_Owner = owner;
	m_LastOwner = m_Owner;
	m_ChargeRate = chargeRate;

	m_IsThorwing = true;
}

void Ball::ThrowDirection(const Vector3& direction, Chara* owner, float chargeRate)
{
	Throw(owner, chargeRate);

	int chargeLevel = 0;

	if (chargeRate < 0.5f)
	{
		chargeLevel = 0;
	}
	else if (chargeRate < 1.0f)
	{
		chargeLevel = 1;
	}
	else
	{
		chargeLevel = 2;
	}

	m_Physics->velocity = direction * BALL_REF.ChargeLevels[chargeLevel].Speed;
}

void Ball::ThrowHoming(const std::shared_ptr<BallTarget>& target, Chara* owner, float chargeRate, float curveAngle, float curveScale)
{
	std::shared_ptr<BallTarget> temp = target;
	m_HomingTarget = std::move(temp);

	m_IsHoming			= true;
	m_DoRefreshHoming	= true;

	m_HomingOrigin		= transform->position;
	m_HormingCurveAngle	= curveAngle;
	m_HormingCurveScale	= curveScale;
	m_HomingProgress	= 0.0f;

	Vector3 direction = Vector3::Normalize((transform->position + transform->Forward() * 100.0f) - transform->position);
	
	ThrowDirection(direction, owner, chargeRate);
	m_Physics->SetIsActive(false);

	m_HomingSpeed = m_Physics->velocity.GetLength();
}

void Ball::CollisionEvent(const CollisionData& colData)
{
	if (not m_IsActive)
		return;

	if (colData.Other()->Parent<Catcher>() != nullptr)
		return;

	if (colData.Other()->Parent<Chara>() == m_Owner)
		return;

	if (colData.Other()->Tag() == ColDefine::Tag::tWindArea)
		return;

	if (m_State == S_THROWN)
	{
		if (m_IsHoming) homingDeactivate();

		m_Physics->velocity = m_Physics->FlatVelocity() * -10.0f + Vector3(0.0f, 200.0f, 0.0f);	// Magic:(

		changeState(S_LANDED);

		if (m_Owner && m_Owner->LastBall() == this)
		{
			m_Owner->SetLastBall(nullptr);
		}

		for (const auto& attribute : m_Attributes) {
			if (attribute != nullptr)
				attribute->OnHit();
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

			float e = BALL_REF.Bounciness;
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

void Ball::SetTexture(const BallTexture& texture, const std::string& mapKey)
{
	BallRenderer* ballRenderer = Object3D::GetComponent<BallRenderer>();
	if (ballRenderer == nullptr)
	{
		ballRenderer = Object3D::AddComponent<BallRenderer>();
		ballRenderer->InitVertices();
	}

	ballRenderer->SetTexture(texture, mapKey);
}

void Ball::SetTrailImage(int hImage)
{
	m_hTrailImage = hImage;
}

void Ball::SetOwner(Chara* pChara)
{
	m_Owner = pChara;
	m_LastOwner = m_Owner;
}

void Ball::PickUp()
{
	m_IsPickedUp = true;
}

void Ball::Knockback(const Vector3& other, float force_vertical, float force_horizontal)
{
	Vector3 otherToMe = transform->position - other;
	Vector3 impactNorm = otherToMe.Normalize();
	Vector3 impactVertical = Vector3(0.0f, impactNorm.y, 0.0f) * force_vertical;
	Vector3 impactHorizontal = Vector3(impactNorm.x, 0.0f, impactNorm.z) * force_horizontal;
	Vector3 impact = impactVertical + impactHorizontal;

	GetComponent<Physics>()->velocity += impact;
}

void Ball::collisionToGround()
{
	if (m_State == S_OWNED) return;

	Vector3 hitPos;
	bool hit = Stage::ColCheckGround(transform->position + Vector3::SetY(BALL_RADIUS), transform->position - Vector3::SetY(BALL_RADIUS), &hitPos);
	if (hit)
	{
		if (m_IsHoming) homingDeactivate();

		// Y方向に跳ね返る
		transform->position = hitPos + Vector3::SetY(BALL_RADIUS);
		m_Physics->velocity.y *= -BALL_REF.Bounciness;

		// 転がっていく処理
		float forwardRad = atan2f(m_Physics->velocity.x, m_Physics->velocity.z);
		transform->rotation.y = forwardRad;

		m_Physics->velocity.x *= 0.99f;
		m_Physics->velocity.z *= 0.99f;
		m_Physics->angularVelocity.x = m_Physics->FlatVelocity().GetLength() * 0.01f;

		changeState(S_LANDED);
	}
}

void Ball::changeState(const State& s)
{
	m_StatePrev = m_State;
	m_State = s;
}

void Ball::homingProcess()
{
	if (m_HomingTarget == nullptr) return;

	// ---- ホーミング補間 ----
	m_HomingTargetPos = m_HomingTarget->Position();

	const Vector3 diff = m_HomingTargetPos - m_HomingOrigin;
	const Vector3 dir = Vector3::Normalize(diff);
	const Vector3 dirRot = Vector3Util::DirToEuler(dir) + Vector3(0.0f, 0.0f, m_HormingCurveAngle);
	const MATRIX dirRotMat = (dirRot).ToRotationMatrix();

	const Vector3 middle = (m_HomingTargetPos + m_HomingOrigin) / 2.0f;
	const Vector3 middleToCurrent = transform->position - middle;

	const float distance = diff.GetLength();

	float delta = 0.0f;

	if (distance < 0.0f)
	{
		delta = 1.0f;
	}
	else
	{
		delta = m_HomingSpeed / (distance + ((distance * 0.5f * DX_PI_F) - distance) * m_HormingCurveScale) * GTime.DeltaTime();
	}

	m_HomingProgress += delta;

	if (m_HomingProgress >= 1.0f)
	{
		transform->position = m_HomingTargetPos;
		homingDeactivate();
	}
	else
	{
		const float circleY = sinf(m_HomingProgress * DX_PI_F) * m_HormingCurveScale;
		const float circleZ = -1 + m_HomingProgress * 2.0f;
		const Vector3 circlePos = Vector3(0.0f, circleY, circleZ) *
			distance * 0.5f *
			dirRotMat;

		m_Physics->velocity = (circlePos + middle) - transform->position;
		transform->position += m_Physics->velocity;
	}
}

bool Ball::collisionToStage()
{
	bool hit = false;

	if (m_Collider != nullptr && m_Collider->IsActive())
	{
		Vector3 p1 = transform->Global().position;
		Vector3 p2 = m_Collider->OffsetWorld();
		float radius = BALL_RADIUS;

		Vector3 pushVec;
		if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
		{
			hit = true;

			transform->position += pushVec;

			// 押し出し方向
			Vector3 normal = pushVec.Normalize();

			// 速度を反射させる
			const float bounciness = BALL_REF.Bounciness;
			const Vector3 vel = m_Physics->velocity;

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

	return hit;
}

void Ball::homingDeactivate()
{
	m_Physics->SetIsActive(true);
	m_Physics->SetGravity(GRAVITY);
	m_IsHoming = false;
	m_HomingTarget.reset();
}
