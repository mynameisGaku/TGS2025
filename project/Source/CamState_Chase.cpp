#include "Camera.h"
#include "cameraDefine.h"
#include "TinyFSM.h"

// ���ėp
#include "../Library/time.h"
#include "Util/Utils.h"

// ���ʂŕK�v�ȕ�
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"

#include "CharaManager.h"

using namespace CameraDefine;

void Camera::ChaseState(FSMSignal sig)
{
    // �ړ��\��
    static bool canMove;

    switch (sig)
    {
    case FSMSignal::SIG_Enter: // ������ (Constractor)
    {
        canMove = true;
    }
    break;
    case FSMSignal::SIG_Update: // �X�V (Update)
    {
        OperationByMouse();
        OperationByStick();

        CharaManager* charaM = FindGameObject<CharaManager>();
        if (charaM == nullptr)
            return;

        const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
        if (chara == nullptr)
            return;

        const Transform charaTrs = chara->transform->Global();

        SetOffset(CAMERA_OFFSET_CHASE);
        SetTarget(Vector3(0.0f, 100.0f, 0.0f) * charaTrs.Matrix());
        transform->position = charaTrs.position;

        ColCheckToTerrain();

        transform->rotation.x = Math::Clamp(transform->rotation.x, CAMERA_ROT_X_MIN, CAMERA_ROT_X_MAX);

        if (transform->rotation.y < -Math::PI)
            transform->rotation.y += Math::PI_TW;
        else if (transform->rotation.y > Math::PI)
            transform->rotation.y -= Math::PI_TW;
    }
    break;
    case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V (AfterUpdate)
    {

    }
    break;
    case FSMSignal::SIG_Exit: // �I�� (Exit)
    {
        canMove = true;
    }
    break;
    }
}