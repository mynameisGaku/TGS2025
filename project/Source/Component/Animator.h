#pragma once

// ���p����
#include "Component.h"

// ���ėp
#include <unordered_map>
#include <assert.h>

// ���ʂɕK�v�Ȃ���
#include "AnimationDefine.h"
#include "AttachedAnimation.h"

class Object3D;

/// <summary>
/// �A�j���[�V�����̍Đ����s���R���|�[�l���g
/// </summary>
/// <author>�~�b�`�A�����h�l</author>
class Animator : public Component {
public:
	//==========================================================================================
	// ���\����

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Animator();
	~Animator();

	//==========================================================================================
	// ���e��֐�

	/// <summary>
	/// ���������s��
	/// </summary>
	/// <param name="origin">���f���̌��_�̖��O</param>
	/// <param name="frameRate">�t���[���̌o�ߑ��x(1�b�Ԃŉ��t���[���i�ނ�)</param>
	/// <param name="mergeTimeMax">���b�ŕ⊮�����邩</param>
	void Init(std::string origin, float frameRate, float mergeTimeMax);

	void Update() override;
	void Draw() override {};

	/// <summary>
	/// �A�j���[�V���������[�h
	/// </summary>
	/// <param name="folder">�A�j���[�V����������t�H���_�̃p�X</param>
	/// <param name="name">�A�j���[�V������</param>
	/// <param name="option">�ݒ荀��</param>
	void LoadAnim(std::string folder, std::string name, AnimOption option);

	/// <summary>
	/// �A�j���[�V������Json�����Ɉꊇ���[�h
	/// </summary>
	/// <param name="path">Json�t�@�C���̃p�X</param>
	void LoadAnimsFromJson(std::string path);

	/// <summary>
	/// �A�j���[�V�������Đ�����
	/// �����A�j���[�V�����̏ꍇ�́A��������
	/// </summary>
	/// <param name="label">�A�j���[�V�����̃��x����</param>
	/// <param name="speed">�Đ����x</param>
	void Play(std::string label, float speed = -1.0f);

	void PlaySub(std::string frameName, std::string label, float speed = -1.0f);
	void StopSub(std::string frameName);

	void DeleteAnimInfos();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �A�j���[�V�����̍Đ����x�i�{���j��ύX����
	/// �W�����x��1.0f
	/// </summary>
	/// <param name="speed">�Đ����x</param>
	void SetPlaySpeed(float speed);

	/// <summary>
	/// ���̃A�j���[�V�����̐i�s�x���Z�b�g
	/// </summary>
	/// <param name="ratio">�i�s�x�i0..1�j</param>
	inline void SetCurrentRatio(float ratio) { current->SetFrameByRatio(ratio); }

	/// <summary>
	/// ���݂̃A�j���[�V�����̃t���[����ݒ肷��
	/// </summary>
	/// <param name="frame">�A�j���[�V�����̃t���[��</param>
	inline void SetCurrentFrame(float frame) { current->SetFrame(frame); }

	/// <summary>
	/// �w��t���[���ɍs����Z�b�g
	/// </summary>
	/// <param name="frameName">�Ώۂ̃t���[���̖��O</param>
	/// <param name="matrix"></param>
	void SetFrameMatrix(std::string frameName, const MATRIX& matrix) {
		assert(MV1SearchFrame(parentModel, frameName.c_str()) >= 0);
		frameMatrix[frameName] = matrix;
	}

	void SetOffsetMatrix(const MATRIX& matrix) {
		offsetMatrix = matrix;
	}

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���[�v���Ȃ��A�j���[�V�������Ō�܂ōĐ����������ׂ�
	/// </summary>
	/// <returns>�Ō��true</returns>
	inline bool IsFinished() {

		if (anims[playingLabel].option.isLoop)
			return false;

		return (current->Frame() >= current->MaxFrame());
	};

	/// <summary>
	/// ���[�v���Ȃ��A�j���[�V�������Ō�܂ōĐ����������ׂ�
	/// </summary>
	/// <returns>�Ō��true</returns>
	inline bool IsFinishedSub(std::string frame) {
		if (not currentSubs.contains(frame))
			return false;

		AttachedAnimation_Sub* currentSub = currentSubs.at(frame);

		if (currentSub->Info().option.isLoop)
			return false;

		return (currentSub->Frame() >= currentSub->MaxFrame());
	};

	inline const std::unordered_map<std::string, AnimInfo> GetAllAnimInfo() const { return anims; }

	inline AnimInfo GetAnimInfo() const {
		return GetAnimInfo(playingLabel);
	}

	inline AnimInfo GetAnimInfo(std::string type) const {

		if (anims.contains(type) == false)
			return AnimInfo();

		return anims.at(type);
	}

	/// <summary>
	/// ���̃A�j���[�V�����̐i�s�x���t���[���ŕԂ�
	/// </summary>
	/// <returns>���݂̃t���[��</returns>
	inline float CurrentFrame() const { return current->Frame(); }

	/// <summary>
	/// ���̃A�j���[�V�����̐i�s�x��0..1�ŕԂ�
	/// </summary>
	/// <returns>���݂̐i�s�x</returns>
	inline float CurrentRatio() const { return current->FrameByRatio(); }

	/// <summary>
	/// ���݂̍Đ����x���擾����
	/// </summary>
	inline float CurrentPlaySpeed() const { return playSpeed; }

	/// <summary>
	/// �u�����h�̌o�ߑ��x���擾����
	/// </summary>
	inline float MargeTime() const { return mergeTime; }

private:
	//==========================================================================================
	// �������o�ϐ�
	std::unordered_map<std::string, AnimInfo> anims;	// �A�j���[�V�����̏��

	std::list<AttachedAnimation_Main*> prevs;
	AttachedAnimation_Main* current;

	std::unordered_map<std::string, AttachedAnimation_Sub*> currentSubs;

	int parentModel;	// �e�̃��f��
	float mergeTime;	// �u�����h�̌o�ߑ��x(1�b�Ԃŉ��t���[���i�ނ�)
	float mergeTimeMax;	// ���b�ŕ⊮�����邩
	float playSpeed;	// �A�j���[�V�����̍Đ����x
	float frameRate;	// �A�j���[�V�����̃t���[���o�ߑ��x

	std::string origin;	// ���f���̌��_�̖��O
	std::string playingLabel;	// �Đ����̃A�j���[�V�����̖��D

	std::unordered_map<std::string, MATRIX> frameMatrix;
	MATRIX offsetMatrix;
};