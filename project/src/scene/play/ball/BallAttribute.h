#pragma once

class Ball;

/// <summary>
/// �{�[���Ɏ�������������
/// </summary>
/// <author>�����h�l</author>
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
