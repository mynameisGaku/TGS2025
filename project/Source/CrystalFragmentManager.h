#pragma once
#include "crystalfragment.h"

/// <summary>
/// �N���X�^���̔j�Ђ��Ǘ�����N���X
/// </summary>
class CrystalFragmentManager : public GameObject
{
public:
    CrystalFragmentManager();
    ~CrystalFragmentManager();

    void Update() override;

    /// <summary>
    /// �j�Ђ𐶐�����B
    /// �������ꂽ�j�Ђ́A�����_���ȕ����ɔ��ł����܂��B
    /// </summary>
    /// <param name="pos">�����ꏊ</param>
    void CreateFragment(const Vector3& pos);

private:
    std::list<CrystalFragment*> m_CrystalFragments; // �j�Ѓ��X�g
};