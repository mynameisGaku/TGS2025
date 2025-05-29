#pragma once
#include <deque>
#include "src/util/fx/trail/trail3D/TrailPoint3D.h"
#include "src/util/math/Vector3.h"
#include "src/util/easing/EasingUtils.h"
#include <src/util/easing/easing.h>

class Trail3D 
{
public:
    void Init(int texHandle, float lifetime, float width);
    void Add(const Vector3& pos);
    void Update();
    void Draw();
    void Deactive();
    bool IsActive();
private:
    struct TrailPoint 
    {
        Vector3 position;
        float timeAlive;
    };

    void DrawTrailSegment(const VECTOR& pos1, const VECTOR& pos2, float t1, float t2, const VECTOR& sideAxis, float alpha1, float alpha2);

    std::deque<TrailPoint> m_Points;
    int m_TextureHandle = -1;
    float m_MaxLifeTime = 1.0f;
    float m_TrailWidth = 1.0f;
};