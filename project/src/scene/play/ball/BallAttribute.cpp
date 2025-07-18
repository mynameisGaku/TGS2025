#include "src/scene/play/ball/BallAttribute.h"
#include "src/scene/play/ball/Ball.h"

BallAttribute::BallAttribute(Ball* owner)
{
	this->owner = owner;
}

BallAttribute::~BallAttribute()
{
}
