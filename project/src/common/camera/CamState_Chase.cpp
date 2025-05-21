#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/Utils.h"

// ◇個別で必要な物
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

#include "src/scene/play/chara/CharaManager.h"
#include "src/reference/camera/CameraDefineRef.h"

using namespace KeyDefine;
using namespace CameraDefine;

void Camera::ChaseState(FSMSignal sig)
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
        OperationByMouse();
        OperationByStick();

        // キャラクターの管理者
        CharaManager* charaM = FindGameObject<CharaManager>();
        if (charaM == nullptr)
            return;

        // 追従するキャラクター
        const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
        if (chara == nullptr)
            return;

        // キャラクターのトランスフォーム
        const Transform charaTrs = chara->transform->Global();

        // 注視点の差異
        const Vector3 targetDiff = (CAMERADEFINE_REF.m_TargetChase * charaTrs.Matrix()) - target;

        // カメラの相対座標を設定
        SetOffset(CAMERADEFINE_REF.m_OffsetChase);

        // カメラの注視点を設定
        SetTarget(target + targetDiff * Vector3(0.1f, 0.25f, 0.1f));
        
        transform->position = charaTrs.position;

        ColCheckToTerrain();

        transform->rotation.x = Math::Clamp(transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
        Function::RotLimit(&transform->rotation.y);

        // 注視するキャラ
        const CharaBase* targetChara = charaM->TargetChara(m_CharaIndex);

        if (InputManager::Hold(KeyCode::RightClick) && targetChara != nullptr)
        {
            ChangeState(&Camera::AimState);
        }
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