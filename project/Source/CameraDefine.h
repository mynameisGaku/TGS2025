#pragma once

// ���ėp
#include "../Source/Util/Utils.h"
#include "../Source/Util/Vector3.h"

namespace CameraDefine {

	/// <summary>
	/// �J�����̊e��X�e�[�g
	/// </summary>
	enum State {
		sNone = -1,
		sTitle,			// �^�C�g��
		sPlay,			// �v���C
		sResult,		// ���U���g
		sChase,
		sTarget,		// ����
		sPerformance,	// ���o
		sTeam,			// �`�[���v���C
		sDebug,			// �f�o�b�O
		sMax
	};

	static const float CAMERA_NEAR = 10.0f;		// �`�揈�����s���ߕӂ̈ʒu
	static const float CAMERA_FAR = 100000.0f;	// �`�揈�����s�������̈ʒu

	static const float CAMERA_PERSPECTIVE = Math::DegToRad(60.0f);	// ����p
	static const float ROT_SPEED_LIMIT = Math::DegToRad(13.0f);		// �J�����̉�]���x�̐���

	static const float CAMERA_ROT_X_MIN = Math::DegToRad(-85.0f);	// X��]�̍ŏ��l
	static const float CAMERA_ROT_X_MAX = Math::DegToRad(30.0f);	// X��]�̍ő�l

	static const Vector3 CAMERA_OFFSET_DEF = Vector3(0.0f, 100.0f, -200.0f);	// �J�����̊��葊�΍��W
	static const Vector3 CAMERA_TARGET_DEF = Vector3(0.0f, 100.0f, 100.0f);		// �J�����̊��蒍�����W

	static const Vector3 CAMERA_OFFSET_CHASE = Vector3(0.0f, 170.0f, -320.0f);		// �ǔ��J�����̊��葊�΍��W
	static const Vector3 CAMERA_OFFSET_AIM = Vector3(80.0f, 220.0f, -30.0f);		// �G�C���J�����̊��葊�΍��W
	static const Vector3 CAMERA_OFFSET_GAME_END = Vector3(-150.0f, 100.0f, -150.0f);// �Q�[���I�����̃J�����̊��葊�΍��W

	static const float SHAKE_MIN = 4.0f;	// �J�����U���̍ŏ��l
	static const float SHAKE_MAX = 10.0f;	// �J�����U���̍ŏ��l

	/// <summary>
	/// �J�������o�̎��
	/// </summary>
	enum class CameraWorkType {
		None = -1,
		Type,			// ���
		Opening,		// �J�n��
		PlayerLose,		// �s�k
		PlayerWin,		// ����
		PlayerAtkSp,	// �v���C���[�̕K�E�Z
		Max
	};

	/// <summary>
	/// �J�������o���̗v�f
	/// </summary>
	enum class CameraWorkElement {
		None = -1,
		Type,		// ���
		Duration,	// ���ʎ���
		TargetX,	// �����_(X��)
		TargetY,	// �����_(Y��)
		TargetZ,	// �����_(Z��)
		PositionX,	// ���W(X��)
		PositionY,	// ���W(Y��)
		PositionZ,	// ���W(Z��)
		AffterTaste,// �]�C
		Holder,		// �J�����ۗ̕L��
		Max
	};

	/// <summary>
	/// �J�������o�̕�ԂŎg�p������
	/// </summary>
	class StagingData {
	public:
		float duration;		// ���ʎ���
		float totalDuration;// �����ʎ���
		float affterTaste;	// �]�C
		bool end;			// �I��������
		Vector3 position;	// ���W
		Vector3 target;		// �����_
		std::string holder;	// �ۗL��

		StagingData() :
			duration(0.0f),
			totalDuration(0.0f),
			affterTaste(0.0f),
			end(false),
			target(V3::ZERO),
			position(V3::ZERO),
			holder("")
		{
		}

		~StagingData(){}
	};
}