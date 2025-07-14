#pragma once
#include "src/util/object3D/Object3D.h"

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

    // スポナー有効
    void Activate() {m_IsActive = true;}
    // スポナー無効
    void Deactivate() { m_IsActive = false; }

private:
    CrystalFragmentManager* m_pCrystalM;        // クリスタルマネージャー
    float                   m_SpawnInterval;    // 生成までのインターバル
    float                   m_SpawnAmount;      // 一度で生成する数
    float                   m_SpawnTimer;       // 生成までの時間を計測する用タイマー
    bool                    m_IsActive;         // スポナーが有効かどうか
};