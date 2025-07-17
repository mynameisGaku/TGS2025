#pragma once
#include "src/util/object3D/Object3D.h"
#include <string>

class ColliderCapsule;
class Chara;

class Tackler : public Object3D
{
public:
    Tackler();
    ~Tackler();

	void Init(std::string tag);
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void CollisionEvent(const CollisionData& colData) override;

	void SetColliderActive(bool isActive);
	void SetParent(Chara* parent) { m_Parent = parent; }

	bool IsColliderActive() const;
private:
	ColliderCapsule* m_Collider;	// 当たり判定
	Chara* m_Parent;			// 親キャラクター
};