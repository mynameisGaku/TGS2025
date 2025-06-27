#pragma once
#include "src/scene/play/force_field/ForceFieldBase.h"

class ForceFieldSphere : public ForceFieldBase
{
public:
	ForceFieldSphere(float radius);
	~ForceFieldSphere() {}
};
