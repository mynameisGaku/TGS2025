#pragma once
#include <string>
#include "src/util/math/Vector3.h"
typedef struct
{
    Vector3     posRandomRange; // 位置のランダム範囲（トレイルの位置にランダムなオフセットを加えるための範囲）
    std::string frameName;      // モデルのフレーム名
    std::string trailName;      // トレイルの名前
    int         interval;       // フレーム間隔（何フレームごとに描画するか
    int         subdivisions;   // 補間分割数（大きいほど滑らか）
    float       thick;          // トレイルの太さ
    float       lifeTime;       // トレイルの寿命
    float       appearRate;     // トレイルが出現する確率（0.0f～1.0f）
}MODEL_FRAME_TRAIL_RENDERER_DESC;