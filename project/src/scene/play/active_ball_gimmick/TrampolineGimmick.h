#pragma once
#include "interfaces/TrampolineBase.h"
#include <src/common/component/collider/CollisionData.h>
#include <src/util/transform/Transform.h>
#include <src/common/component/collider/CollisionDefine.h>

struct TRAMPOLINE_DESC
{
    Transform   transform{};
    float       repulsion_force{}; // 反発力 (1で減衰なし)
};

class Physics;

/// <summary>
/// トランポリンギミッククラス
/// </summary>
class TrampolineGimmick : public TrampolineBase
{
public:
    TrampolineGimmick();
    ~TrampolineGimmick();
    void Init(const TRAMPOLINE_DESC& desc, const ColDefine::ColBaseParam& col);
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
	float m_BounceCooldown = 0.2f; // 秒数（連続反射禁止）
	float m_BounceTimer = 0.0f;
};