#include "colliderBase.h"

// ◇汎用
#include "../Util/Utils.h"

// ◇演出・機能
#include "CollisionManager.h"
#include "../Object3D.h"

ColliderBase::ColliderBase() {

	transform = nullptr;
	lastGlobal = nullptr;
	hittedData = nullptr;

	shape = ColDefine::Shape::sNone;
	tag = ColDefine::Tag::tNone;

	isActive = false;
	isDraw = false;
}

ColliderBase::~ColliderBase() {

	Function::DeletePointer(transform);
	Function::DeletePointer(lastGlobal);
	Function::DeletePointer(hittedData);

	CollisionManager* colM = FindGameObject<CollisionManager>();
	if (colM != nullptr)
		colM->Remove(this);
}

void ColliderBase::BaseInit(const ColDefine::ColBaseParam& param) {

	transform = new Transform(param.trs);
	lastGlobal = new Transform(param.trs);

	Object3D* obj = Parent<Object3D>();
	if (obj != nullptr)
		transform->SetParent(obj->transform);

	tag = param.tag;
	targetTags = param.targetTags;

	isActive = true;

	if (param.onlyOnce)
		hittedData = new HittedData;

	CollisionManager* colM = FindGameObject<CollisionManager>();
	if (colM != nullptr)
		colM->Add(this);
}

void ColliderBase::UpdateData() {

	if (lastGlobal != nullptr) {
		*lastGlobal = transform->Global();
	}
}

bool ColliderBase::IsTarget(ColDefine::Tag otherTag) {

	for (const auto& itr : targetTags) {
		if (itr == otherTag)
			return true;
	}

	return false;
}

bool HittedData::IsHitted(ColliderBase const* other) {

	for (ColliderBase const* hit : hitted) {
		if (hit == other)
			return true;
	}

	return false;
}
