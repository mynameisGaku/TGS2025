#pragma once
#include "src/util/object3D/Object3D.h"
#include <memory>

class ForceBase;

class ForceFieldBase : public Object3D
{
public:
	ForceFieldBase();
	~ForceFieldBase();

	/// <summary>
	/// “–‚½‚è”»’èˆ—
	/// </summary>
	/// <param name="colData">“–‚½‚è”»’èî•ñ</param>
	void CollisionEvent(const CollisionData& colData) override;

protected:
	std::unique_ptr<ForceBase> m_force;	// —Íê‚Ìˆ—
};
