#pragma once
#include "crystalfragment.h"
#include "Pool.h"

#define USE_POOL

/// <summary>
/// �N���X�^���̔j�Ђ��Ǘ�����N���X
/// </summary>
class CrystalFragmentManager : public GameObject
{
public:
    CrystalFragmentManager();
    ~CrystalFragmentManager();

    void Update() override;
    void Draw() override;

    /// <summary>
    /// �j�Ђ𐶐�����B
    /// �������ꂽ�j�Ђ́A�����_���ȕ����ɔ��ł����܂��B
    /// </summary>
    /// <param name="pos">�����ꏊ</param>
    void CreateFragment(const Vector3& pos);

private:
    CrystalFragment* initfunc(uint32_t index, CrystalFragment* pFrag);

#ifdef USE_POOL
    Pool<CrystalFragment>* m_pPool;
#else
    std::list<CrystalFragment*> m_CrystalFragments; // �j�Ѓ��X�g
#endif
};