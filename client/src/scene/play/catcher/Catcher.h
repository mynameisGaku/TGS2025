#pragma once
#include "src/util/object3D/Object3D.h"
#include <string>

class ColliderCapsule;
class Chara;
class Ball;
class ForceFieldManager;
class ForceFieldCorn;

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
	void SetParent(Chara* parent) { m_pParent = parent; }

	bool IsColliderActive() const;
	// キャッチ角度内にボールがあるかどうか
	bool CanCatch(Ball* ball) const;

	// キャッチ成功処理
	void CatchSuccese(Ball* ball);

private:
	ColliderCapsule* m_pCollider;	// 当たり判定
	Chara* m_pParent;			// 親キャラクター
	ForceFieldManager* m_pForceFieldManager;	// 吸込み用、力場マネージャー
	ForceFieldCorn* m_pWindArea;	// 吸込みエリア

	void createWindArea();
	void deleteWindArea();
};
