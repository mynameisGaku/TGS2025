#include <DxLib.h>

#include "src/util/fx/trail/trail3D/Trail3D.h"
#include "src/util/time/GameTime.h"
#include <src/util/math/MathUtil.h>

void Trail3D::Init(int texHandle, float lifetime, float width)
{
    textureHandle = texHandle;
    maxLifeTime = lifetime;
    trailWidth = width;
    points.clear();
}


void Trail3D::Add(const Vector3& pos)
{
    const float MIN_SEGMENT_LENGTH = trailWidth * 0.5f;
    if (points.empty() || VSize(VSub(pos, points.back().position)) >= MIN_SEGMENT_LENGTH)
    {
        points.push_back({ pos, 0.0f });
    }
}

void Trail3D::Update()
{
    float dt = GameTime::DeltaTime();
    for (auto& pt : points)
    {
        pt.timeAlive += dt;
    }
    while (!points.empty() && points.front().timeAlive > maxLifeTime)
    {
        points.pop_front();
    }
}

void Trail3D::Draw()
{
    if (points.size() < 4 || textureHandle == -1)
    {
        return;
    }

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

    VECTOR camPos = GetCameraPosition();

    auto CalcAlpha = [&](float timeAlive) -> float
        {
            float t = timeAlive / maxLifeTime;
            float eased = 1.0f - t;
            float alpha = eased * eased;
            return alpha * 255.0f;
        };

    const int subdivs = 16; // •âŠÔ•ªŠ„”i‘å‚«‚¢‚Ù‚ÇŠŠ‚ç‚©j

    for (size_t i = 1; i + 2 < points.size(); ++i)
    {
        const TrailPoint& p0 = points[i - 1];
        const TrailPoint& p1 = points[i];
        const TrailPoint& p2 = points[i + 1];
        const TrailPoint& p3 = points[i + 2];

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

void Trail3D::DrawTrailSegment(const VECTOR& pos1, const VECTOR& pos2, float t1, float t2, const VECTOR& sideAxis, float alpha1, float alpha2)
{
    float width1 = trailWidth * (1.0f - t1 / maxLifeTime);
    float width2 = trailWidth * (1.0f - t2 / maxLifeTime);

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

    DrawPolygon3D(verts, 2, textureHandle, TRUE);
}
