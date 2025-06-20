#pragma once

// ���ėp
#include "src/Util/math/Vector3.h"
#include <src/util/time/GameTime.h>

namespace CameraDefine {

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

	enum class CameraAnimState {
		None,			// �A�j���[�V�����Ȃ�
		Begin_To_End,	// �J�n�ʒu����I���ʒu�ֈړ���
		Hold,			// �I���ʒu�ŗ��܂��Ă���
		End_To_Begin,	// �I���ʒu����J�n�ʒu�֖߂�
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

	class CameraAnimData {
	public:
		Vector3 begin;			// �J�n�n�_
		Vector3 end;			// �I���n�_
		Vector3 beginTarget;	// �J�n�n�_�̒����_
		Vector3 endTarget;		// �I���n�_�̒����_
		Vector3 beginRotMatrix;	// �J�n�n�_�̍s��
		Vector3 endRotMatrix;	// �I���n�_�̍s��
		float beginToEndSec;	// �J�n�n�_����I���n�_�܂ł̎���
		float beginToEndSec_Max;// �J�n�n�_����I���n�_�܂ł̎���
		float endToBeginSec;	// �I���n�_����J�n�n�_�܂ł̎���
		float endToBeginSec_Max;// �I���n�_����J�n�n�_�܂ł̎���
		float holdTimeSec;		// ���܂鎞��

		bool isPlaying;	// �A�j���[�V���������ǂ���

		bool useAnim;	// �A�j���[�V�������g�p���邩�ǂ���
		bool useTarget;	// �����_���g�p���邩�ǂ���s
		bool useMatrix;	// �s����g�p���邩�ǂ���

		CameraAnimState state; // �A�j���[�V�����̏��

		CameraAnimData() : 
			begin(Vector3::Zero),
			end(Vector3::Zero),
			beginTarget(Vector3::Zero),
			endTarget(Vector3::Zero),
			beginRotMatrix(Vector3::Zero),
			endRotMatrix(Vector3::Zero),
			beginToEndSec(0.0f),
			beginToEndSec_Max(0.0f),
			endToBeginSec(0.0f),
			endToBeginSec_Max(0.0f),
			holdTimeSec(0.0f),
			isPlaying(false),
			useAnim(false),
			useTarget(false),
			useMatrix(false),
			state(CameraAnimState::None)
		{
		}

		~CameraAnimData() {}

		inline void SetAnim(const Vector3& begin, const Vector3& end, float beginToEnd, float endToBegin) {
			this->begin = begin;
			this->end = end;
			this->beginToEndSec = beginToEnd;
			this->beginToEndSec_Max = beginToEnd;
			this->endToBeginSec = endToBegin;
			this->endToBeginSec_Max = endToBegin;

			isPlaying = true; // �A�j���[�V�����J�n
			useAnim = true; // �A�j���[�V�������g�p����
			state = CameraAnimState::Begin_To_End; // �J�n�ʒu����I���ʒu�ֈړ���
		}

		inline void SetRotMat(const Vector3& begin, const Vector3& end, float beginToEnd, float endToBegin) {
			this->beginRotMatrix = begin;
			this->endRotMatrix = end;
			this->beginToEndSec = beginToEnd;
			this->beginToEndSec_Max = beginToEnd;
			this->endToBeginSec = endToBegin;
			this->endToBeginSec_Max = endToBegin;

			isPlaying = true; // �A�j���[�V�����J�n
			useMatrix = true; // �A�j���[�V�������g�p����
			state = CameraAnimState::Begin_To_End; // �J�n�ʒu����I���ʒu�ֈړ���
		}
		
		inline void SetHoldTime(float sec) {
			this->holdTimeSec = sec;
		}

		inline void SetUseAnim(bool use) {
			this->useAnim = use;
		}

		inline void SetUseTarget(bool use) {
			this->useTarget = use;
		}

		inline void SetUseMatrix(bool use) {
			this->useMatrix = use;
		}

		inline void Reset() {
			this->begin = Vector3::Zero;
			this->end = Vector3::Zero;
			this->beginTarget = Vector3::Zero;
			this->endTarget = Vector3::Zero;
			this->beginRotMatrix = Vector3::Zero;
			this->endRotMatrix = Vector3::Zero;
			this->beginToEndSec = 0.0f;
			this->beginToEndSec_Max = 0.0f;
			this->endToBeginSec = 0.0f;
			this->endToBeginSec_Max = 0.0f;
			this->holdTimeSec = 0.0f;
			this->isPlaying = false;
			this->useAnim = false;
			this->useTarget = false;
			this->useMatrix = false;
			this->state = CameraAnimState::None;
		}

		inline void Update() {

			if (not isPlaying)
				return;

			switch (state) {
			case CameraDefine::CameraAnimState::Begin_To_End:
				if (beginToEndSec > 0.0f) {
					beginToEndSec -= GTime.deltaTime;
				}
				else {
					beginToEndSec = 0.0f;
					state = CameraAnimState::Hold;
				}
				break;

			case CameraDefine::CameraAnimState::Hold:
				if (holdTimeSec > 0.0f) {
					holdTimeSec -= GTime.deltaTime;
				}
				else {
					holdTimeSec = 0.0f;
					state = CameraAnimState::End_To_Begin;
				}
				break;

			case CameraDefine::CameraAnimState::End_To_Begin:
				if (endToBeginSec > 0.0f) {
					endToBeginSec -= GTime.deltaTime;
				}
				else {
					endToBeginSec = 0.0f;
					state = CameraAnimState::None;
					isPlaying = false; // �A�j���[�V�����I��
				}
				break;

			default:
				break;
			}
		}
	};
}