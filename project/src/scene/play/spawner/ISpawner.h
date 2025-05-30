#pragma once
#include "framework/gameObject.h"
#include "src/util/math/Vector3.h"
#include "src/scene/play/spawner/SpawnerDescs.h"

/// <summary>
/// スポナーのインターフェース
/// </summary>
/// <author>藤本樂</author>
class ISpawner : public GameObject
{
public:
    ISpawner();
    ~ISpawner();

    void Init(const SPAWNER_DESC& desc);
    virtual void Start() override;
    virtual void Update() override;
    virtual void Draw() override;
    // 生成処理を実行。実装は派生クラスで行う。
    virtual void OnSpawn(const Vector3& pos) {}

    // Spawnerの有効化
    void Activate() { m_IsActive = true; }
    // Spawnerの無効化
    void Deactivate() { m_IsActive = false; }

    // 生成間隔を進める
    void IntervalProcess();

    // 生成処理を行う
    bool GenerateProcess();
    
    // 指定個数生成する
    void Spawn(int spawnAmount);
    
protected:
    SPAWNER_DESC    m_Desc;             // スポナーの設定
    float           m_SpawnIntervalSec; // 生成までの時間を計測する用タイマー
    bool            m_IsActive;         // Spawnerが有効かどうか
};