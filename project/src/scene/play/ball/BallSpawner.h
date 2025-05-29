#pragma once
#include "src/scene/play/spawner/ISpawner.h"

class BallManager;
class Ball;

class BallSpawner : public ISpawner
{
public:
    BallSpawner();
    ~BallSpawner();

    void Start() override;
    void Update() override;
    void OnSpawn(const Vector3& pos) override;

private:
    int             m_GenerateBallIndex;
    Ball*           m_pGeneratedBall;
    BallManager*    m_pBallManager;
};

void BallSpawnerPlaceByJson(const std::string& filepath, const std::string& filekey = "");