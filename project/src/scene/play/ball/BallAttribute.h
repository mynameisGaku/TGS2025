#pragma once

class Ball;

/// <summary>
/// ƒ{[ƒ‹‚É‚½‚¹‚é“ÁêŒø‰Ê
/// </summary>
/// <author>²“¡h“l</author>
class BallAttribute
{
public:
	BallAttribute(Ball* owner);
	~BallAttribute();

	virtual void Update() {}

	virtual void Throwing() {}
	virtual void OnHave() {}
	virtual void OnHit() {}
	virtual void OnGround() {}

protected:
	Ball* owner;

};
