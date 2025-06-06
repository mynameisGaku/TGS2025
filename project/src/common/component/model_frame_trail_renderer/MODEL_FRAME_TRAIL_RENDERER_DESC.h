#pragma once
#include <string>
#include "src/util/math/Vector3.h"
typedef struct
{
    Vector3     posRandomRange; // �ʒu�̃����_���͈́i�g���C���̈ʒu�Ƀ����_���ȃI�t�Z�b�g�������邽�߂͈̔́j
    std::string frameName;      // ���f���̃t���[����
    std::string trailName;      // �g���C���̖��O
    int         interval;       // �t���[���Ԋu�i���t���[�����Ƃɕ`�悷�邩
    int         subdivisions;   // ��ԕ������i�傫���قǊ��炩�j
    float       thick;          // �g���C���̑���
    float       lifeTime;       // �g���C���̎���
    float       appearRate;     // �g���C�����o������m���i0.0f�`1.0f�j
}MODEL_FRAME_TRAIL_RENDERER_DESC;