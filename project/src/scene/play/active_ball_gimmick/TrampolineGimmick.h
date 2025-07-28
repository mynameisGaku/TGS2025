#pragma once
#include "interfaces/TrampolineBase.h"
#include <src/common/component/collider/CollisionData.h>

typedef struct 
{
    Transform   transform{};
    float       repulsion_force{}; // 反発力 (1で減衰なし)
}TRAMPOLINE_DESC;

class Physics;

/// <summary>
/// トランポリンギミッククラス
/// </summary>
class TrampolineGimmick : public TrampolineBase
{
public:
    TrampolineGimmick();
    ~TrampolineGimmick();
    void Init(const TRAMPOLINE_DESC& desc);
    void Update() override;
    void Draw() override;
    void CollisionEvent(const CollisionData& colData) override;
    void OnBallHit(Ball* pBall) override;
    void OnCharaContact(Chara* pChara) override;
    void ReflectBall(Ball* pBall) override;
    void ReflectChara(Chara* pChara) override;
private:
    enum class type
    {
        chara,
        ball
    };
    void bounce(Physics* pPhys, const type& _type);
    float m_RepulsionForce = 0.0f;
    Vector3 m_SurfaceNormal;
};