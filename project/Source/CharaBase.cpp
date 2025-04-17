#include "CharaBase.h"
#include "InputManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Component/ColliderCapsule.h"
#include "Stage.h"

using namespace KeyDefine;

CharaBase::CharaBase()
{
}

CharaBase::~CharaBase()
{

}

void CharaBase::Update() {

	HitGroundProcess();

	Object3D::Update();
}

void CharaBase::CollisionEvent(const CollisionData& colData) {

	// 当たった相手がPlayerもしくはEnemyの場合
	if (colData.Other()->Tag() == ColDefine::Tag::tPlayer || colData.Other()->Tag() == ColDefine::Tag::tEnemy) {

		// 相手の情報
		CharaBase* chara = colData.Other()->Parent<CharaBase>();

		if (chara != nullptr) {
			Vector3 myPos = transform->Global().position;			// 自身の座標
			Vector3 otherPos = chara->transform->Global().position;	// 相手の座標

			Physics* myPhysics = GetComponent<Physics>();			// 自身の物理挙動
			Physics* otherPhysics = chara->GetComponent<Physics>();	// 相手の物理挙動

			ColliderCapsule* collider = GetComponent<ColliderCapsule>();	// 当たり判定

			if (myPhysics == nullptr || otherPhysics == nullptr || collider == nullptr)
				return;

			// 相手へ向かうベクトル
			Vector3 toVec = otherPos - myPos;
			if (toVec.SquareSize() == 0)
				toVec = Vector3(0, 0, 1);

			// 反発力の強さを定義
			constexpr float REPELLENT_FORCE_SCALE_MAX = 2.0f;
			constexpr float REPELLENT_FORCE_SCALE_MIN = 1.0f;
			const float REPELLENT_RADIUS = collider->Radius() * 4.0f;

			// 反発力
			const Vector3 repellentForce = toVec.Norm() * (REPELLENT_FORCE_SCALE_MIN + (REPELLENT_FORCE_SCALE_MAX - REPELLENT_FORCE_SCALE_MIN) * max(0.0f, 1.0f - toVec.Size() / REPELLENT_RADIUS));

			// 反発力を加える
			myPhysics->resistance -= repellentForce;
			otherPhysics->velocity += repellentForce;
		}
	}
}

void CharaBase::HitGroundProcess() {

	// 物理挙動
	Physics* physics = GetComponent<Physics>();
	if (physics == nullptr)
		return;

	Vector3 colliderGlobalPos1 = V3::ZERO;
	Vector3 colliderGlobalPos2 = transform->Global().position + V3::SetY(-0.5f);
	Vector3 hitPos;	// 接地点

	// 当たり判定
	ColliderCapsule* capsuleCol = GetComponent<ColliderCapsule>();

	if (capsuleCol == nullptr)
		colliderGlobalPos1 = physics->LastTransform()->Global().position;
	else
		colliderGlobalPos1 = capsuleCol->OffsetWorld();

	// 始点と終点の座標を渡して、地形と当たっているかを判定
	if (Stage::ColCheckGround(colliderGlobalPos1, colliderGlobalPos2, &hitPos)) {
		transform->position.y = hitPos.y;
		physics->velocity.y = 0.0f;
		physics->resistance.y = 0.0f;
		physics->SetGravity(V3::ZERO);
	}
}