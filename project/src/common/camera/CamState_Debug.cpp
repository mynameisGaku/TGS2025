#include "Camera.h"
#include "cameraDefine.h"

// ◇汎用
#include "../Library/time.h"
#include "Util/Utils.h"

// ◇個別で必要な物
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"

using namespace KeyDefine;

void Camera::DebugState(FSMSignal sig)
{
    // 移動可能か
    static bool canMove;

    switch (sig)
    {
    case FSMSignal::SIG_Enter: // 初期化 (Constractor)
    {
        canMove = true;
    }
        break;
    case FSMSignal::SIG_Update: // 更新 (Update)
    {
        // 右クリックを検知
        if (InputManager::Push(KeyCode::RightClick)) {
            canMove = !canMove;

            // 移動可能な場合、マウスカーソルを画面中央に固定する
            if (canMove)
                MouseController::SetMouseMovement(MouseMovement::Fixed);
            // 移動不可能な場合、マウスカーソルを自由に操作できるようにする
            else
                MouseController::SetMouseMovement(MouseMovement::Free);
        }

        if (canMove)
            MoveProcess();
    }
        break;
    case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
    {

    }
        break;
    case FSMSignal::SIG_Exit: // 終了 (Exit)
    {
        canMove = true;
    }
        break;
    }
}