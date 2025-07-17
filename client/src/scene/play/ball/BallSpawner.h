#pragma once
#include "src/scene/play/spawner/SpawnerDescs.h"
#include "src/util/object3D/Object3D.h"
#include "src/util/math/mathIncluder.h"

class BallManager;
class Ball;

class BallSpawner : public Object3D
{
public:
    BallSpawner();
    ~BallSpawner();

    void Init(const BALL_SPAWNER_DESC& desc, const std::string& id = "NULL");
    void Start() override;
    void Update() override;
    void Draw() override;
    void OnSpawn(const Vector3& pos);

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
private:
    int                 m_GenerateBallIndex;
    Ball*               m_pGeneratedBall;
    BallManager*        m_pBallManager;
    BALL_SPAWNER_DESC   m_Desc;             // スポナーの設定
    float               m_SpawnIntervalSec; // 生成までの時間を計測する用タイマー
    bool                m_IsActive;         // Spawnerが有効かどうか
    std::string         m_ID;
};

BallSpawner* AddBallSpawner(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc, const std::string& id = "NULL");
//void BallSpawnerPlaceByJson(const std::string& filepath, const std::string& filekey = "");