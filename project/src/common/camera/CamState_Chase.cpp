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

void Camera::ChaseState(FSMSignal sig)
{
	// �ړ��\��
	static bool canMove;
	static const float EASING_TIME = 0.5f;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
		canMove = true;

		m_TargetTransitionTime = 0.0f;
		m_EasingTime = EASING_TIME;

		m_PositionPrev = transform->Global().position;
		m_OffsetPrev = Offset();
		m_TargetPrev = Target();

		// �L�����N�^�[�̊Ǘ���
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		// �Ǐ]����L�����N�^�[
		m_FollowerChara = charaM->CharaInst(m_CharaIndex);
		// ��������L����
		m_TargetChara = charaM->NearestEnemy(m_CharaIndex);
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		m_TargetTransitionTime = max(m_TargetTransitionTime - GTime.DeltaTime(), 0.0f);
		m_EasingTime = max(m_EasingTime - GTime.DeltaTime(), 0.0f);

		// �L�����N�^�[�̊Ǘ���
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		// �Ǐ]����L�����N�^�[
		m_FollowerChara = charaM->CharaInst(m_CharaIndex);
		// ��������L����
		m_TargetChara = charaM->NearestEnemy(m_CharaIndex);

		if (m_FollowerChara == nullptr)
			return;

		const Transform FOLLOWER_TRS = m_FollowerChara->transform->Global();

		const Vector3 OFFSET = CAMERADEFINE_REF.m_OffsetChase;
		const Vector3 TARGET = CAMERADEFINE_REF.m_TargetChase * FOLLOWER_TRS.Matrix();
		const Vector3 POSITION = FOLLOWER_TRS.position;

		if (m_EasingTime > 0.0f)
		{
			SetOffset(EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_OffsetPrev, OFFSET));
			SetTarget(EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_TargetPrev, TARGET));
			transform->position = EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_PositionPrev, POSITION);
		}
		else
		{
			SetOffset(OFFSET);
			SetTarget(TARGET);
			transform->position = POSITION;
		}

		OperationByStick(m_CharaIndex + 1);
		
		MathUtil::ClampAssing(&transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
		MathUtil::RotLimitAssing(&transform->rotation.y);

		// ��������L���������݁A�{�^�����͂����ꂽ�ꍇ
		if (m_TargetChara != nullptr && InputManager::Hold("TargetCamera", m_FollowerChara->GetIndex() + 1))
		{
			// ���_�ړ����m
			if (MouseController::Info().Move().GetLengthSquared() > 5.0f ||
				PadController::NormalizedRightStick(m_CharaIndex + 1).GetLengthSquared() >= KeyDefine::STICK_DEADZONE)
				m_TargetTransitionTime = 0.5f;

			// �R�[���`��̔�����ɒ�������L����������ꍇ
			if (m_TargetTransitionTime <= 0.0f && ColFunction::ColCheck_ConeToPoint(cameraCone, m_TargetChara->transform->position).IsCollision())
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