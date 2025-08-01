#pragma once
// ◇継承元
#include "src/util/object3D/Object3D.h"

// ◇汎用
#include "src/util/easing/Easing.h"
#include "src/util/transform/Transform.h"
#include "src/util/transform/RectTransform.h"

// ◇個別で必要な物
#include "src/util/fx/effect/EffectDefine.h"

/// <summary>
/// エフェクトを再生するクラス
/// </summary>
class EffectBase : public Object3D {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	EffectBase();
	~EffectBase();

	//================================================================================
	// ▼各種関数

	void Update() override;

	/// <summary>
	/// 3Dエフェクトを再生する
	/// </summary>
	/// <param name="info">エフェクトの情報</param>
	/// <param name="transform">座標・回転・拡縮の情報</param>
	/// <param name="label">判別ラベル</param>
	/// <param name="isLoop">ループ再生を行うか</param>
	void Play3D(const EffectDefine::EffectInfo& info, const Transform& transform, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// 2Dエフェクトを再生する
	/// </summary>
	/// <param name="info">エフェクトの情報</param>
	/// <param name="transform">座標・回転・拡縮の情報</param>
	/// <param name="label">判別ラベル</param>
	/// <param name="isLoop">ループ再生を行うか</param>
	void Play2D(const EffectDefine::EffectInfo& info, const RectTransform& transform, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// エフェクトの再生を止める
	/// </summary>
	void Stop();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 座標・回転・拡縮を設定する
	/// </summary>
	/// <param name="transform">座標・回転・拡縮の情報</param>
	void SetTransform3D(const Transform& transform);
	void SetTransform2D(const RectTransform& transform);

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos">座標の情報</param>
	void SetPosition3D(const Vector3& pos);
	void SetPosition2D(const Vector2& pos);

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rot">回転の情報</param>
	void SetRotation3D(const Vector3& rot);
	void SetRotation2D(float rot);

	/// <summary>
	/// 拡縮を設定する
	/// </summary>
	/// <param name="scale">拡縮の情報</param>
	void SetScale3D(const Vector3& scale);
	void SetScale2D(const Vector2& scale);

	/// <summary>
	/// 再生速度を設定する
	/// </summary>
	/// <param name="speed">再生速度</param>
	void SetPlaySpeed(const float& speed);

	/// <summary>
	/// 色と不透明度を設定する
	/// </summary>
	/// <param name="r">赤</param>
	/// <param name="g">緑</param>
	/// <param name="b">青</param>
	/// <param name="a">不透明度</param>
	void SetRGBA(const int& r, const int& g, const int& b, const int& a);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// エフェクトが再生されているかを取得する
	/// </summary>
	bool IsPlaying() const;

	/// <summary>
	/// 自身が持っているエフェクトの情報を取得する
	/// </summary>
	inline EffectDefine::EffectInfo Info() const { return info; }

	/// <summary>
	/// 判別ラベルを取得する
	/// </summary>
	inline std::string Label() const { return label; }

	/// <summary>
	/// 整合性チェックを行う
	/// </summary>
	/// <param name="typeName">名前</param>
	/// <param name="label">判別ラベル</param>
	/// <returns>合っていればtrue</returns>
	bool CheckConsistency(const std::string& typeName, const std::string& label) const;

private:
	//================================================================================
	// ▼メンバ変数

	EffectDefine::EffectInfo info;	// 自身のエフェクト情報
	std::string label;				// 判別ラベル

	EasingInt alphaEasing;
};