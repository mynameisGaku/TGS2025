#include "src/common/component/model_frame_trail_renderer/ModelFrameTrailRenderer.h"
#include "src/common/component/model_frame_trail_renderer/FrameTrail.h"
#include "src/util/ptr/PtrUtil.h"

ModelFrameTrailRenderer::~ModelFrameTrailRenderer()
{
    for (auto& trail : m_Trails)
    {
        PtrUtil::SafeDelete(trail.second); // トレイルのインスタンスを削除
    }
    m_Trails.clear(); // マップをクリア
}

void ModelFrameTrailRenderer::Finalize(int hModel, const std::vector<MODEL_FRAME_TRAIL_RENDERER_DESC>& descs, int hImage)
{
    m_hImage = hImage;
    m_hModel = hModel;
    for (const auto& desc : descs)
    {
        FrameTrail* trail = new FrameTrail;
        trail->Init(m_hModel, desc, m_hImage);
        m_Trails[desc.frameName] = trail; // フレーム名をキーにしてトレイルを保存
    }
}

void ModelFrameTrailRenderer::Update()
{
    for (auto& trail : m_Trails)
    {
        trail.second->Update(); // 各トレイルの更新
    }
}

void ModelFrameTrailRenderer::Draw()
{
    for (auto& trail : m_Trails)
    {
        trail.second->Draw(); // 各トレイルの描画
    }
}
