#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ���ėp
#include "src/util/time/GameTime.h"

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
		targetChara = charaM->TargetChara(m_CharaIndex);
		if (targetChara == nullptr) {
			ChangeState(&Camera::ChaseState);
			return;
		}

		// �R�[���͈̔͂ɓ����ċ��Ȃ��ꍇ
		if (not ColFunction::ColCheck_ConeToPoint(cameraCone, targetChara->transform->position).IsCollision()) {
			bool inCone = ColFunction::ColCheck_ConeToPoint(cameraCone, targetChara->transform->position).IsCollision();
			targetChara = nullptr;
			ChangeState(&Camera::ChaseState);
			return;
		}

		const Transform charaTrs = chara->transform->Global();			// �Ǐ]�L�����̃g�����X�t�H�[��
		const Transform targetTrs = targetChara->transform->Global();	// �����L�����̃g�����X�t�H�[��

		// �J�������W�ƒǏ]�L�������W����v������
		transform->position = charaTrs.position;

		// �J�����̑��΍��W��ݒ�
		SetOffset(CAMERADEFINE_REF.m_OffsetAim);

		const Vector3 targetDiff = targetTrs.position + Vector3::SetY(100.0f) - target;	// �����_�̍���

		// �J�����̒����_��ݒ�
		SetTarget(target + targetDiff * Vector3(0.1f, 0.25f, 0.1f));

		const Vector3 diffPos = targetTrs.position - transform->Global().position;	// �����L�����ƃJ�����̍��W�̍���
		float targetRot = atan2f(diffPos.x, diffPos.z);		// �J�������璍���L�����ւ̊p�x
		float diffRot = targetRot - transform->rotation.y;	// ���݂̌����ƒ����L�����ւ̊p�x�̍���
		const float ROT_X = MathUtil::ToRadians(-20.0f);			// �J������X����]�l

		// ��]�p�x��K��
		MathUtil::RotLimit(&diffRot);
		transform->rotation.x += (ROT_X - transform->rotation.x) * 0.1f;
		transform->rotation.y += diffRot * 0.1f;
		MathUtil::RotLimit(&transform->rotation.y);

		// �J�����ƃL�����̌����𑵂���
		chara->transform->rotation.y = transform->rotation.y;

		//ColCheckToTerrain();

		if (not InputManager::Hold(KeyCode::RightClick))
		{
			targetChara = nullptr;
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
		targetChara = nullptr;
		canMove = true;
	}
	break;
	}
}