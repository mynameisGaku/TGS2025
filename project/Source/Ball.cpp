#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"

// ToDo:ŠO•”‰»
namespace
{
	static const Vector3 GRAVITY_DEFAULT = Vector3(0, -9.8f, 0);
	static const Vector3 FRICTION_DEFAULT = Vector3(0, 0, 0);
	static const float FLOOR_Y = 0;
	static const float BALL_RADIUS = 83.951f;
}

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1"));

	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(GRAVITY_DEFAULT, FRICTION_DEFAULT);

	m_State = S_OWNED;
}

Ball::~Ball()
{
}

void Ball::Update()
{
	Object3D::Update();

	// °‚Å’µ‚Ë•Ô‚éi‰¼j
	if (isHitFloor())
	{
		transform->position.y = FLOOR_Y + BALL_RADIUS;
		setVelocity(m_Physics->velocity * -0.5f);
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
