#include "src/scene/play/spawner/ISpawner.h"
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"
#include "src/util/math/Random.h"

ISpawner::ISpawner()
    : m_Desc({})
    , m_SpawnIntervalSec(0.0f)
    , m_IsActive(false)
{
}

ISpawner::~ISpawner()
{
}

void ISpawner::Init(const SPAWNER_DESC& desc)
{
    m_Desc = desc;
}

void ISpawner::Start()
{
    m_SpawnIntervalSec = m_Desc.INTERVAL_SEC + Random.GetFloatRange(-m_Desc.INTERVAL_SEC_RANDOM_RANGE, m_Desc.INTERVAL_SEC_RANDOM_RANGE);
}

void ISpawner::Update()
{
    if (not m_IsActive)
        return;

    // インターバルを進める
    IntervalProcess();

    // 生成処理
    if (GenerateProcess()) return;
}

bool ISpawner::GenerateProcess()
{
    if (m_SpawnIntervalSec > 0.0f)
        return false;

    if (m_Desc.SPAWN_CONDITION != nullptr)
    {
        // 生成条件がある場合、条件を満たしているか確認
        if (not m_Desc.SPAWN_CONDITION())
        {
            return false; // 条件を満たしていないので生成しない
        }
    }

    int spawnAmount = 0;

    // 一度に生成する数をランダムに決定
    if (m_Desc.SPAWN_AMOUNT_ONCE_MIN == 1 &&
        m_Desc.SPAWN_AMOUNT_ONCE_MAX == 1)
    {
        spawnAmount = 1;
    }
    else
    {
        spawnAmount = Random.GetIntRange(m_Desc.SPAWN_AMOUNT_ONCE_MIN, m_Desc.SPAWN_AMOUNT_ONCE_MAX + m_Desc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE);
    }

    // 沸かす
    Spawn(spawnAmount);

    // 生成処理終了
    m_SpawnIntervalSec = m_Desc.INTERVAL_SEC + Random.GetFloatRange(-m_Desc.INTERVAL_SEC_RANDOM_RANGE, m_Desc.INTERVAL_SEC_RANDOM_RANGE);

    return true;
}

void ISpawner::Spawn(int spawnAmount)
{
    for (int i = 0; i < spawnAmount; ++i)
    {
        Vector3 random = Vector3
        (
            Random.GetFloatRange(-m_Desc.SPAWN_RANGE.x, m_Desc.SPAWN_RANGE.x),
            Random.GetFloatRange(-m_Desc.SPAWN_RANGE.y, m_Desc.SPAWN_RANGE.y),
            Random.GetFloatRange(-m_Desc.SPAWN_RANGE.z, m_Desc.SPAWN_RANGE.z)
        );

        Vector3 spawnPos = m_Desc.SPAWNER_POSITION + random;

        OnSpawn(spawnPos);
    }
}

void ISpawner::Draw()
{
#if TRUE
    DrawSphere3D(m_Desc.SPAWNER_POSITION, 50.0f, 4, 0xffffff, 0x000000, false);
#endif
}

void ISpawner::IntervalProcess()
{
    m_SpawnIntervalSec -= GTime.deltaTime;
}