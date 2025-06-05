#pragma once
#include <unordered_map>
#include <string>
#include "src/common/component/Component.h"
#include "src/common/component/model_frame_trail_renderer/MODEL_FRAME_TRAIL_RENDERER_DESC.h"

class FrameTrail;

/// <summary>
/// �w��̃��f���̎w��̃t���[���Ƀg���C����`�悷��R���|�[�l���g
/// �����w��A���ꂼ��̏o�����ݒ肪�\�B
/// </summary>
class ModelFrameTrailRenderer : public Component
{
public:
    ModelFrameTrailRenderer() = default;
    ~ModelFrameTrailRenderer() override;

    void Finalize(int hModel, const std::vector<MODEL_FRAME_TRAIL_RENDERER_DESC>& descs, int hImage = -1);

    void Update() override;
    void Draw() override;

private:
    std::unordered_map<std::string, FrameTrail*>    m_Trails; // �t���[�����ƃg���C���̃}�b�v
    int                                             m_hImage{};
    int                                             m_hModel{}; // ���f���n���h��
};