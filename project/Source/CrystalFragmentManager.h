#pragma once
#include "crystalfragment.h"

class CrystalFragmentManager : public GameObject
{
public:
    CrystalFragmentManager();
    ~CrystalFragmentManager();

    void Update() override;

    void CreateFragment(const Vector3& pos);


private:
    std::list<CrystalFragment*> m_CrystalFragments;
};