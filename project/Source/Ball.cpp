#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"
#include "BallRef.h"
#include "Component/ColliderCapsule.h"
#include "Stage.h"

namespace
{
	static const float BALL_MODEL_RADIUS = 83.951f;
	static const float BALL_RADIUS = 100.0f;
	static const float BALL_SCALE = BALL_RADIUS / BALL_MODEL_RADIUS;
}

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1"));

	transform->scale = V3::ONE * BALL_SCALE;
	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(BALL_REF.GravityDefault, BALL_REF.FrictionDefault);

	m_Collider = Object3D::AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;
	param.trs.scale = V3::ONE * BALL_MODEL_RADIUS * 2;
	param.tag = ColDefine::Tag::tPlayerAtk;
	param.targetTags = { ColDefine::Tag::tEnemy, ColDefine::Tag::tTerrain };
	m_Collider->SetOffset(V3::ZERO);

	m_Collider->BaseInit(param);
	m_Collider->SetDraw(true);

	m_State = S_OWNED;
	m_Owner = nullptr;
}

Ball::~Ball()
{
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
	m_Physics->velocity *= -1.0f;
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
