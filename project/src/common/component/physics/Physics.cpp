#include "src/common/component/physics/Physics.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/object3D/Object3D.h"
#include "src/util/ptr/PtrUtil.h"

Physics::Physics() {

	pTransform = nullptr;
	lastTransform = nullptr;

	velocity = Vector3::Zero;
	angularVelocity = Vector3::Zero;
	
	gravity = GRAVITY;
	friction = FRICTION;

	resistance = Vector3::Zero;
	fallingAirResistance = 0.0f;
}

Physics::~Physics() {
    lastTransform->SetParent(nullptr);
	PtrUtil::SafeDelete(lastTransform);
}

void Physics::Init(const Vector3& _gravity, const Vector3& _friction) {

	pTransform = dynamic_cast<Object3D*>(parent)->transform;
	lastTransform = new Transform(*pTransform);

	velocity = Vector3::Zero;
	angularVelocity = Vector3::Zero;
	fallingAirResistance = 0.0f;

	SetGravity(_gravity);
	SetFriction(_friction);
}

void Physics::Update() {

	if (IsActive() == false)
		return;

	float timeLapseRate = GTime.DeltaTime();

	// 前トランスフォームを保存
	*lastTransform = *pTransform;

	updateMovementVelocity();
	updateResistanceForce();
	updateFriction();
	updateGravity();
}

void Physics::updateGravity() {

	// 重力加速度を適応
	velocity += gravity * GTime.DeltaTime();
}

void Physics::updateMovementVelocity() {

	// 座標に加速度を適応
	pTransform->position += velocity * GTime.DeltaTime();
	pTransform->rotation += angularVelocity * GTime.DeltaTime();
}

void Physics::updateResistanceForce() {

	// 座標に抵抗力を適応
	pTransform->position += resistance * GTime.DeltaTime();
}

void Physics::updateFriction() {

	const Vector3 multFriction = Vector3::Horizontal * friction + Vector3::UnitY;

	velocity *= multFriction;
	resistance *= multFriction;
}