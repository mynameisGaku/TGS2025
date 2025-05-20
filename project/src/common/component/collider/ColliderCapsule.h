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

	/// <summary>
	/// 終点の位置(相対座標)を取得する
	/// </summary>
	inline const Vector3 OffsetLocal() const { return offset; }

	/// <summary>
	/// 終点の位置(絶対座標)を取得する
	/// </summary>
	inline const Vector3 OffsetWorld() const { return transform->Global().position + offset; }

private:
	Vector3 offset;

};