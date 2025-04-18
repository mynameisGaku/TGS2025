#pragma once
#include "crystalfragment.h"

/// <summary>
/// クリスタルの破片を管理するクラス
/// </summary>
class CrystalFragmentManager : public GameObject
{
public:
    CrystalFragmentManager();
    ~CrystalFragmentManager();

    void Update() override;

    /// <summary>
    /// 破片を生成する。
    /// 生成された破片は、ランダムな方向に飛んでいきます。
    /// </summary>
    /// <param name="pos">生成場所</param>
    void CreateFragment(const Vector3& pos);

private:
    std::list<CrystalFragment*> m_CrystalFragments; // 破片リスト
};