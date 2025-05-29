#include <DxLib.h>

#include "src/util/fx/trail/trail3D/Trail3D.h"
#include "src/util/time/GameTime.h"
#include <src/util/math/MathUtil.h>

void Trail3D::Init(int texHandle, float lifetime, float width)
{
    m_TextureHandle = texHandle;
    m_MaxLifeTime = lifetime;
    m_TrailWidth = width;
    m_Points.clear();
}

void Trail3D::Add(const Vector3& pos)
{
    const float MIN_SEGMENT_LENGTH = m_TrailWidth * 0.5f;
    if (m_Points.empty() || VSize(VSub(pos, m_Points.back().position)) >= MIN_SEGMENT_LENGTH)
    {
        m_Points.push_back({ pos, 0.0f });
    }
}

void Trail3D::Update()
{
    float dt = GameTime::DeltaTime();
    for (auto& pt : m_Points)
    {
        pt.timeAlive += dt;
    }
    while (!m_Points.empty() && m_Points.front().timeAlive > m_MaxLifeTime)
    {
        m_Points.pop_front();
    }
}

void Trail3D::Draw()
{
    if (m_Points.size() < 4 || m_TextureHandle == -1)
    {
        return;
    }

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

    VECTOR camPos = GetCameraPosition();

    auto CalcAlpha = [&](float timeAlive) -> float
        {
            float t = timeAlive / m_MaxLifeTime;
            float eased = 1.0f - t;
            float alpha = eased * eased;
            return alpha * 255.0f;
        };

    const int subdivs = 16; // 補間分割数（大きいほど滑らか）

    for (size_t i = 1; i + 2 < m_Points.size(); ++i)
    {
        const TrailPoint& p0 = m_Points[i - 1];
        const TrailPoint& p1 = m_Points[i];
        const TrailPoint& p2 = m_Points[i + 1];
        const TrailPoint& p3 = m_Points[i + 2];

        for (int j = 0; j < subdivs; ++j)
        {
            float t1 = (float)j / subdivs;
            float t2 = (float)(j + 1) / subdivs;

            VECTOR pos1 = MathUtil::CatmullRomVec3(p0.position, p1.position, p2.position, p3.position, t1);
            VECTOR pos2 = MathUtil::CatmullRomVec3(p0.position, p1.position, p2.position, p3.position, t2);

            float time1 = MathUtil::Lerp(p1.timeAlive, p2.timeAlive, t1);
            float time2 = MathUtil::Lerp(p1.timeAlive, p2.timeAlive, t2);

            float alpha1 = CalcAlpha(time1);
            float alpha2 = CalcAlpha(time2);

            VECTOR dir = VSub(pos2, pos1);
            VECTOR viewVec = VSub(camPos, pos1);
            VECTOR sideVec = VNorm(VCross(dir, viewVec));

            DrawTrailSegment(pos1, pos2, time1, time2, sideVec, alpha1, alpha2);
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    SetWriteZBuffer3D(TRUE);
}

void Trail3D::Deactive()
{
    // すべてのトレイルを削除する
    while(m_Points.size() > 0)
    {
        m_Points.pop_front();
    }
}

bool Trail3D::IsActive()
{
    return m_Points.size() > 0;
}

void Trail3D::DrawTrailSegment(const VECTOR& pos1, const VECTOR& pos2, float t1, float t2, const VECTOR& sideAxis, float alpha1, float alpha2)
{
    float width1 = m_TrailWidth * (1.0f - t1 / m_MaxLifeTime);
    float width2 = m_TrailWidth * (1.0f - t2 / m_MaxLifeTime);

    VECTOR side1 = VScale(sideAxis, width1 * 0.5f);
    VECTOR side2 = VScale(sideAxis, width2 * 0.5f);

    VECTOR a1 = VAdd(pos1, side1);
    VECTOR a2 = VSub(pos1, side1);
    VECTOR b1 = VAdd(pos2, side2);
    VECTOR b2 = VSub(pos2, side2);

    COLOR_U8 col1 = GetColorU8(255, 255, 255, (int)alpha1);
    COLOR_U8 col2 = GetColorU8(255, 255, 255, (int)alpha2);

    VERTEX3D verts[6];
    verts[0] = { a1, VGet(0, 0, 0), col1, col1, 0, 0, 0, 0 };
    verts[1] = { b1, VGet(0, 0, 0), col2, col2, 1, 0, 0, 0 };
    verts[2] = { b2, VGet(0, 0, 0), col2, col2, 1, 1, 0, 0 };
    verts[3] = verts[0];
    verts[4] = verts[2];
    verts[5] = { a2, VGet(0, 0, 0), col1, col1, 0, 1, 0, 0 };

    DrawPolygon3D(verts, 2, m_TextureHandle, TRUE);
}
