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

	GravityUpdate();
	VelocityUpdate();
	ResistanceUpdate();
}

void Physics::GravityUpdate() {

	// 時間経過率
	const float timeLapseRate = GTime.DeltaTime();

	// 重力加速度を適応
	velocity += gravity * timeLapseRate;
}

void Physics::VelocityUpdate() {

	// 時間経過率
	const float timeLapseRate = GTime.DeltaTime();

	// 座標に加速度を適用
	pTransform->position += velocity * timeLapseRate;

	// 角速度を適用
	pTransform->rotation += angularVelocity * timeLapseRate;
}

void Physics::ResistanceUpdate() {

	// 時間経過率
	const float timeLapseRate = GTime.DeltaTime();

	//==========================================================================================
	// ▼加速度に落下時の空気抵抗を適応

	// 座標に抵抗力を適応
	pTransform->position += resistance * timeLapseRate;

	float airResistance = velocity.y - fallingAirResistance * timeLapseRate;	// 落下時の空気抵抗
	float velSize = velocity.GetLength();	// 現在の速度の大きさ
	float fricSize = friction.GetLength();	// 摩擦係数の大きさ

	// 摩擦係数を適応
	velSize = max(velSize - fricSize * timeLapseRate, 0.0f);
	
	velocity = velocity.Normalize() * velSize;
	velocity.y = airResistance;

	airResistance = resistance.y - fallingAirResistance * timeLapseRate;	// 落下時の空気抵抗

	//==========================================================================================
	// ▼抵抗力に落下時の空気抵抗を適応

	float res = resistance.GetLength();	// 現在の速度の大きさ

	// 空気抵抗を適応
	res = max(res - fricSize * timeLapseRate, 0.0f);

	resistance = resistance.Normalize() * res;
	resistance.y = res;
}
