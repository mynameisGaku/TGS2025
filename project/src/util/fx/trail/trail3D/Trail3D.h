#pragma once
#include <deque>
#include "src/util/math/Vector3.h"
#include "src/util/easing/EasingUtils.h"
#include <src/util/easing/easing.h>

class Trail3D 
{
public:
    void Init(int texHandle, float lifetime, float width);
    void Add(const Vector3& pos, bool isActive = true);
    void Update();
    void Draw();
    void Deactive();
    bool IsActive();
    void SetInterval(int frame) { m_UpdateInterval = frame; }
    void SetSubdivisions(int subdivisions) { m_Subdivisions = subdivisions; }
private:
    struct TrailPoint 
    {
        Vector3 position;
        float timeAlive;
        bool isActive;
    };

    void DrawTrailSegment(const VECTOR& pos1, const VECTOR& pos2, float t1, float t2, const VECTOR& sideAxis, float alpha1, float alpha2);

    std::deque<TrailPoint> m_Points;
    int m_TextureHandle = -1;
    float m_MaxLifeTime = 1.0f;
    float m_TrailWidth = 1.0f;

    int m_UpdateInterval = 1; // 更新間隔（フレーム単位）
    int m_Subdivisions = 16; // 補間分割数（大きいほど滑らか）
};