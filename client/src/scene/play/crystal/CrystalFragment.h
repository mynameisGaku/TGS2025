#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/physics/Physics.h"

/// <summary>
/// クリスタルから落ちる破片クラス
/// </summary>
class CrystalFragment : public Object3D
{
public:
    enum Element
    {
        eNONE = 0,
        eFIRE,
        eICE,
        eLIGHTNING,
        eWIND,
        eEARTH,
        eMAX
    };
    enum Size
    {
        sSMALL = 0,
        sMIDDLE,
        sLARGE,
        sMAX
    };
public:
    CrystalFragment(uint32_t index = -1, const Element& elem = eNONE, const Size& size = sSMALL);
    void SetRandFirstVelocity();
    ~CrystalFragment();

    void Init(const Element& elem = eNONE, const Size& size = sSMALL);

    void Update() override;
    void Draw() override;

    /// <summary>
    /// 有効か？
    /// </summary>
    /// <returns>有効ならtrue</returns>
    bool IsActive() const { return m_IsActive; }

    void SetIsActive(bool flag) { m_IsActive = flag; }
private:
    // お友達
    friend class CrystalFragmentManager;

    Vector3         m_FirstVelocity;    // 初速度 生成時にランダムで設定
    Physics*        m_pPhysics;         // 物理コンポーネント
    float           m_Lifetime;         // 寿命
    float           m_ElementPoint;     // 属性ポイント
    float           m_Size;             // 大きさ
    float           m_RotSpeed;         // Y軸回転速度
    uint32_t        m_Index;            // 自分のインデックス
    bool            m_IsActive;         // 有効かどうか
    Element         m_Element;          // 属性
};

typedef CrystalFragment::Element CrystalElement;
typedef CrystalFragment::Size    CrystalSize;