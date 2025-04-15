#pragma once

// ◇汎用
#include "Vector3.h"
#include <list>

/// <summary> 
/// Transformの分類
/// </summary>
enum class TrsType {
	tNone = -1,
	tPos,	// 座標
	tRot,	// 回転
	tScale,	// 拡縮
	tAll,	// 全て
};

/// <summary>
/// 座標、回転、拡縮を持つクラス
/// </summary>
class Transform {
public:
	//==========================================================================================
	// ▼メンバ変数

	Vector3 position;	// 座標
	Vector3 rotation;	// 回転
	Vector3 scale;		// 拡縮

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	// トランスフォームの初期化
	Transform() : Transform(V3::ZERO, V3::ZERO, V3::ZERO, nullptr) {}

	/// <summary>
	/// トランスフォームの初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="rotation">回転</param>
	/// <param name="scale">拡縮</param>
	/// <param name="parent">親のトランスフォーム</param>
	Transform(const Vector3& position, const Vector3& rotation = V3::ZERO, const Vector3& scale = V3::ONE, Transform* parent = nullptr);

	~Transform();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 親を設定する
	/// </summary>
	/// <param name="parent">親にするトランスフォーム</param>
	void SetParent(Transform* parent);

	/// <summary>
	/// 指定したグローバルになるように、ローカルを変更する
	/// </summary>
	/// <param name="global">グローバル座標</param>
	void SetGlobal(const Transform& global);

	//================================================================================
	// ▼ゲッター

	// 親の影響を含めたグローバル情報を取得(親のXが10、子のXが20だったら子のグローバル座標はX30、みたいな)
	// 親が未設定なら意味なし
	const Transform Global() const;

	// 親のトランスフォーム
	const Transform Parent() const;

	// 自身の持つ座標、回転、拡縮情報から行列を取得
	// モデルの描画の時に使用
	const MATRIX Matrix() const;

	// 回転の行列だけ取得
	// キャラの向いてる方向をベクトルで欲しいときなどに使用
	const MATRIX RotationMatrix() const;

	// 回転を参照し、右を指す単位ベクトルを取得
	const Vector3 Right() const;

	// 回転を参照し、正面を指す単位ベクトルを取得
	const Vector3 Up() const;

	// 回転を参照し、上を指す単位ベクトルを取得
	const Vector3 Forward() const;

	// 回転を各軸0～2πの間に直して取得
	const Vector3 LimitedRotation() const;

	// 親が設定されていたらTrueを取得する
	inline const bool IsSetParent() const { return (parent != nullptr); }

	// 自身の複製品を取得する
	Transform* Copy() const;

	//================================================================================
	// ▼計算

	const Transform operator+(const Transform& trs) const;
	const Transform operator-(const Transform& trs) const;
	const Transform operator*(const float sca) const;
	const Transform operator/(const float sca) const;
	const Transform operator-() const;
	void operator+=(const Transform& trs);
	void operator-=(const Transform& trs);
	void operator*=(const float sca);

private:
	//================================================================================
	// ▼メンバ変数

	Transform* parent;	// 親トランスフォームのポインタ
	std::list<Transform*> children;	// 子トランスフォームのポインタのリスト

	//================================================================================
	// ▼各種関数

	// 子を追加
	inline void AddChild(Transform* child) { children.push_back(child); }

	// 子をリストから消す
	void RemoveChild(Transform* child);

	/// <summary>
	/// 親との連携を解除
	/// </summary>
	/// <param name="parent">親のポインタ</param>
	/// <param name="removeFromChildren">親の子供リストから消す</param>
	void RemoveParent(Transform* parent, bool removeFromChildren = true);
};
