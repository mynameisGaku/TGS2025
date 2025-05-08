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
#include "CameraDefineRef.h"

using namespace KeyDefine;
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

        // �L�����N�^�[�̊Ǘ���
        CharaManager* charaM = FindGameObject<CharaManager>();
        if (charaM == nullptr)
            return;

        // �Ǐ]����L�����N�^�[
        const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
        if (chara == nullptr)
            return;

        // �L�����N�^�[�̃g�����X�t�H�[��
        const Transform charaTrs = chara->transform->Global();

        // �����_�̍���
        const Vector3 targetDiff = (CAMERADEFINE_REF.m_TargetChase * charaTrs.Matrix()) - target;

        // �J�����̑��΍��W��ݒ�
        SetOffset(CAMERADEFINE_REF.m_OffsetChase);

        // �J�����̒����_��ݒ�
        SetTarget(target + targetDiff * Vector3(0.1f, 0.25f, 0.1f));
        
        transform->position = charaTrs.position;

        ColCheckToTerrain();

        transform->rotation.x = Math::Clamp(transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
        Function::RotLimit(&transform->rotation.y);

        if (InputManager::Hold(KeyCode::RightClick))
        {
            ChangeState(&Camera::AimState);
        }
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