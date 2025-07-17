//#include "ui_Base.h"
//#include "framework/SceneManager.h"
//
//void UI_Base::Initialize()
//{
//    graph.handle = -1; // �摜�n���h��
//    graph.size_x = 0;  // �摜�̉��T�C�Y
//    graph.size_y = 0;  // �摜�̏c�T�C�Y
//    graph.cell_x = 1;  // �摜�̃Z�����i���j
//    graph.cell_y = 1;  // �摜�̃Z�����i�c�j
//    graph.center_x = 0.0f;
//    graph.center_y = 0.0f;
//    graph.alpha = 255.0f;  // �摜�̃A���t�@�l
//    graph.ext = 1.0f;    // �g�嗦
//    graph.angle = 0.0f;  // ��]�p�x
//    graph.bright.red = 255.0f;    // �P�x�i�ԁj
//    graph.bright.green = 255.0f;  // �P�x�i�΁j
//    graph.bright.blue = 255.0f;   // �P�x�i�j
//    graph.pos = Vector2(0.0f, 0.0f);  // �摜�̈ʒu
//
//    //�r���{�[�h������
//    billgauge.m_polygonNum = 2;
//
//    //GetLightDirection();  //�����ɒ��_�̖@������]������
//    billgauge.m_pRawVertexNorm = Vector3(0.0f, 0.0f, 1.0f);
//
//    //�h��̗\��t���O��false�ŏ�����
//    isShake = false;
//    //�g�k�̗\��t���O��false�ŏ�����
//    isBound = false;
//    //�f�o�b�O�ϐ�������
//    isShow = true;
//}
//
//void UI_Base::DrawHorizontalGauge(Vector2 Pos, float cx, float cy, float Size, float MaxValue, float CurrentValue, float Angle, int GrHandle, int TransFlag, bool ReverseFlagX, bool ReverseFlagY)
//{
//    // �Q�[�W�̑S�̂̃T�C�Y
//    float gaugeSize = Size;
//
//    // CurrentValue �ɉ����ĕ`�悷��Q�[�W�̒������v�Z
//    float gaugeLength = gaugeSize * (CurrentValue / MaxValue);
//
//    // ���_�Q�[�W�̍���̍��W���v�Z
//    float gaugeLeft = Pos.x - (gaugeSize * cx);
//    float gaugeTop = Pos.y - (gaugeSize * cy);
//
//    // ���_�Q�[�W�̉E���̍��W���v�Z
//    float gaugeRight = gaugeLeft + gaugeLength;
//    float gaugeBottom = gaugeTop + gaugeSize;
//
//    // �Q�[�W�̕`��
//    DrawRectRotaGraph3F(gaugeLeft, gaugeTop, 0, 0, (int)gaugeLength, (int)gaugeSize, 0.0f, 0.0f, 1.0, 1.0, Angle, GrHandle, TransFlag, ReverseFlagX, ReverseFlagY);
//}
//
//void UI_Base::DrawParallelGauge(Vector2 Pos, float cx, float cy, float Width, float Height, float MaxValue, float CurrentValue, float Angle, int Color, int FillFlag, bool ReverseFlagX, bool ReverseFlagY)
//{
//    // �Q�[�W�̕��ƍ���
//    float gaugeWidth = Width;
//    float gaugeHeight = Height;
//
//    // CurrentValue �ɉ����ĕ`�悷��Q�[�W�̉��̒������v�Z
//    float gaugeLength = gaugeWidth * (CurrentValue / MaxValue);
//
//    // �Q�[�W�̍���A�E��A�E���A�����̒��_�̍��W���v�Z
//    float gaugeTopLeftX = Pos.x - (gaugeWidth * cx);
//    float gaugeTopLeftY = Pos.y - (gaugeHeight * cy);
//    float gaugeTopRightX = gaugeTopLeftX + gaugeLength * cosf(Angle);
//    float gaugeTopRightY = gaugeTopLeftY + gaugeLength * sinf(Angle);
//    float gaugeBottomLeftX = Pos.x - (gaugeWidth * (1 - cx));
//    float gaugeBottomLeftY = Pos.y + (gaugeHeight * (1 - cy));
//    float gaugeBottomRightX = gaugeBottomLeftX + gaugeLength * cosf(Angle);
//    float gaugeBottomRightY = gaugeBottomLeftY + gaugeLength * sinf(Angle);
//
//    // ���]�t���O�ɉ����č��W�𒲐�
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
//    // �Q�[�W�̕`��
//    DrawQuadrangle((int)gaugeTopLeftX, (int)gaugeTopLeftY, (int)gaugeTopRightX, (int)gaugeTopRightY, (int)gaugeBottomRightX, (int)gaugeBottomRightY, (int)gaugeBottomLeftX, (int)gaugeBottomLeftY, (unsigned int)Color, FillFlag);
//}
//
////�r���{�[�h�ŕK�v�ȃJ�����̃w�b�_�[
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
//    //�摜�擾
//    billgauge.m_graphHandle = graph;
//    //�`��ʒu
//    billgauge.m_pos = _pos;
//    //�J�����̕��������]�s����쐬
//    MATRIX vert_LookMat = MGetIdent();/*SceneManager::CommonScene()->FindGameObject<Camera>()->transform->GetRotationMatrix();*/
//
//    //�\���̂̊e�ϐ����킩��₷��
//    Vector2 left_down = Vector2(param.x1, param.y1);
//    Vector2 left_up = Vector2(param.x2, param.y2);
//    Vector2 right_down = Vector2(param.x3, param.y3);
//    Vector2 right_up = Vector2(param.x4, param.y4);
//
//    //�w��̕`��ʒu�����_�Ɋe���_���W���v�Z
//    Vector3 vert_pos1 = VAdd(VTransform(left_down, vert_LookMat), billgauge.m_pos);
//    Vector3 vert_pos2 = VAdd(VTransform(left_up, vert_LookMat), billgauge.m_pos);
//    Vector3 vert_pos3 = VAdd(VTransform(right_down, vert_LookMat), billgauge.m_pos);
//    Vector3 vert_pos4 = VAdd(VTransform(right_up, vert_LookMat), billgauge.m_pos);
//
//    //���W�x�N�g������]�s��ŉ�
//    vert_pos1 = VTransform(vert_pos1, vert_LookMat);
//    vert_pos2 = VTransform(vert_pos2, vert_LookMat);
//    vert_pos3 = VTransform(vert_pos3, vert_LookMat);
//    vert_pos4 = VTransform(vert_pos4, vert_LookMat);
//
//    //���_�ݒ�
//    //����
//    billgauge.m_vert[0].pos = VGet(vert_pos1.x, vert_pos1.y, vert_pos1.z);				//���� 0, 1
//    billgauge.m_vert[0].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[0].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[0].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[0].su = 0.0f;
//    billgauge.m_vert[0].sv = 0.0f;
//    //����
//    billgauge.m_vert[1].pos = VGet(vert_pos2.x, vert_pos2.y, vert_pos2.z);				//���� 0, 0
//    billgauge.m_vert[1].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[1].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[1].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[1].su = 0.0f;
//    billgauge.m_vert[1].sv = 0.0f;
//    //�E��
//    billgauge.m_vert[2].pos = VGet(vert_pos3.x, vert_pos3.y, vert_pos3.z);				//�E�� 1, 1
//    billgauge.m_vert[2].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[2].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[2].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[2].su = 0.0f;
//    billgauge.m_vert[2].sv = 0.0f;
//    //�E��
//    billgauge.m_vert[3].pos = VGet(vert_pos4.x, vert_pos4.y, vert_pos4.z);		    	//�E�� 1, 0
//    billgauge.m_vert[3].norm = VGet(billgauge.m_pRawVertexNorm.x, billgauge.m_pRawVertexNorm.y, billgauge.m_pRawVertexNorm.z);
//    billgauge.m_vert[3].dif = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[3].spc = GetColorU8(255, 255, 255, 255);
//    billgauge.m_vert[3].su = 0.0f;
//    billgauge.m_vert[3].sv = 0.0f;
//
//    //�Q�[�W�̘g
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
//    //�g�p�̍��W�x�N�g������]�s��ŉ�
//    frame_1_vert_pos1 = VTransform(frame_1_vert_pos1, vert_LookMat);
//    frame_1_vert_pos2 = VTransform(frame_1_vert_pos2, vert_LookMat);
//    frame_1_vert_pos3 = VTransform(frame_1_vert_pos3, vert_LookMat);
//    frame_1_vert_pos4 = VTransform(frame_1_vert_pos4, vert_LookMat);
//    frame_2_vert_pos1 = VTransform(frame_2_vert_pos1, vert_LookMat);
//    frame_2_vert_pos2 = VTransform(frame_2_vert_pos2, vert_LookMat);
//    frame_2_vert_pos3 = VTransform(frame_2_vert_pos3, vert_LookMat);
//    frame_2_vert_pos4 = VTransform(frame_2_vert_pos4, vert_LookMat);
//    //�g�p�̒��_
//    VERTEX3D frame_1_vert[VERT_INDEX]{ 0 };
//    VERTEX3D frame_2_vert[VERT_INDEX]{ 0 };
//    for (int i = 0; i < VERT_INDEX; i++)
//    {
//        //������
//        frame_1_vert[i] = billgauge.m_vert[i];
//        frame_2_vert[i] = billgauge.m_vert[i];
//    }
//    //���ꂼ��̒��_�̍��W��ݒ�
//    frame_1_vert[0].pos = frame_1_vert_pos1;
//    frame_1_vert[1].pos = frame_1_vert_pos2;
//    frame_1_vert[2].pos = frame_1_vert_pos3;
//    frame_1_vert[3].pos = frame_1_vert_pos4;
//    frame_2_vert[0].pos = frame_2_vert_pos1;
//    frame_2_vert[1].pos = frame_2_vert_pos2;
//    frame_2_vert[2].pos = frame_2_vert_pos3;
//    frame_2_vert[3].pos = frame_2_vert_pos4;
//
//    //UV���W�w��
//    Vector2 uv_pos_leftdown = Vector2(param.u1, param.v1);
//    Vector2 uv_pos_leftup = Vector2(param.u2, param.v2);
//    Vector2 uv_pos_rightdown = Vector2(param.u3, param.v3);
//    Vector2 uv_pos_rightup = Vector2(param.u4, param.v4);
//
//    //����
//    frame_1_vert[0].u = (float)0.0f + 0.5f;
//    frame_1_vert[0].v = (float)1.0f;
//    //����
//    frame_1_vert[1].u = (float)0.0f + 0.5f;
//    frame_1_vert[1].v = (float)0.0f;
//    //�E��
//    frame_1_vert[2].u = (float)1.0f;
//    frame_1_vert[2].v = (float)1.0f;
//    //�E��
//    frame_1_vert[3].u = (float)1.0f;
//    frame_1_vert[3].v = (float)0.0f;
//
//    //���ݒl�𐳋K������
//    float normalizedCurrent = (float)(current / currentMax) * (float)(this->graph.center_x);
//
//    //����
//    billgauge.m_vert[0].u = (float)(uv_pos_leftdown.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[0].v = (float)uv_pos_leftdown.y / this->graph.size_y;
//    //����
//    billgauge.m_vert[1].u = (float)(uv_pos_leftup.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[1].v = (float)uv_pos_leftup.y / this->graph.size_y;
//    //�E��
//    billgauge.m_vert[2].u = (float)(uv_pos_rightdown.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[2].v = (float)uv_pos_rightdown.y / this->graph.size_y;
//    //�E��
//    billgauge.m_vert[3].u = (float)(uv_pos_rightup.x + normalizedCurrent) / this->graph.size_x;
//    billgauge.m_vert[3].v = (float)uv_pos_rightup.y / this->graph.size_y;
//
//    //�v�Z��̒��_�f�[�^�����̂��ĂQ�����̃|���S���ɂ���
//    //�Q�[�W�{��
//    VERTEX3D gauge_verts[3 + 3] =
//    {
//        //�ꖇ��
//        billgauge.m_vert[0],billgauge.m_vert[1],billgauge.m_vert[2],
//        //�񖇖�
//        billgauge.m_vert[2],billgauge.m_vert[1],billgauge.m_vert[3]
//    };
//    //�g
//    VERTEX3D frame_1_verts[3 + 3] =
//    {
//        //�ꖇ��
//        frame_1_vert[0],frame_1_vert[1],frame_1_vert[2],
//        //�񖇖�
//        frame_1_vert[2],frame_1_vert[1],frame_1_vert[3]
//    };
//    VERTEX3D frame_2_verts[3 + 3] =
//    {
//        //�ꖇ��
//        frame_2_vert[0],frame_2_vert[1],frame_2_vert[2],
//        //�񖇖�
//        frame_2_vert[2],frame_2_vert[1],frame_2_vert[3]
//    };
//
//    //�}�b�s���O���[�h�����b�v�ɐݒ�
//    SetTextureAddressMode(DX_TEXADDRESS_WRAP);
//
//    //�`��    �摜��-1�Ȃ�摜��K�p���Ȃ�
//    //�g�Q(��)
//    DrawPolygon3D(frame_2_verts, billgauge.m_polygonNum, DX_NONE_GRAPH, transflag);
//    SetDrawBright(0, 0, 0);
//    //�g�P
//    DrawPolygon3D(frame_1_verts, billgauge.m_polygonNum, billgauge.m_graphHandle > 0 ? billgauge.m_graphHandle : DX_NONE_GRAPH, transflag);
//    SetDrawBright(255, 255, 255);
//
//    //�Q�[�W�{��
//    DrawPolygon3D(gauge_verts, billgauge.m_polygonNum, billgauge.m_graphHandle > 0 ? billgauge.m_graphHandle : DX_NONE_GRAPH, transflag);
//
//    //�}�e���A���̐F��߂�
//    Material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
//    Material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
//    Material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
//    Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
//    Material.Power = 100.0f;
//    SetMaterialParam(Material);
//}
//
