#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/collider/CollisionDefine.h"
#include <memory>

class ForceBase;
class ColliderBase;

/// <summary>
/// 範囲内のオブジェクトにルールに従って力を加える
/// </summary>
/// <author>佐藤紘斗</author>
class ForceFieldBase : public Object3D
{
public:
	ForceFieldBase();
	virtual ~ForceFieldBase();

	void SetColTag(const ColDefine::Tag& tag);
	void SetColTargetTags(const std::list<ColDefine::Tag>& targetTags);
	void SetForce(std::unique_ptr<ForceBase> force);

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	virtual void CollisionEvent(const CollisionData& colData) override;

protected:
	std::unique_ptr<ForceBase> m_Force;	// 力場の処理
	ColliderBase* m_pCollider;	// 当たり判定
};
