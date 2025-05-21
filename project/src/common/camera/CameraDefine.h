#pragma once

// ���ėp
#include "src/Util/Utils.h"
#include "src/Util/math/Vector3.h"

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
			target(Vector3::Zero),
			position(Vector3::Zero),
			holder("")
		{
		}

		~StagingData(){}
	};
}