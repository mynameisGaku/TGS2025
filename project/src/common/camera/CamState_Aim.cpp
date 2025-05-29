#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ���ėp
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"

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

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // ������ (Constractor)
	{
		canMove = true;
	}
	break;
	case FSMSignal::SIG_Update: // �X�V (Update)
	{
		// �L�����N�^�[�̊Ǘ���
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		// �Ǐ]����L����
		const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
		if (chara == nullptr)
			return;

		// ��������L����
		m_TargetChara = charaM->TargetChara(m_CharaIndex);
		if (m_TargetChara == nullptr || MouseController::Info().Move().GetLengthSquared() > 5.0f) {
			m_TargetTransitionTime = 0.5f;
			ChangeState(&Camera::ChaseState);
			return;
		}

		// �R�[���͈̔͂ɓ����ċ��Ȃ��ꍇ
		if (not ColFunction::ColCheck_ConeToPoint(cameraCone, m_TargetChara->transform->position).IsCollision()) {
			m_TargetChara = nullptr;
			ChangeState(&Camera::ChaseState);
			return;
		}

		const Transform charaTrs = chara->transform->Global();			// �Ǐ]�L�����̃g�����X�t�H�[��
		const Transform targetTrs = m_TargetChara->transform->Global();	// �����L�����̃g�����X�t�H�[��
		Vector3 targetOffset = Vector3::SetY(100.0f);					// �����_�̑��΍��W
		Vector3 targetPos = targetTrs.position + targetOffset;

		// �J�������W�ƒǏ]����L�����̍��W����v������
		transform->position = charaTrs.position;

		// �J�����̑��΍��W��ݒ�
		SetOffsetAfter(CAMERADEFINE_REF.m_OffsetAim);

		// �J�����̒����_��ݒ�
		SetTargetAfter(targetPos);

		const float TARGET_DIR = targetPos.Direction(transform->Global().position);	// �J�������璍���L�����ւ̊p�x
		const float ROT_X = MathUtil::ToRadians(-20.0f);			// �J������X����]�l
		float diffRot = TARGET_DIR - transform->rotation.y;	// ���݂̌����ƒ����L�����ւ̊p�x�̍���

		// ��]�p�x�ɐ�����������
		MathUtil::RotLimit(&diffRot);

		transform->rotation.x += (ROT_X - transform->rotation.x) * 0.1f;
		transform->rotation.y += diffRot * 0.1f;

		// ��]�p�x�ɐ�����������
		MathUtil::RotLimit(&transform->rotation.y);

		// �J�����ƃL�����̌����𑵂���
		//chara->transform->rotation.y = transform->rotation.y;

		if (not InputManager::Hold("TargetCamera"))
		{
			m_TargetChara = nullptr;
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
		m_TargetChara = nullptr;
		canMove = true;
	}
	break;
	}
}