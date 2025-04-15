#pragma once
#include "colliderBase.h"

/// <summary>
/// カプセル当たり判定
/// </summary>
class ColliderCapsule : public ColliderBase {
public:
	//==========================================================================================
	// ▼コンストラクタ

	ColliderCapsule();
	~ColliderCapsule() {};

	//==========================================================================================
	// ▼各種関数

	void Draw() override;

	//==========================================================================================
	// ▼セッター

	inline void SetOffset(Vector3 _offset) { offset = _offset; }

	//==========================================================================================
	// ▼ゲッター

	// 半径を取得する
	inline float Radius() const { return transform->Global().scale.Average() / 2.0f; }

	inline Vector3 Offset() const { return offset; }

private:
	Vector3 offset;

};