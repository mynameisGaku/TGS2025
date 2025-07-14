#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"

#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"

using namespace CameraDefine;

void Camera::PerformanceState(FSMSignal sig)
{
    // 移動可能か
    static bool canMove;

    switch (sig)
    {
    case FSMSignal::SIG_Enter: // 初期化 (Constractor)
    {
        canMove = true;

		m_Offset = Vector3(0.0f, 0.0f, 0.0f);
		m_OffsetPrev = Vector3(0.0f, 0.0f, 0.0f);
    }
    break;
    case FSMSignal::SIG_Update: // 更新 (Update)
    {
		// 演出が設定されていない場合
		if (m_PerformanceDatas.empty())
		{
			ChangeState(&Camera::ChaseState);
			return;
		}

		PerformanceData prevData;	// 前回の演出情報
		prevData.easingInfo.isActive = false;
		int endCount = 0;	// 終了した演出の数を保持する

		// ▼カメラワークの処理
		for (auto& itr : m_PerformanceDatas)
		{
			// 自身の処理が終了している場合、次の要素へ移る
			if (not itr.easingInfo.isActive)
			{
				prevData = itr;
				endCount++;
				continue;
			}

			// 一つ前の要素が終了していない場合、処理を止める
			if (prevData.easingInfo.isActive)
				break;

			Vector3 nextPos = itr.position;
			Vector3 nextTarget = itr.target;

			Vector3 prevPos = prevData.position;
			Vector3 prevTarget = prevData.target;

			// 総効果時間が設定されている場合、線形補間処理を行う
			if (itr.easingInfo.totaltime > 0.0f && itr.easingInfo.time < itr.easingInfo.totaltime) {
				itr.easingInfo.time = MathUtil::Clamp(itr.easingInfo.time + GTime.DeltaTime(), 0.0f, itr.easingInfo.totaltime);

				EasingFunc::Process(&itr.easingInfo, &transform->position, nextPos, prevPos);
				EasingFunc::Process(&itr.easingInfo, &m_Target, nextTarget, prevTarget);
			}
			else {
				transform->position = nextPos;
				m_Target = nextTarget;
			}

			// 演出時間が終了した場合
			if (itr.easingInfo.time <= 0.0f) {
				itr.afterTaste = max(itr.afterTaste - GTime.DeltaTime(), 0.0f);

				// 余韻が残っている場合
				if (itr.afterTaste > 0.0f)
					break;

				itr.easingInfo.isActive = false;
				endCount++;
			}

			prevData = itr;
		}

		// 注視点への向きを求めて、適応させる
		float dir = atan2f(TargetLay().x, TargetLay().z);
		if (dir > DX_PI_F)			dir -= DX_TWO_PI_F;
		else if (dir < -DX_PI_F)	dir += DX_TWO_PI_F;
		transform->rotation.y = dir;

		// 演出の数と終了した演出の数を比較する
		//if (m_PerformanceDatas.size() == endCount)
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