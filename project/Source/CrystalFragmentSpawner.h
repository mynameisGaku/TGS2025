#pragma once
#include "object3D.h"

class CrystalFragmentManager;

class CrystalFragmentSpawner : public Object3D
{
public:
    CrystalFragmentSpawner();
    CrystalFragmentSpawner(const Vector3& pos);
    ~CrystalFragmentSpawner();
    
    void Start() override;
    void Update() override;
    void Draw() override;

    void Activate() {m_IsActive = true;}
    void Deactivate() { m_IsActive = false; }

private:
    CrystalFragmentManager* m_pCrystalM;
    float                   m_SpawnInterval;
    float                   m_SpawnAmount;
    float                   m_SpawnTimer;
    bool                    m_IsActive;
};