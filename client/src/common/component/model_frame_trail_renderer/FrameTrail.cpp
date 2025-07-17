#include "src/common/component/model_frame_trail_renderer/FrameTrail.h"
#include "src/util/math/Random.h"

void FrameTrail::Init(int hModel, const MODEL_FRAME_TRAIL_RENDERER_DESC& desc, int hImage)
{
    m_hImage = hImage;
    m_hModel = hModel;
    m_FrameName = desc.frameName;
    m_Trail.Init(m_hImage > 0 ? m_hImage : DX_NONE_GRAPH, desc.lifeTime, desc.thick);
    m_Trail.SetInterval(desc.interval);
    m_Trail.SetSubdivisions(desc.subdivisions);
    m_Interval = desc.interval;
    m_Subdivisions = desc.subdivisions;
    m_Thick = desc.thick;
    m_LifeTime = desc.lifeTime;
    m_AppearRate = desc.appearRate;
    m_PosRandomRange = desc.posRandomRange;

    m_UpdateAppearIntervalSec = 0.5f;
    m_UpdateAppearTimer = m_UpdateAppearIntervalSec;
    m_IsActive = false;
}

void FrameTrail::Update()
{
    m_PrevPos = m_CurrPos; // 前回の位置を更新
    m_CurrPos = MV1GetFramePosition(m_hModel, MV1SearchFrame(m_hModel, m_FrameName.c_str()));

    if (m_UpdateAppearTimer <= 0.0f)
    {
        if (GameTime::FrameCount() % m_Interval == 0 && GetRand(100) < m_AppearRate * 100)
        {
            m_IsActive = true; // トレイルを追加する
        }
        else
        {
            m_IsActive = false; // トレイルを追加しない
        }
        m_UpdateAppearTimer = m_UpdateAppearIntervalSec;
    }
    else
    {
        m_UpdateAppearTimer -= GameTime::DeltaTime();
    }

    // 前回の位置と現在の位置が同じ場合はトレイルを追加しない
    if (VSize(VSub(m_CurrPos, m_PrevPos)) < m_Thick * 0.5f)
    {
        m_IsActive = false;
    }

    if (m_CurrPos.GetLength() <= 0.0f)
    {
        m_IsActive = false;
    }

    // トレイルを追加する
    Vector3 randomRange = Vector3(
        Random.GetFloatRange(m_PosRandomRange.x, -m_PosRandomRange.x),
        Random.GetFloatRange(m_PosRandomRange.y, -m_PosRandomRange.y),
        Random.GetFloatRange(m_PosRandomRange.z, -m_PosRandomRange.z)
    );
    m_Trail.Add(m_CurrPos + randomRange, m_IsActive);
    m_Trail.Update();
}

void FrameTrail::Draw()
{
    if (m_Trail.IsActive())
    {
        m_Trail.Draw();
    }
}
