#pragma once

// ���p����
#include "Component.h"

// ���ėp
#include "../../Library/myDxLib.h"
#include "../../Library/csvReader.h"
#include <vector>
#include <string>
#include <unordered_map>

// ���ʂŕK�v�ȕ�
#include "AnimationEvent.h"

class Object3D;

/// <summary>
/// �A�j���[�V�����̍Đ����s���R���|�[�l���g
/// </summary>
class Animator : public Component {
public:
	//==========================================================================================
	// ���\����

	/// <summary>
	/// �A�j���[�V�����������
	/// </summary>
	struct AnimInfo {
		int handle;			// �A�j���[�V�����̃n���h��
		bool isLoop;		// ���[�v�Đ��̗L��
		bool isFixedRoot;	// ���[�J�����W�̌Œ艻�̗L��
		float startFrame;	// �Đ��J�n�t���[��
		float endFrame;		// �Đ��I���t���[��
		float defAnimSpeed;	// ����Đ����x
		std::vector<AnimationEvent*> event;	// �A�j���[�V�����̃R�}���h
		std::string animName;

		AnimInfo() : 
			handle(-1),
			isLoop(false),
			isFixedRoot(false),
			startFrame(0.0f),
			endFrame(0.0f),
			defAnimSpeed(1.0f),
			animName("")
		{
		}
	};

	/// <summary>
	/// �A�^�b�`(�Đ�)���Ă���A�j���[�V�����̏�� 
	/// </summary>
	struct AttachInfo {
		int attachID;		// �A�^�b�`ID
		std::string attachName;
		float nowFrame;		// ���݂̍Đ��t���[��
		float beforeFrame;	// 1�t���[���O�̍Đ��t���[��
		float maxFrame;		// ���Đ��t���[��

		AttachInfo() :
			attachID(0),
			attachName(""),
			nowFrame(0.0f),
			beforeFrame(0.0f),
			maxFrame(0.0f)
		{
		}
	};

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

	// ��Ɋ|����s����Z�b�g
	void SetOffsetMatrix(const MATRIX& matrix) { offsetMatrix = matrix; }

	void Update() override;
	void Draw() override {};

	/// <summary>
	/// �A�j���[�V�����t�@�C�������[�h����
	/// </summary>
	/// <param name="csvFilePath">CSV�f�[�^������t�@�C����</param>
	/// <param name="animFilePath">�A�j���[�V�����f�[�^������t�@�C����</param>
	void LoadAnimCsv(std::string csvFilePath, std::string animFilePath);

	/// <summary>
	/// �A�j���[�V�����t�@�C�������[�h����
	/// </summary>
	/// <param name="filename">�A�j���[�V�����f�[�^������t�@�C����</param>
	/// <param name="label">�A�j���[�V�����̃��x����</param>
	/// <param name="loop">���[�v�A�j���[�V�����̏ꍇ��true</param>
	/// <param name="fixedRoot">root�����̏�Œ肷��ꍇ��true</param>
	/// <param name="extension">�g���q</param>
	void LoadAnim(std::string animFilePath, std::string label, bool loop = false, bool fixedRoot = false, std::string extension = ".mv1");

	/// <summary>
	/// �A�j���[�V�������Đ�����
	/// �����A�j���[�V�����̏ꍇ�́A��������
	/// </summary>
	/// <param name="label">�A�j���[�V�����̃��x����</param>
	/// <param name="speed">�Đ����x</param>
	void Play(std::string label, float speed = -1.0f);

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �A�j���[�V�����̍Đ����x�i�{���j��ύX����
	/// �W�����x��1.0f
	/// </summary>
	/// <param name="speed">�Đ����x</param>
	inline void SetPlaySpeed(float speed) { playSpeed = speed; }

	/// <summary>
	/// ���̃A�j���[�V�����̐i�s�x���Z�b�g
	/// </summary>
	/// <param name="ratio">�i�s�x�i0..1�j</param>
	inline void SetCurrentRatio(float ratio) { current.nowFrame = current.maxFrame * max(min(ratio, 1.0f), 0.0f); }

	/// <summary>
	/// ���݂̃A�j���[�V�����̃t���[����ݒ肷��
	/// </summary>
	/// <param name="frame">�A�j���[�V�����̃t���[��</param>
	inline void SetCurrentFrame(float frame) { current.nowFrame = max(min(frame, current.maxFrame), 0.0f); }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���[�v���Ȃ��A�j���[�V�������Ō�܂ōĐ����������ׂ�
	/// </summary>
	/// <returns>�Ō��true</returns>
	inline bool IsFinished() {

		if (anims[playingLabel].isLoop)
			return false;

		return (current.nowFrame >= current.maxFrame);
	};

	inline const std::unordered_map<std::string, AnimInfo> GetAllAnimInfo() const { return anims; }

	inline AnimInfo GetAnimInfo(std::string type) {

		if (anims.contains(type) == false)
			return AnimInfo();

		return anims[type];
	}

	/// <summary>
	/// ���̃A�j���[�V�����̏����擾����
	/// </summary>
	inline const AttachInfo& CurrentAttach() const { return current; }

	/// <summary>
	/// ���̃A�j���[�V�����̐i�s�x��0..1�ŕԂ�
	/// </summary>
	/// <returns>���̐i�s�x</returns>
	inline float CurrentRatio() const { return current.nowFrame / current.maxFrame; }

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

	enum AnimInfoElement {
		None = -1,
		MotionDescription,	// ���[�V�����̐���
		ResourceName,		// ���\�[�X��
		LabelName,			// ���x����
		IsLoop,				// ���[�v�Đ��̗L��
		IsFixedRoot,		// ���W�Œ艻�̗L��
		StartFrame,			// �Đ��J�n�t���[��
		EndFrame,			// �Đ��I���t���[��
		DefAnimSpeed,		// ����Đ����x
		Command,			// �R�}���h
		Details,			// ���s���e
		RunFrame,			// ���s�t���[��
		Max
	};

	Object3D* parentObj;
	CsvReader* animCsv;	// �A�j���[�V������CSV�f�[�^
	std::unordered_map<std::string, AnimInfo> anims;	// �A�j���[�V�����̏��

	AttachInfo current;	// ���݃A�^�b�`���̃A�j���[�V�������
	AttachInfo prev;	// ��O�ɃA�^�b�`���Ă����A�j���[�V�������

	int parentModel;	// �e�̃��f��
	float mergeTime;	// �u�����h�̌o�ߑ��x(1�b�Ԃŉ��t���[���i�ނ�)
	float mergeTimeMax;	// ���b�ŕ⊮�����邩
	float playSpeed;	// �A�j���[�V�����̍Đ����x
	float frameRate;	// �A�j���[�V�����̃t���[���o�ߑ��x

	std::string origin;	// ���f���̌��_�̖��O
	std::string playingLabel;	// �Đ����̃A�j���[�V�����̖��D

	MATRIX offsetMatrix;	// ���[�g�ɏ�Ɋ|����s��
};