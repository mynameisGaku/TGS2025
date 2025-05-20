#include "Camera.h"
#include "cameraDefine.h"

// ���ėp
#include "../Library/time.h"
#include "Util/Utils.h"

// ���ʂŕK�v�ȕ�
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"

using namespace KeyDefine;

void Camera::DebugState(FSMSignal sig)
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
        // �E�N���b�N�����m
        if (InputManager::Push(KeyCode::RightClick)) {
            canMove = !canMove;

            // �ړ��\�ȏꍇ�A�}�E�X�J�[�\������ʒ����ɌŒ肷��
            if (canMove)
                MouseController::SetMouseMovement(MouseMovement::Fixed);
            // �ړ��s�\�ȏꍇ�A�}�E�X�J�[�\�������R�ɑ���ł���悤�ɂ���
            else
                MouseController::SetMouseMovement(MouseMovement::Free);
        }

        if (canMove)
            MoveProcess();
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