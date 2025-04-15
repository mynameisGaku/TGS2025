#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"

// ToDo:ŠO•”‰»
namespace
{
	static const Vector3 GRAVITY_DEFAULT = Vector3(0, -9.8f, 0);
	static const Vector3 FRICTION_DEFAULT = Vector3(0, 0, 0);
	static const float FLOOR_Y = 0;
}

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/lizard.mv1"));

	m_Physics = Object3D::AddComponent<Physics>();
	m_Physics->Init(GRAVITY_DEFAULT, FRICTION_DEFAULT);
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
		transform->position.y = FLOOR_Y;
		SetVelocity(m_Physics->velocity * -0.5f);
	}
}

void Ball::Draw()
{
	Object3D::Draw();
}

void Ball::SetVelocity(const Vector3& velocity)
{
	m_Physics->velocity = velocity;
}

bool Ball::isHitFloor() const
{
	return Object3D::transform->position.y < FLOOR_Y;
}
