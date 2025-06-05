#pragma once
#include "src/util/fx/trail/trail3D/Trail3D.h"
#include "src/common/component/model_frame_trail_renderer/MODEL_FRAME_TRAIL_RENDERER_DESC.h"
#include "src/common/component/model_frame_trail_renderer/ModelFrameTrailRenderer.h"

#include <string>
#include "src/util/math/Vector3.h"

class FrameTrail
{
public:
    FrameTrail() = default;
    ~FrameTrail() = default;
    void Init(int hModel, const MODEL_FRAME_TRAIL_RENDERER_DESC& desc, int hImage);
    void Update();
    void Draw();

private:
    Vector3     m_PrevPos = Vector3::Zero; // �O��̈ʒu
    Vector3     m_CurrPos = Vector3::Zero; // ���݂̈ʒu
    std::string m_FrameName = ""; // ���f���̃t���[����
    Trail3D     m_Trail; // �g���C���̃C���X�^���X
    int         m_hImage = -1; // �g���C���̉摜�n���h��
    int         m_hModel = -1; // ���f���n���h��
    int         m_Interval = 1; // �t���[���Ԋu
    int         m_Subdivisions = 16; // ��ԕ�����
    float       m_Thick = 1.0f; // �g���C���̑���
    float       m_LifeTime = 1.0f; // �g���C���̎���
    float       m_AppearRate = 1.0f; // �o����
    float       m_UpdateAppearIntervalSec = 1.0f; // ���b���Ƃɏo�������v�Z���邩
    float       m_UpdateAppearTimer = 0.0f; // �o�������X�V����^�C�}�[
    bool        m_IsActive = true; // �g���C�����A�N�e�B�u���ǂ���
};