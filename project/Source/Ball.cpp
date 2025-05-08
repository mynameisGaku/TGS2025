#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"
#include "BallRef.h"
#include "Component/ColliderCapsule.h"
#include "Stage.h"
#include "BloomManager.h"
#include "CharaDefineRef.h"
#include "CharaBase.h"
#include "EffectManager.h"

namespace
{
	static const float BALL_MODEL_RADIUS = 83.951f;
	static const float BALL_RADIUS = 30.0f;
	static const float BALL_SCALE = BALL_RADIUS / BALL_MODEL_RADIUS;
	static const float BALL_COLOR_RADIUS = 90.0f;
}

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1"));

	transform->scale = V3::ONE * BALL_SCALE;
	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(BALL_REF.GravityDefault, BALL_REF.FrictionDefault);

	m_State = S_OWNED;
	m_Owner = nullptr;
	m_Collider = nullptr;
	m_CharaTag = CHARADEFINE_REF.Tags[0];
}

Ball::~Ball()
{
	if (m_Owner != nullptr && m_Owner->LastBall() == this)
	{
		m_Owner->SetLastBall(nullptr);
	}
}

void Ball::Init(std::string charaTag)
{
	m_Collider = Object3D::AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;
	param.trs.scale = V3::ONE * BALL_COLOR_RADIUS / BALL_SCALE * 2;

	if (charaTag == "Red")
	{
		param.tag = ColDefine::Tag::tBallRed;
		param.targetTags = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain };
	}
	else if (charaTag == "Blue")
	{
		param.tag = ColDefine::Tag::tBallBlue;
		param.targetTags = { ColDefine::Tag::tCharaRed, ColDefine::Tag::tCatchRed, ColDefine::Tag::tTerrain };
	}
	else
	{
		param.tag = ColDefine::Tag::tBallRed;
		param.targetTags = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain };
	}

	m_Collider->SetOffset(V3::ZERO);
	m_Collider->BaseInit(param);
	m_CharaTag = charaTag;
}

void Ball::Update()
{
	Object3D::Update();

	collisionToGround();
}

void Ball::Draw()
{
	Object3D::Draw();
}

void Ball::Throw(const Vector3& velocity)
{
	m_State = S_THROWN;
	setVelocity(velocity * BALL_REF.SpeedDefault);
	m_Owner = nullptr;
}

void Ball::Throw(const Vector3& velocity, CharaBase* owner)
{
	Throw(velocity);
	m_Owner = owner;
}

void Ball::CollisionEvent(const CollisionData& colData)
{
	if (m_State == S_THROWN)
	{
		m_Physics->velocity = m_Physics->FlatVelocity() * -0.5f + Vector3(0, 20, 0);
		m_State = S_LANDED;
		if (m_Owner->LastBall() == this)
		{
			if (m_CharaTag == "Blue")
			{
				EffectManager::Play3D("Hit_Blue.efk", *transform->Copy(), "Hit_Blue" + m_CharaTag);
			}
			else
			{
				EffectManager::Play3D("Hit_Red.efk", *transform->Copy(), "Hit_Red" + m_CharaTag);
			}

			m_Owner->SetLastBall(nullptr);
		}
	}
}

void Ball::collisionToGround()
{
	Vector3 hitPos;
	bool hit = Stage::ColCheckGround(transform->position + V3::SetY(BALL_RADIUS), transform->position - V3::SetY(BALL_RADIUS), &hitPos);
	if (hit)
	{
		// Y•ûŒü‚É’µ‚Ë•Ô‚é
		transform->position = hitPos + V3::SetY(BALL_RADIUS);
		m_Physics->velocity.y *= -BALL_REF.BouncinessDefault;

		// “]‚ª‚Á‚Ä‚¢‚­ˆ—
		float forwardRad = atan2f(m_Physics->velocity.x, m_Physics->velocity.z);
		transform->rotation.y = forwardRad;

		m_Physics->velocity.x *= 0.99f;
		m_Physics->velocity.z *= 0.99f;
		m_Physics->angularVelocity.x = m_Physics->FlatVelocity().Size() * 0.01f;
	}
}

void Ball::setVelocity(const Vector3& velocity)
{
	m_Physics->velocity = velocity;
}
