#include "src/common/component/motion_blur/MotionBlur.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/object3D/Object3D.h"

namespace {

	static const float MOTION_BLUR_DURATION = 0.5f;			// モーションブラーの効果時間
	static const float MOTION_BLUR_PLAY_INTERVAL = 0.1f;	// モーションブラーの再生間隔
}

MotionBlur::MotionBlur() {

	counter = 0.0f;
	isActive = false;
}

MotionBlur::~MotionBlur() {

	AllDeleteMotionBlur();
}

void MotionBlur::Init() {

	counter = 0.0f;
	isActive = false;
}

void MotionBlur::Update() {

	for (auto itr = motionBlur.begin(); itr != motionBlur.end();) {
		itr->duration -= GTime.deltaTime;
		if (itr->duration <= 0.0f) {
			itr = motionBlur.erase(itr);
			if (itr == motionBlur.end()) break;
		}
		itr++;
	}

	counter = max(counter - GTime.deltaTime, 0.0f);

	if (!isActive || parent == nullptr)
		return;

	if (counter <= 0.0f) {
		counter = MOTION_BLUR_PLAY_INTERVAL;

		Object3D* pObject3D = dynamic_cast<Object3D*>(parent);
		Transform trs = *pObject3D->transform->Copy();
		int model = MV1DuplicateModel(pObject3D->Model());

		motionBlur.push_back(MotionBlurInfo(trs, model, MOTION_BLUR_DURATION));
	}
}

void MotionBlur::Draw() {

	if (parent == nullptr)
		return;

	Object3D* pObject3D = dynamic_cast<Object3D*>(parent);

	for (const auto& itr : motionBlur) {
		if (itr.totalDuration != 0.0f)
			MV1SetOpacityRate(pObject3D->Model(), 0.75f * (itr.duration / itr.totalDuration));

		MV1SetMatrix(itr.model, itr.transform.Global().Matrix());	// MATRIXをモデルに適応
		MV1DrawModel(itr.model);		// モデルの描画
	}

	if (motionBlur.size() > 0)
		MV1SetOpacityRate(pObject3D->Model(), 1.0f);
}

void MotionBlur::SetMotionBlur(bool value) {

	isActive = value;
}

void MotionBlur::AllDeleteMotionBlur() {

	for (auto itr = motionBlur.begin(); itr != motionBlur.end();) {
		itr = motionBlur.erase(itr);
		if (itr == motionBlur.end()) break;
	}
}
