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

    // Spawner�̗L����
    void Activate() { m_IsActive = true; }
    // Spawner�̖�����
    void Deactivate() { m_IsActive = false; }

    // �����Ԋu��i�߂�
    void IntervalProcess();

    // �����������s��
    bool GenerateProcess();

    // �w�����������
    void Spawn(int spawnAmount);
private:
    int                 m_GenerateBallIndex;
    Ball*               m_pGeneratedBall;
    BallManager*        m_pBallManager;
    BALL_SPAWNER_DESC   m_Desc;             // �X�|�i�[�̐ݒ�
    float               m_SpawnIntervalSec; // �����܂ł̎��Ԃ��v������p�^�C�}�[
    bool                m_IsActive;         // Spawner���L�����ǂ���
    std::string         m_ID;
};

BallSpawner* AddBallSpawner(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc, const std::string& id = "NULL");
//void BallSpawnerPlaceByJson(const std::string& filepath, const std::string& filekey = "");