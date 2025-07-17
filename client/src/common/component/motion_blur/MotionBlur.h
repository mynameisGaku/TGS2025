#pragma once

// ◇継承元
#include "src/common/component/Component.h"

// ◇汎用
#include "src/util/transform/Transform.h"
#include <vector>

/// <summary>
/// モーションブラーの情報
/// </summary>
struct MotionBlurInfo {
	Transform transform;	// 座標・回転・拡縮
	int model;				// モデル
	float duration;			// 効果時間
	float totalDuration;	// 総効果時間

	MotionBlurInfo() : 
		transform(Transform()),
		model(-1),
		duration(0.0f),
		totalDuration(0.0f) {
	}
	MotionBlurInfo(Transform trs, int _model, float _totalDuration) :
		transform(trs),
		model(_model),
		duration(_totalDuration),
		totalDuration(_totalDuration) {
	}
};

/// <summary>
/// モーションブラーに関するクラス
/// </summary>
class MotionBlur : public Component {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	MotionBlur();
	~MotionBlur();

	//================================================================================
	// ▼各種関数

	void Init();
	void Update() override;
	void Draw() override;

	// モーションブラーを起動する
	void SetMotionBlur(bool value);

private:
	//================================================================================
	// ▼プライベート関数

	void AllDeleteMotionBlur();

	std::vector<MotionBlurInfo> motionBlur;	// モーションブラーの情報
	float counter;	// カウンター
	bool isActive;	// モーションブラーが効果中か
};

