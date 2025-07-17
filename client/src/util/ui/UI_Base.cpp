//#include "ui_Base.h"
//#include "framework/SceneManager.h"
//
//void UI_Base::Initialize()
//{
//    graph.handle = -1; // 画像ハンドル
//    graph.size_x = 0;  // 画像の横サイズ
//    graph.size_y = 0;  // 画像の縦サイズ
//    graph.cell_x = 1;  // 画像のセル数（横）
//    graph.cell_y = 1;  // 画像のセル数（縦）
//    graph.center_x = 0.0f;
//    graph.center_y = 0.0f;
//    graph.alpha = 255.0f;  // 画像のアルファ値
//    graph.ext = 1.0f;    // 拡大率
//    graph.angle = 0.0f;  // 回転角度
//    graph.bright.red = 255.0f;    // 輝度（赤）
//    graph.bright.green = 255.0f;  // 輝度（緑）
//    graph.bright.blue = 255.0f;   // 輝度（青）
//    graph.pos = Vector2(0.0f, 0.0f);  // 画像の位置
//
//    //ビルボード初期化
//    billgauge.m_polygonNum = 2;
//
//    //GetLightDirection();  //を元に頂点の法線を回転させる
//    billgauge.m_pRawVertexNorm = Vector3(0.0f, 0.0f, 1.0f);
//
//    //揺れの予約フラグをfalseで初期化
//    isShake = false;
//    //拡縮の予約フラグをfalseで初期化
//    isBound = false;
//    //デバッグ変数初期化
//    isShow = true;
//}
//
//void UI_Base::DrawHorizontalGauge(Vector2 Pos, float cx, float cy, float Size, float MaxValue, float CurrentValue, float Angle, int GrHandle, int TransFlag, bool ReverseFlagX, bool ReverseFlagY)
//{
//    // ゲージの全体のサイズ
//    float gaugeSize = Size;
//
//    // CurrentValue に応じて描画するゲージの長さを計算
//    float gaugeLength = gaugeSize * (CurrentValue / MaxValue);
//
//    // 横棒ゲージの左上の座標を計算
//    float gaugeLeft = Pos.x - (gaugeSize * cx);
//    float gaugeTop = Pos.y - (gaugeSize * cy);
//
//    // 横棒ゲージの右下の座標を計算
//    float gaugeRight = gaugeLeft + gaugeLength;
//    float gaugeBottom = gaugeTop + gaugeSize;
//
//    // ゲージの描画
//    DrawRectRotaGraph3F(gaugeLeft, gaugeTop, 0, 0, (int)gaugeLength, (int)gaugeSize, 0.0f, 0.0f, 1.0, 1.0, Angle, GrHandle, TransFlag, ReverseFlagX, ReverseFlagY);
//}
//
//void UI_Base::DrawParallelGauge(Vector2 Pos, float cx, float cy, float Width, float Height, float MaxValue, float CurrentValue, float Angle, int Color, int FillFlag, bool ReverseFlagX, bool ReverseFlagY)
//{
//    // ゲージの幅と高さ
//    float gaugeWidth = Width;
//    float gaugeHeight = Height;
//
//    // CurrentValue に応じて描画するゲージの横の長さを計算
//    float gaugeLength = gaugeWidth * (CurrentValue / MaxValue);
//
//    // ゲージの左上、右上、右下、左下の頂点の座標を計算
//    float gaugeTopLeftX = Pos.x - (gaugeWidth * cx);
//    float gaugeTopLeftY = Pos.y - (gaugeHeight * cy);
//    float gaugeTopRightX = gaugeTopLeftX + gaugeLength * cosf(Angle);
//    float gaugeTopRightY = gaugeTopLeftY + gaugeLength * sinf(Angle);
//    float gaugeBottomLeftX = Pos.x - (gaugeWidth * (1 - cx));
//    float gaugeBottomLeftY = Pos.y + (gaugeHeight * (1 - cy));
//    float gaugeBottomRightX = gaugeBottomLeftX + gaugeLength * cosf(Angle);
//    float gaugeBottomRightY = gaugeBottomLeftY + gaugeLength * sinf(Angle);
//
//    // 反転フラグに応じて座標を調整
//    if (ReverseFlagX) {
//        std::swap(gaugeTopLeftX, gaugeTopRightX);
//        std::swap(gaugeBottomLeftX, gaugeBottomRightX);
//    }
//
//    if (ReverseFlagY) {
//        std::swap(gaugeTopLeftY, gaugeBottomLeftY);
//        std::swap(gaugeTopRightY, gaugeBottomRightY);
//    }
//
//    // ゲージの描画
//    DrawQuadrangle((int)gaugeTopLeftX, (int)gaugeTopLeftY, (int)gaugeTopRightX, (int)gaugeTopRightY, (int)gaugeBottomRightX, (int)gaugeBottomRightY, (int)gaugeBottomLeftX, (int)gaugeBottomLeftY, (unsigned int)Color, FillFlag);
//}
//
////ビルボードで必要なカメラのヘッダー
////#include "src/common/camera/Camera.h"
//
//void UI_Base::DrawBillGauge3D(Vector3 _pos, BillGaugeParam param, float current, float currentMax, int graph, bool transflag)
//{
//    MATERIALPARAM Material;
//    Material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
//    Material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
//    Material.Ambient = GetColorF(1.0f, 1.0f, 1.0f, 0.0f);
//    Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
//    Material.Power = 100.0f;
//    SetMaterialParam(Material);
//
//    //画像取得
//    billgauge.m_graphHandle = graph;
//    //描画位置
//    billgauge.m_pos = _pos;
//    //カメラの方向から回転行列を作成
//    MATRIX vert_LookMat = MGetIdent();/*SceneManager::CommonScene()->FindGameObject<Camera>()->transform->GetRotationMatrix();*/
//
//    //構造体の各変数をわかりやすく
//    Vector2 left_down = Vector2(param.x1, param.y1);
//    Vector2 left_up = Vector2(param.x2, param.y2);
//    Vector2 right_down = Vector2(param.x3, param.y3);
//    Vector2 right_up = Vector2(param.x4, param.y4);
//
//    //指定の描画位置を原点に各頂点座標を計算
//    Vector3 vert_pos1 = VAdd(VTransform(left_down, vert_LookMat), billgauge.m_pos);
//    Vector3 vert_pos2 = VAdd(VTransform(left_up, vert_LookMat), billgauge.m_pos);
//    Vector3 vert_pos3 = VAdd(VTransform(right_down, vert_LookMat), billgauge.m_pos);
//    Vector3 vert_pos4 = VAdd(VTransform(right_up, vert_LookMat), billgauge.m_pos);
//
//    //座標ベクトルを回転行列で回す
//    vert_pos1 = VTransform(vert_pos1, vert_LookMat);
//    vert_pos2 = VTransform(vert_pos2, vert_LookMat);
//    vert_pos3 = VTransform(vert_pos3, vert_LookMat);
//    vert_pos4 = VTransform(vert_pos4, vert_LookMat);
//
//    //頂点設定
//    //左下
//    billgauge.m_vert[0].pos = VGet(vert_pos1.x, vert_pos1.y, vert_pos1.z);				//左下 0, 1
//    billgauge.m_vert[0].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[0].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[0].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[0].su = 0.0f;
//    billgauge.m_vert[0].sv = 0.0f;
//    //左上
//    billgauge.m_vert[1].pos = VGet(vert_pos2.x, vert_pos2.y, vert_pos2.z);				//左上 0, 0
//    billgauge.m_vert[1].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[1].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[1].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[1].su = 0.0f;
//    billgauge.m_vert[1].sv = 0.0f;
//    //右下
//    billgauge.m_vert[2].pos = VGet(vert_pos3.x, vert_pos3.y, vert_pos3.z);				//右下 1, 1
//    billgauge.m_vert[2].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[2].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[2].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[2].su = 0.0f;
//    billgauge.m_vert[2].sv = 0.0f;
//    //右上
//    billgauge.m_vert[3].pos = VGet(vert_pos4.x, vert_pos4.y, vert_pos4.z);		    	//右上 1, 0
//    billgauge.m_vert[3].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[3].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[3].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[3].su = 0.0f;
//    billgauge.m_vert[3].sv = 0.0f;
//
//    //ゲージの枠
//    int frame_1_size = 2;
//    int frame_2_size = 3;
//    float frame_1_z = 0.1f;
//    float frame_2_z = 0.2f;
//    Vector3 frame_1_left_down = Vector3(param.x1 - frame_1_size, param.y1 - frame_1_size, frame_1_z);
//    Vector3 frame_1_left_up = Vector3(param.x2 - frame_1_size, param.y2 + frame_1_size, frame_1_z);
//    Vector3 frame_1_right_down = Vector3(param.x3 + frame_1_size, param.y3 - frame_1_size, frame_1_z);
//    Vector3 frame_1_right_up = Vector3(param.x4 + frame_1_size, param.y4 + frame_1_size, frame_1_z);
//    Vector3 frame_2_left_down = Vector3(param.x1 - frame_2_size, param.y1 - frame_2_size, frame_2_z);
//    Vector3 frame_2_left_up = Vector3(param.x2 - frame_2_size, param.y2 + frame_2_size, frame_2_z);
//    Vector3 frame_2_right_down = Vector3(param.x3 + frame_2_size, param.y3 - frame_2_size, frame_2_z);
//    Vector3 frame_2_right_up = Vector3(param.x4 + frame_2_size, param.y4 + frame_2_size, frame_2_z);
//    Vector3 frame_1_vert_pos1 = VAdd(VTransform(frame_1_left_down, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_1_vert_pos2 = VAdd(VTransform(frame_1_left_up, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_1_vert_pos3 = VAdd(VTransform(frame_1_right_down, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_1_vert_pos4 = VAdd(VTransform(frame_1_right_up, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_2_vert_pos1 = VAdd(VTransform(frame_2_left_down, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_2_vert_pos2 = VAdd(VTransform(frame_2_left_up, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_2_vert_pos3 = VAdd(VTransform(frame_2_right_down, vert_LookMat), billgauge.m_pos);
//    Vector3 frame_2_vert_pos4 = VAdd(VTransform(frame_2_right_up, vert_LookMat), billgauge.m_pos);
//
//    //枠用の座標ベクトルを回転行列で回す
//    frame_1_vert_pos1 = VTransform(frame_1_vert_pos1, vert_LookMat);
//    frame_1_vert_pos2 = VTransform(frame_1_vert_pos2, vert_LookMat);
//    frame_1_vert_pos3 = VTransform(frame_1_vert_pos3, vert_LookMat);
//    frame_1_vert_pos4 = VTransform(frame_1_vert_pos4, vert_LookMat);
//    frame_2_vert_pos1 = VTransform(frame_2_vert_pos1, vert_LookMat);
//    frame_2_vert_pos2 = VTransform(frame_2_vert_pos2, vert_LookMat);
//    frame_2_vert_pos3 = VTransform(frame_2_vert_pos3, vert_LookMat);
//    frame_2_vert_pos4 = VTransform(frame_2_vert_pos4, vert_LookMat);
//    //枠用の頂点
//    VERTEX3D frame_1_vert[VERT_INDEX]{ 0 };
//    VERTEX3D frame_2_vert[VERT_INDEX]{ 0 };
//    for (int i = 0; i < VERT_INDEX; i++)
//    {
//        //初期化
//        frame_1_vert[i] = billgauge.m_vert[i];
//        frame_2_vert[i] = billgauge.m_vert[i];
//    }
//    //それぞれの頂点の座標を設定
//    frame_1_vert[0].pos = frame_1_vert_pos1;
//    frame_1_vert[1].pos = frame_1_vert_pos2;
//    frame_1_vert[2].pos = frame_1_vert_pos3;
//    frame_1_vert[3].pos = frame_1_vert_pos4;
//    frame_2_vert[0].pos = frame_2_vert_pos1;
//    frame_2_vert[1].pos = frame_2_vert_pos2;
//    frame_2_vert[2].pos = frame_2_vert_pos3;
//    frame_2_vert[3].pos = frame_2_vert_pos4;
//
//    //UV座標指定
//    Vector2 uv_pos_leftdown = Vector2(param.u1, param.v1);
//    Vector2 uv_pos_leftup = Vector2(param.u2, param.v2);
//    Vector2 uv_pos_rightdown = Vector2(param.u3, param.v3);
//    Vector2 uv_pos_rightup = Vector2(param.u4, param.v4);
//
//    //左下
//    frame_1_vert[0].u = (float)0.0f + 0.5f;
//    frame_1_vert[0].v = (float)1.0f;
//    //左上
//    frame_1_vert[1].u = (float)0.0f + 0.5f;
//    frame_1_vert[1].v = (float)0.0f;
//    //右下
//    frame_1_vert[2].u = (float)1.0f;
//    frame_1_vert[2].v = (float)1.0f;
//    //右上
//    frame_1_vert[3].u = (float)1.0f;
//    frame_1_vert[3].v = (float)0.0f;
//
//    //現在値を正規化する
//    float normalizedCurrent = (float)(current / currentMax) * (float)(this->graph.center_x);
//
//    //左下
//    billgauge.m_vert[0].u = (float)(uv_pos_leftdown.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[0].v = (float)uv_pos_leftdown.y / this->graph.size_y;
//    //左上
//    billgauge.m_vert[1].u = (float)(uv_pos_leftup.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[1].v = (float)uv_pos_leftup.y / this->graph.size_y;
//    //右下
//    billgauge.m_vert[2].u = (float)(uv_pos_rightdown.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[2].v = (float)uv_pos_rightdown.y / this->graph.size_y;
//    //右上
//    billgauge.m_vert[3].u = (float)(uv_pos_rightup.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[3].v = (float)uv_pos_rightup.y / this->graph.size_y;
//
//    //計算後の頂点データを合体して２枚分のポリゴンにする
//    //ゲージ本体
//    VERTEX3D gauge_verts[3 + 3] =
//    {
//        //一枚目
//        billgauge.m_vert[0],billgauge.m_vert[1],billgauge.m_vert[2],
//        //二枚目
//        billgauge.m_vert[2],billgauge.m_vert[1],billgauge.m_vert[3]
//    };
//    //枠
//    VERTEX3D frame_1_verts[3 + 3] =
//    {
//        //一枚目
//        frame_1_vert[0],frame_1_vert[1],frame_1_vert[2],
//        //二枚目
//        frame_1_vert[2],frame_1_vert[1],frame_1_vert[3]
//    };
//    VERTEX3D frame_2_verts[3 + 3] =
//    {
//        //一枚目
//        frame_2_vert[0],frame_2_vert[1],frame_2_vert[2],
//        //二枚目
//        frame_2_vert[2],frame_2_vert[1],frame_2_vert[3]
//    };
//
//    //マッピングモードをラップに設定
//    SetTextureAddressMode(DX_TEXADDRESS_WRAP);
//
//    //描画    画像が-1なら画像を適用しない
//    //枠２(白)
//    DrawPolygon3D(frame_2_verts, billgauge.m_polygonNum, DX_NONE_GRAPH, transflag);
//    SetDrawBright(0, 0, 0);
//    //枠１
//    DrawPolygon3D(frame_1_verts, billgauge.m_polygonNum, billgauge.m_graphHandle > 0 ? billgauge.m_graphHandle : DX_NONE_GRAPH, transflag);
//    SetDrawBright(255, 255, 255);
//
//    //ゲージ本体
//    DrawPolygon3D(gauge_verts, billgauge.m_polygonNum, billgauge.m_graphHandle > 0 ? billgauge.m_graphHandle : DX_NONE_GRAPH, transflag);
//
//    //マテリアルの色を戻す
//    Material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
//    Material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
//    Material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
//    Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
//    Material.Power = 100.0f;
//    SetMaterialParam(Material);
//}
//
