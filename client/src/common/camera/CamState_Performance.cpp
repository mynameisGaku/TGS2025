#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"

#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"

using namespace CameraDefine;

void Camera::PerformanceState(FSMSignal sig)
{
    // �ړ��\��
    static bool canMove;

    switch (sig)
    {
    case FSMSignal::SIG_Enter: // ������ (Constractor)
    {
        canMove = true;

		m_Offset = Vector3(0.0f, 0.0f, 0.0f);
		m_OffsetPrev = Vector3(0.0f, 0.0f, 0.0f);
    }
    break;
    case FSMSignal::SIG_Update: // �X�V (Update)
    {
		// ���o���ݒ肳��Ă��Ȃ��ꍇ
		if (m_PerformanceDatas.empty())
		{
			ChangeState(&Camera::ChaseState);
			return;
		}

		PerformanceData prevData;	// �O��̉��o���
		prevData.easingInfo.isActive = false;
		int endCount = 0;	// �I���������o�̐���ێ�����

		// ���J�������[�N�̏���
		for (auto& itr : m_PerformanceDatas)
		{
			// ���g�̏������I�����Ă���ꍇ�A���̗v�f�ֈڂ�
			if (not itr.easingInfo.isActive)
			{
				prevData = itr;
				endCount++;
				continue;
			}

			// ��O�̗v�f���I�����Ă��Ȃ��ꍇ�A�������~�߂�
			if (prevData.easingInfo.isActive)
				break;

			Vector3 nextPos = itr.position;
			Vector3 nextTarget = itr.target;

			Vector3 prevPos = prevData.position;
			Vector3 prevTarget = prevData.target;

			// �����ʎ��Ԃ��ݒ肳��Ă���ꍇ�A���`��ԏ������s��
			if (itr.easingInfo.totaltime > 0.0f && itr.easingInfo.time < itr.easingInfo.totaltime) {
				itr.easingInfo.time = MathUtil::Clamp(itr.easingInfo.time + GTime.DeltaTime(), 0.0f, itr.easingInfo.totaltime);

				EasingFunc::Process(&itr.easingInfo, &transform->position, nextPos, prevPos);
				EasingFunc::Process(&itr.easingInfo, &m_Target, nextTarget, prevTarget);
			}
			else {
				transform->position = nextPos;
				m_Target = nextTarget;
			}

			// ���o���Ԃ��I�������ꍇ
			if (itr.easingInfo.time <= 0.0f) {
				itr.afterTaste = max(itr.afterTaste - GTime.DeltaTime(), 0.0f);

				// �]�C���c���Ă���ꍇ
				if (itr.afterTaste > 0.0f)
					break;

				itr.easingInfo.isActive = false;
				endCount++;
			}

			prevData = itr;
		}

		// �����_�ւ̌��������߂āA�K��������
		float dir = atan2f(TargetLay().x, TargetLay().z);
		if (dir > DX_PI_F)			dir -= DX_TWO_PI_F;
		else if (dir < -DX_PI_F)	dir += DX_TWO_PI_F;
		transform->rotation.y = dir;

		// ���o�̐��ƏI���������o�̐����r����
		//if (m_PerformanceDatas.size() == endCount)
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