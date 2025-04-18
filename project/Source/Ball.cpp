#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"
#include "BallRef.h"
#include "Component/ColliderSphere.h"
#include "Stage.h"

// ToDo:ƒRƒŠƒWƒ‡ƒ“
namespace
{
	static const float BALL_RADIUS = 83.951f;
}

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1"));

	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(BALL_REF.GravityDefault, BALL_REF.FrictionDefault);

	m_Collider = Object3D::AddComponent<ColliderSphere>();

	ColDefine::ColBaseParam param;
	param.push = true;
	param.trs.scale = V3::ONE * BALL_RADIUS;
	// ToDo:tag•t‚¯‚é

	m_Collider->BaseInit(param);

	m_State = S_OWNED;
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
