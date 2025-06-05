#include "src/common/component/model_frame_trail_renderer/ModelFrameTrailRenderer.h"
#include "src/common/component/model_frame_trail_renderer/FrameTrail.h"
#include "src/util/ptr/PtrUtil.h"

ModelFrameTrailRenderer::~ModelFrameTrailRenderer()
{
    for (auto& trail : m_Trails)
    {
        PtrUtil::SafeDelete(trail.second); // �g���C���̃C���X�^���X���폜
    }
    m_Trails.clear(); // �}�b�v���N���A
}

void ModelFrameTrailRenderer::Finalize(int hModel, const std::vector<MODEL_FRAME_TRAIL_RENDERER_DESC>& descs, int hImage)
{
    m_hImage = hImage;
    m_hModel = hModel;
    for (const auto& desc : descs)
    {
        FrameTrail* trail = new FrameTrail;
        trail->Init(m_hModel, desc, m_hImage);
        m_Trails[desc.frameName] = trail; // �t���[�������L�[�ɂ��ăg���C����ۑ�
    }
}

void ModelFrameTrailRenderer::Update()
{
    for (auto& trail : m_Trails)
    {
        trail.second->Update(); // �e�g���C���̍X�V
    }
}

void ModelFrameTrailRenderer::Draw()
{
    for (auto& trail : m_Trails)
    {
        trail.second->Draw(); // �e�g���C���̕`��
    }
}
