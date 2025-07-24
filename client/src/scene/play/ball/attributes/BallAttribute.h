#pragma once

class Ball;

/// <summary>
/// �{�[���Ɏ������������ʁB�{�[���̏������A�������A�q�b�g���A���e���ɉ������������s���B
/// </summary>
/// <author>�����h�l</author>
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
