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
    Vector3     m_PrevPos = Vector3::Zero; // 前回の位置
    Vector3     m_CurrPos = Vector3::Zero; // 現在の位置
    std::string m_FrameName = ""; // モデルのフレーム名
    Trail3D     m_Trail; // トレイルのインスタンス
    int         m_hImage = -1; // トレイルの画像ハンドル
    int         m_hModel = -1; // モデルハンドル
    int         m_Interval = 1; // フレーム間隔
    int         m_Subdivisions = 16; // 補間分割数
    float       m_Thick = 1.0f; // トレイルの太さ
    float       m_LifeTime = 1.0f; // トレイルの寿命
    float       m_AppearRate = 1.0f; // 出現率
    float       m_UpdateAppearIntervalSec = 1.0f; // 何秒ごとに出現率を計算するか
    float       m_UpdateAppearTimer = 0.0f; // 出現率を更新するタイマー
    bool        m_IsActive = true; // トレイルがアクティブかどうか
};