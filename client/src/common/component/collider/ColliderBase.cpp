#include "src/common/component/collider/ColliderBase.h"

// ◇汎用
#include <src/util/ptr/PtrUtil.h>

// ◇演出・機能
#include "src/common/component/collider/CollisionManager.h"
#include "src/util/object3D/Object3D.h"

ColliderBase::ColliderBase() {

	transform = nullptr;
	global = nullptr;
	hittedData = nullptr;

	shape = ColDefine::Shape::sNone;
	tag = ColDefine::Tag::tNone;

	isActive = false;
	isDraw = false;
}

ColliderBase::~ColliderBase() {

	PtrUtil::SafeDelete(transform);
	PtrUtil::SafeDelete(global);
	PtrUtil::SafeDelete(hittedData);

	CollisionManager* colM = FindGameObject<CollisionManager>();
	if (colM != nullptr)
		colM->Remove(this);
}

void ColliderBase::BaseInit(const ColDefine::ColBaseParam& param) {

	if (transform == nullptr)
		transform = new Transform(param.trs);
	else
		*transform = param.trs;

	if (global == nullptr)
		global = new Transform(param.trs);
	else
        *global = param.trs;

	Object3D* obj = Parent<Object3D>();
	if (obj != nullptr)
		transform->SetParent(obj->transform);

	tag = param.tag;
	targetTags = param.targetTags;

	isActive = true;

	if (param.onlyOnce)
	{
		if (hittedData == nullptr)
			hittedData = new HittedData;
	}

	CollisionManager* colM = FindGameObject<CollisionManager>();
	if (colM != nullptr)
		colM->Add(this);
}

void ColliderBase::UpdateData() {
	if (global != nullptr) {
		*global = transform->Global();
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
