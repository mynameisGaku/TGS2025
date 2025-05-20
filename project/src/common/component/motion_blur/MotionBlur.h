#pragma once

// ���p����
#include "src/common/component/Component.h"

// ���ėp
#include "src/util/transform/Transform.h"
#include <vector>

/// <summary>
/// ���[�V�����u���[�̏��
/// </summary>
struct MotionBlurInfo {
	Transform transform;	// ���W�E��]�E�g�k
	int model;				// ���f��
	float duration;			// ���ʎ���
	float totalDuration;	// �����ʎ���

	MotionBlurInfo() : 
		transform(Transform()),
		model(-1),
		duration(0.0f),
		totalDuration(0.0f) {
	}
	MotionBlurInfo(Transform trs, int _model, float _totalDuration) :
		transform(trs),
		model(_model),
		duration(_totalDuration),
		totalDuration(_totalDuration) {
	}
};

/// <summary>
/// ���[�V�����u���[�Ɋւ���N���X
/// </summary>
class MotionBlur : public Component {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	MotionBlur();
	~MotionBlur();

	//================================================================================
	// ���e��֐�

	void Init();
	void Update() override;
	void Draw() override;

	// ���[�V�����u���[���N������
	void SetMotionBlur(bool value);

private:
	//================================================================================
	// ���v���C�x�[�g�֐�

	void AllDeleteMotionBlur();

	std::vector<MotionBlurInfo> motionBlur;	// ���[�V�����u���[�̏��
	float counter;	// �J�E���^�[
	bool isActive;	// ���[�V�����u���[�����ʒ���
};

