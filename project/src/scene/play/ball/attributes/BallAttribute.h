#pragma once

class Ball;

/// <summary>
/// ボールに持たせる特殊効果。ボールの所持中、投擲中、ヒット時、着弾時に応じた処理を行う。
/// </summary>
/// <author>佐藤紘斗</author>
class BallAttribute
{
public:
	BallAttribute(Ball* owner);
	virtual ~BallAttribute();

	virtual void Update() {}

	virtual void Throwing() {}
	virtual void OnHave() {}
	virtual void OnHit() {}
	virtual void OnGround() {}

protected:
	Ball* owner;

};
