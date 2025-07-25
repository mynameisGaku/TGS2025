#pragma once

// ◇継承元
#include "src/common/component/Component.h"

// ◇汎用
#include "src/util/math/MathUtil.h"
#include "src/util/transform/Transform.h"
#include "src/util/math/Vector3.h"

namespace {

	// 既定の重力加速度
	static const Vector3 GRAVITY = Vector3::SetY(MathUtil::GRAVITY_3D);

	// 既定の摩擦係数
	static const Vector3 FRICTION = Vector3(0.9f);
}

/// <summary>
/// 速度を管理するコンポーネント
/// </summary>
class Physics : public Component {
public:
	//==========================================================================================
	// ▼メンバ変数

	// 速度。毎フレーム速度分だけ座標を移動する
	Vector3 velocity;

	// 角速度。毎フレーム速度分だけ回転する
	Vector3 angularVelocity;

	// 抵抗力。速度に影響されない移動を行う時に使用する
	Vector3 resistance;

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	Physics();
	~Physics();

	//==========================================================================================
	// ▼各種関数

	/// <summary>
	/// 初期化処理を行う
	/// </summary>
	/// <param name="gravity">重力加速度</param>
	/// <param name="friction">摩擦係数</param>
	void Init(const Vector3& gravity, const Vector3& friction);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

	/**
		@brief		重力加速度処理の更新
	*/
	void updateGravity();

	/**
		@brief		移動速度処理の更新
	*/
	void updateMovementVelocity();

	/**
		@brief		抵抗力処理の更新
	*/
	void updateResistanceForce();

	/**
		@brief		摩擦係数処理の更新
	*/
	void updateFriction();

public:

public:

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 重力加速度を設定する
	/// </summary
	inline void SetGravity(const Vector3& _gravity) { gravity = _gravity; }

	/// <summary>
	/// 摩擦係数を設定する
	/// </summary>
	inline void SetFriction(const Vector3& _friction) { friction = _friction; }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 1フレーム前のトランスフォームのコピーを取得する
	/// </summary>
	inline const Transform* const LastTransform() const { return lastTransform; }

	/// <summary>
	/// 重力加速度を取得する
	/// </summary>
	inline Vector3 const Gravity() const { return gravity; }

	/// <summary>
	/// 摩擦係数を取得する
	/// </summary>
	inline Vector3 const Friction() const { return friction; }

	/// <summary>
	/// 水平方向の速度を取得する
	/// </summary>
	inline Vector3 const FlatVelocity() const { return Vector3(velocity.x, 0.0f, velocity.z); }

	/// <summary>
	/// 垂直方向の速度を取得する
	/// </summary>
	inline Vector3 const UpVelocity() const { return Vector3(0.0f, velocity.y, 0.0f); }

private:
	//==========================================================================================
	// ▼メンバ変数

	// リンクするトランスフォームのポインタ
	// 速度に応じてリンクしたトランスフォームの座標を毎フレーム変化させる
	// このクラス内でnew、deleteはしない
	Transform* pTransform;

	// 1フレーム前のトランスフォームのコピー
	// 当たり判定などで、フレーム間の補間に使う
	Transform* lastTransform;

	// 重力加速度。フレーム毎に速度に加算される
	// 普通の下向き重力ならyだけマイナスにして他を0にすれば良い
	Vector3 gravity;

	// 摩擦係数
	Vector3 friction;

	// 落下時の空気抵抗
	float fallingAirResistance;
};
