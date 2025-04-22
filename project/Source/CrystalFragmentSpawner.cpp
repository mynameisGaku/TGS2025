#include "CrystalFragmentSpawner.h"
#include "CrystalFragmentSpawnerRef.h"
#include "CrystalFragmentManager.h"
#include <Library/time.h>

CrystalFragmentSpawner::CrystalFragmentSpawner() : CrystalFragmentSpawner(V3::ZERO)
{ /*DO NOTHING*/ }

CrystalFragmentSpawner::CrystalFragmentSpawner(const Vector3& pos):
    Object3D(),
    m_pCrystalM(nullptr),
    m_SpawnInterval(0.0f),
    m_SpawnAmount(0.0f),
    m_SpawnTimer(0.0f),
    m_IsActive(false)
{
    // 座標のみ初期化
    transform->position = pos;
}

CrystalFragmentSpawner::~CrystalFragmentSpawner()
{ /*DO NOTHING*/ }

void CrystalFragmentSpawner::Start()
{
    m_pCrystalM = FindGameObject<CrystalFragmentManager>();
    if (m_pCrystalM == nullptr)
    {
        m_pCrystalM = Instantiate<CrystalFragmentManager>();
    }

    CRYSTALFRAGMENTSPAWNER_REF.Load();
    
    m_SpawnInterval = CRYSTALFRAGMENTSPAWNER_REF.SpawnInterval;
    m_SpawnAmount   = CRYSTALFRAGMENTSPAWNER_REF.SpawnAmount;
    m_SpawnTimer    = m_SpawnInterval;
    m_IsActive      = true;
}

void CrystalFragmentSpawner::Update()
{
    if (m_IsActive == false)
        return;
    // スポーン間隔を計測
    m_SpawnTimer -= Time::DeltaTime();
    // スポーン間隔が経過したらスポーン
    if (m_SpawnTimer <= 0.0f)
    {
        for (int i = 0; i < m_SpawnAmount; ++i)
        {
            Vector3 pos = transform->position;
            m_pCrystalM->CreateFragment(pos);
        }
        m_SpawnTimer = m_SpawnInterval;
    }
}

void CrystalFragmentSpawner::Draw()
{
    DrawCapsule3D(transform->position, transform->position + Vector3(0.0f, 5.0f, 0.0f), 32.0f, 32, GetColor(255, 0, 255), 0x00ff00, true);
}
