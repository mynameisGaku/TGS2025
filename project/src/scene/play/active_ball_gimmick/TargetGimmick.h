#pragma once
#include "interfaces/TargetBase.h"

class TargetGimmick : public TargetBase
{
public:
	void Update() override;
	void Draw() override;
	void OnBallHit(Ball* pBall) override;
	void OnCharaContact(Chara* pChara) override;
	void CollisionEvent(const CollisionData& colData) override;
	void SpawnReward() override;
private:

};