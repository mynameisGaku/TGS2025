#include "src/common/component/physics/Physics.h"

// ���ėp
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

	// �O�g�����X�t�H�[����ۑ�
	*lastTransform = *pTransform;

	updateMovementVelocity();
	updateResistanceForce();
	updateFriction();
	updateGravity();
}

void Physics::updateGravity() {

	// �d�͉����x��K��
	velocity += gravity * GTime.DeltaTime();
}

void Physics::updateMovementVelocity() {

	// ���W�ɉ����x��K��
	pTransform->position += velocity * GTime.DeltaTime();
	pTransform->rotation += angularVelocity * GTime.DeltaTime();
}

void Physics::updateResistanceForce() {

	// ���W�ɒ�R�͂�K��
	pTransform->position += resistance * GTime.DeltaTime();
}

void Physics::updateFriction() {

	const Vector3 multFriction = Vector3::Horizontal * friction + Vector3::UnitY;

	velocity *= multFriction;
	resistance *= multFriction;
}