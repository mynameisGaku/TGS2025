#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"
#include "BallRef.h"

// ToDo:コリジョン
namespace
{
	static const float FLOOR_Y = 0;
	static const float BALL_RADIUS = 83.951f;
}

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1"));

	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(BALL_REF.GravityDefault, BALL_REF.FrictionDefault);

	m_State = S_OWNED;
}

Ball::~Ball()
{
}

void Ball::Update()
{
	Object3D::Update();

	// 床で跳ね返る（仮）
	if (isHitFloor())
	{
		transform->position.y = FLOOR_Y + BALL_RADIUS;
		setVelocity(m_Physics->velocity * -BALL_REF.BouncinessDefault);
	}
}

void Ball::Draw()
{
	Object3D::Draw();
}

void Ball::Throw(const Vector3& velocity)
{
	m_State = S_THROWN;
	setVelocity(velocity);
}

bool Ball::isHitFloor() const
{
	return (Object3D::transform->position.y - BALL_RADIUS) < FLOOR_Y;
}

void Ball::setVelocity(const Vector3& velocity)
{
	m_Physics->velocity = velocity;
}
