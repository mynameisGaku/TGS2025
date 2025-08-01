#pragma once
#include "src/common/component/collider/ColliderBase.h"

/// <summary>
/// 球形当たり判定
/// </summary>
class ColliderSphere : public ColliderBase {
public:
	//==========================================================================================
	// ▼コンストラクタ

	ColliderSphere();
	~ColliderSphere() {};

	//==========================================================================================
	// ▼各種関数

	void Draw() override;

	//==========================================================================================
	// ▼ゲッター

	// 半径を取得する
	inline float Radius() const { return transform->Global().scale.x / 2.0f; }

};