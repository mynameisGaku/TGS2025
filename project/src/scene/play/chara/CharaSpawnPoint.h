#pragma once
#include "src/util/object3D/Object3D.h"

class CharaSpawnPoint : public Object3D
{
public:
    CharaSpawnPoint();
    ~CharaSpawnPoint();
    void Update() override;
    void Draw() override;
private:
    
};