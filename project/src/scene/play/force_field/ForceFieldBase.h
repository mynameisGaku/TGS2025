#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/collider/CollisionDefine.h"
#include <memory>

class ForceBase;
class ColliderBase;

class ForceFieldBase : public Object3D
{
public:
	ForceFieldBase();
	virtual ~ForceFieldBase();

	void SetColTag(const ColDefine::Tag& tag);
	void SetColTargetTags(const std::list<ColDefine::Tag>& targetTags);
	void SetForce(std::unique_ptr<ForceBase> force);

	/// <summary>
	/// “–‚½‚è”»’èˆ—
	/// </summary>
	/// <param name="colData">“–‚½‚è”»’èî•ñ</param>
	virtual void CollisionEvent(const CollisionData& colData) override;

protected:
	std::unique_ptr<ForceBase> m_Force;	// —Íê‚Ìˆ—
	ColliderBase* m_pCollider;	// “–‚½‚è”»’è
};
