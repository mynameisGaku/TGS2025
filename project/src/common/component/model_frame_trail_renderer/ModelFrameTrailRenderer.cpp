#include "src/common/component/model_frame_trail_renderer/ModelFrameTrailRenderer.h"
#include "src/common/component/model_frame_trail_renderer/FrameTrail.h"
#include "src/util/ptr/PtrUtil.h"
#include "src/common/performance_profiler/PerformanceProfilerManagerManager.h"

static int c = 0;

ModelFrameTrailRenderer::~ModelFrameTrailRenderer()
{
    for (auto& trail : *m_Trails)
    {
        PtrUtil::SafeDelete(trail.second); // トレイルのインスタンスを削除
    }
    delete m_Trails;
    m_Trails = nullptr;
    PtrUtil::SafeDelete(m_pUpdateProfiler);
    PtrUtil::SafeDelete(m_pDrawProfiler);
}

void ModelFrameTrailRenderer::Build(int hModel, const std::vector<MODEL_FRAME_TRAIL_RENDERER_DESC>& descs, int hImage)
{
    if (not m_Trails)
    {
        m_Trails = new std::unordered_map<std::string, FrameTrail*>;
    }

    m_hImage = hImage;
    m_hModel = hModel;
    for (const auto& desc : descs)
    {
        FrameTrail* trail = new FrameTrail;
        trail->Init(m_hModel, desc, m_hImage);
        (*m_Trails)[desc.frameName] = trail; // フレーム名をキーにしてトレイルを保存
    }

    std::string tmp = "MFTR : " + std::to_string(c);
    std::string update = "[Update] " + tmp;
    std::string draw = "[Draw] " + tmp;

    m_pUpdateProfiler = new PerformanceProfiler(update);
    m_pUpdateProfiler->Activate();
    m_pDrawProfiler = new PerformanceProfiler(draw);
    m_pDrawProfiler->Activate();
    c++;
}

void ModelFrameTrailRenderer::Update()
{
    m_pUpdateProfiler->BeginProfiling();
    for (auto& trail : *m_Trails)
    {
        trail.second->Update(); // 各トレイルの更新
    }
    m_pUpdateProfiler->EndProfiling();
}

void ModelFrameTrailRenderer::Draw()
{
    m_pDrawProfiler->BeginProfiling();
    for (auto& trail : *m_Trails)
    {
        trail.second->Draw(); // 各トレイルの描画
    }
    m_pDrawProfiler->EndProfiling();
}
