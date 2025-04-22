#pragma once
#include "crystalfragment.h"
#include "Pool.h"

#define USE_POOL

/// <summary>
/// クリスタルの破片を管理するクラス
/// </summary>
class CrystalFragmentManager : public GameObject
{
public:
    CrystalFragmentManager();
    ~CrystalFragmentManager();

    void Update() override;
    void Draw() override;

    /// <summary>
    /// 破片を生成する。
    /// 生成された破片は、ランダムな方向に飛んでいきます。
    /// </summary>
    /// <param name="pos">生成場所</param>
    void CreateFragment(const Vector3& pos);

private:
    CrystalFragment* initfunc(uint32_t index, CrystalFragment* pFrag);

#ifdef USE_POOL
    Pool<CrystalFragment>* m_pPool;
#else
    std::list<CrystalFragment*> m_CrystalFragments; // 破片リスト
#endif
};