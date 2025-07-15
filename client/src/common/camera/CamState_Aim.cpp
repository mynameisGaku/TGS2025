#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ���ėp
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"
#include "src/util/easing/EasingUtils.h"

// ���ʂŕK�v�ȕ�
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

#include "src/scene/play/chara/CharaManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/component/collider/CollisionFunc.h"

using namespace KeyDefine;
using namespace CameraDefine;

void Camera::AimState(FSMSignal sig)
{
	// �ړ��\��
	static bool canMove;
	static const float EASING_TIME = 1.0f;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
		canMove = true;

		m_EasingTime = EASING_TIME;

		m_PositionPrev = transform->Global().position;
		m_RotationPrev = transform->Global().rotation;
		m_OffsetPrev = Offset();
		m_TargetPrev = Target();

		// �L�����̊Ǘ���
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		// �Ǐ]����L����
		m_pFollowerChara = charaM->CharaInst(m_CharaIndex);	// �Ǐ]����L����
		m_pTargetChara = charaM->NearestEnemy(m_CharaIndex);// ��������L����
		if (m_pFollowerChara == nullptr || m_pTargetChara == nullptr) {
			ChangeState(&Camera::ChaseState);
			return;
		}

		// �R�[���͈̔͂ɓ����ċ��Ȃ��ꍇ
		if (not ColFunction::ColCheck_ConeToPoint(m_CameraCone, m_pTargetChara->transform->position).IsCollision() ||
			MouseController::Info().Move().GetLengthSquared() > 5.0f) {
			m_pTargetChara = nullptr;
			m_TargetTransitionTime = 0.5f;
			ChangeState(&Camera::ChaseState);
			return;
		}
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		m_EasingTime = max(m_EasingTime - GTime.DeltaTime(), 0.0f);

		// �L�����̊Ǘ���
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		m_pFollowerChara = charaM->CharaInst(m_CharaIndex);	// �Ǐ]����L����
		m_pTargetChara = charaM->NearestEnemy(m_CharaIndex);// ��������L����

		if (m_pFollowerChara == nullptr || m_pTargetChara == nullptr) {
			ChangeState(&Camera::ChaseState);
			return;
		}

		const Transform FOLLOWER_TRS = m_pFollowerChara->transform->Global();
		const Transform TARGET_TRS = m_pTargetChara->transform->Global();

		const Vector3 OFFSET = CAMERADEFINE_REF.m_OffsetChase;
		const Vector3 TARGET = TARGET_TRS.position + Vector3::SetY(140.0f);
		const Vector3 POSITION = FOLLOWER_TRS.position;
		const Vector3 TOVEC = TARGET - POSITION;
		const Vector3 ROTATION = Vector3(MathUtil::ToRadians(-20.0f), MathUtil::RotLimit(atan2f(TOVEC.x, TOVEC.z)), 0.0f);

		if (m_EasingTime > 0.0f)
		{
			if (fabsf(ROTATION.y - m_RotationPrev.y) > DX_PI_F) {
				if (m_RotationPrev.y > 0.0f)
					m_RotationPrev.y -= DX_TWO_PI_F;
				else if (m_RotationPrev.y < 0.0f)
					m_RotationPrev.y += DX_TWO_PI_F;
			}

			SetOffset(EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_OffsetPrev, OFFSET));
			SetTarget(EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_TargetPrev, TARGET));
			transform->position = EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_PositionPrev, POSITION);
			transform->rotation = EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_RotationPrev, ROTATION);
		}
		else
		{
			SetOffset(OFFSET);
			SetTarget(TARGET);
			transform->position = POSITION;
			transform->rotation = ROTATION;
		}

		MathUtil::ClampAssing(&transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
		MathUtil::RotLimitAssing(&transform->rotation.y);

		if (not InputManager::Hold("TargetCamera", m_pFollowerChara->GetIndex() + 1))
		{
			m_pTargetChara = nullptr;
			ChangeState(&Camera::ChaseState);
		}

		if (MouseController::Info().Move().GetLengthSquared() > 5.0f ||
			PadController::NormalizedRightStick(m_CharaIndex + 1).GetLengthSquared() >= KeyDefine::STICK_DEADZONE)
		{
			m_TargetTransitionTime = 0.5f;
			m_pTargetChara = nullptr;
			ChangeState(&Camera::ChaseState);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // �X�V��̍X�V (AfterUpdate)
	{

	}
	break;
	case FSMSignal::SIG_Exit: // �I�� (Exit)
	{
		m_pTargetChara = nullptr;
		canMove = true;
	}
	break;
	}
}