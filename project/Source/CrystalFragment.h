#pragma once
#include "object3d.h"
#include "Component/Physics.h"

/// <summary>
/// クリスタルから落ちる破片クラス
/// </summary>
class CrystalFragment : public Object3D
{
public:
    CrystalFragment();
    ~CrystalFragment();

    void Update() override;
    void Draw() override;

    /// <summary>
    /// 有効か？
    /// </summary>
    /// <returns>有効ならtrue</returns>
    bool IsActive() const { return m_IsActive; }
private:
    // お友達
    friend class CrystalFragmentManager;

    Vector3     m_FirstVelocity;    // 初速度 生成時にランダムで設定
    Physics*    m_pPhysics;         // 物理コンポーネント
    float       m_Lifetime;         // 寿命
    float       m_ElementPoint;     // 属性ポイント
    float       m_Size;             // 大きさ
    float       m_RotSpeed;         // Y軸回転速度
    bool        m_IsActive;         // 有効かどうか
};