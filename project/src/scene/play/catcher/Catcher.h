#pragma once
#include "src/util/object3D/Object3D.h"
#include <string>

class ColliderCapsule;
class CharaBase;
class Ball;

/// <summary>
/// キャラクターの吸引キャッチの当たり判定のクラス
/// </summary>
/// <author>佐藤紘斗</author>
class Catcher : public Object3D
{
public:
	Catcher();
	~Catcher();

	void Init(std::string tag);
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void CollisionEvent(const CollisionData& colData) override;

	void SetColliderActive(bool isActive);
	void SetParent(CharaBase* parent) { m_Parent = parent; }

	bool IsColliderActive() const;
	// キャッチ角度内にボールがあるかどうか
	bool CanCatch(Ball* ball) const;
private:
	ColliderCapsule* m_Collider;	// 当たり判定
	CharaBase* m_Parent;			// 親キャラクター
};
