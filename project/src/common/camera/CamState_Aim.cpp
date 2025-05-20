#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ���ėp
#include "src/util/time/GameTime.h"
#include "src/util/Utils.h"

// ���ʂŕK�v�ȕ�
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

#include "src/scene/play/chara/CharaManager.h"
#include "src/reference/camera/CameraDefineRef.h"

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
		const CharaBase* targetChara = charaM->TargetChara(m_CharaIndex);
		if (targetChara == nullptr)
			return;

		// �Ǐ]�L�����̃g�����X�t�H�[��
		const Transform charaTrs = chara->transform->Global();

		// �����L�����̃g�����X�t�H�[��
		const Transform targetTrs = targetChara->transform->Global();

		// �����_�̍���
		const Vector3 targetDiff = targetTrs.position + Vector3::SetY(100.0f) - target;

		transform->position = charaTrs.position;

		// �J�����̑��΍��W��ݒ�
		SetOffset(CAMERADEFINE_REF.m_OffsetChase);

		// �J�����̒����_��ݒ�
		SetTarget(target + targetDiff * Vector3(0.1f, 0.25f, 0.1f));

		// �����L�����ƃJ�����̍��W�̍���
		const Vector3 diffPos = targetTrs.position - transform->Global().position;

		// �J�������璍���L�����ւ̊p�x
		float targetRot = atan2f(diffPos.x, diffPos.z);

		// ���݂̌����ƒ����L�����ւ̊p�x�̍���
		float diffRot = targetRot - transform->rotation.y;

		// �J������X����]�l
		const float ROT_X = Math::DegToRad(-20.0f);

		// ��]�p�x��K��
		Function::RotLimit(&diffRot);
		transform->rotation.x += (ROT_X - transform->rotation.x) * 0.1f;
		transform->rotation.y += diffRot * 0.1f;
		Function::RotLimit(&transform->rotation.y);

		chara->transform->rotation.y = transform->rotation.y;

		//ColCheckToTerrain();

		if (not InputManager::Hold(KeyCode::RightClick))
		{
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
		canMove = true;
	}
	break;
	}
}