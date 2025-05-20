#include "physics.h"

// ���ėp
#include "../../Library/time.h"
#include "../Object3D.h"

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

	Function::DeletePointer(lastTransform);
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

	float timeLapseRate = Time::LapseRate();

	// �O�g�����X�t�H�[����ۑ�
	*lastTransform = *pTransform;

	GravityUpdate();
	VelocityUpdate();
	ResistanceUpdate();
}

void Physics::GravityUpdate() {

	// ���Ԍo�ߗ�
	const float timeLapseRate = Time::LapseRate();

	// �d�͉����x��K��
	velocity += gravity * timeLapseRate;
}

void Physics::VelocityUpdate() {

	// ���Ԍo�ߗ�
	const float timeLapseRate = Time::LapseRate();

	// ���W�ɉ����x��K�p
	pTransform->position += velocity * timeLapseRate;

	// �p���x��K�p
	pTransform->rotation += angularVelocity * timeLapseRate;
}

void Physics::ResistanceUpdate() {

	// ���Ԍo�ߗ�
	const float timeLapseRate = Time::LapseRate();

	//==========================================================================================
	// �������x�ɗ������̋�C��R��K��

	// ���W�ɒ�R�͂�K��
	pTransform->position += resistance * timeLapseRate;

	float airResistance = velocity.y - fallingAirResistance * timeLapseRate;	// �������̋�C��R
	float velSize = velocity.Size();	// ���݂̑��x�̑傫��
	float fricSize = friction.Size();	// ���C�W���̑傫��

	// ���C�W����K��
	velSize = max(velSize - fricSize * timeLapseRate, 0.0f);
	
	velocity = velocity.Norm() * velSize;
	velocity.y = airResistance;

	airResistance = resistance.y - fallingAirResistance * timeLapseRate;	// �������̋�C��R

	//==========================================================================================
	// ����R�͂ɗ������̋�C��R��K��

	float res = resistance.Size();	// ���݂̑��x�̑傫��

	// ��C��R��K��
	res = max(res - fricSize * timeLapseRate, 0.0f);

	resistance = resistance.Norm() * res;
	resistance.y = res;
}
