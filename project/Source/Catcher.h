#pragma once
#include "Object3D.h"
#include "CharaDefine.h"

class ColliderCapsule;

/// <summary>
/// キャラクターの吸引キャッチの当たり判定のクラス
/// </summary>
/// <author>佐藤紘斗</author>
class Catcher : public Object3D
{
public:
	Catcher();
	~Catcher();

	void Init(CharaDefine::CharaTag tag);
	void Update() override;
	void Draw() override;

	void SetColliderActive(bool isActive);
private:
	ColliderCapsule* m_Collider;	// 当たり判定
};
