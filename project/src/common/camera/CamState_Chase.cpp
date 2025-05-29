#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ���ėp
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtils.h"

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

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
		canMove = true;

		m_TargetTransitionTime = 0.0f;
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		if (m_TargetTransitionTime > 0.0f)
			m_TargetTransitionTime -= GTime.DeltaTime();
		else
			m_TargetTransitionTime = 0.0f;

		OperationByMouse();
		OperationByStick(m_CharaIndex + 1);

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

		// �J�����̑��΍��W��ݒ�
		SetOffsetAfter(CAMERADEFINE_REF.m_OffsetChase);

		// �J�����̒����_��ݒ�
		SetTargetAfter(CAMERADEFINE_REF.m_TargetChase * charaTrs.Matrix());
		
		// �J�������W�ƒǏ]����L�����̍��W����v������
		transform->position = charaTrs.position;

		MathUtil::ClampAssing(&transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
		MathUtil::RotLimit(&transform->rotation.y);

		// ��������L����
		m_TargetChara = charaM->TargetChara(m_CharaIndex);

		// ��������L���������݁A�{�^�����͂����ꂽ�ꍇ
		if (m_TargetChara != nullptr && InputManager::Hold("TargetCamera"))
		{
			// �}�E�X�̈ړ����m
			if (MouseController::Info().Move().GetLengthSquared() > 5.0f)
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