#pragma once
#include "AnimationDefine.h"
#include <DxLib.h>
#include <list>

/// <summary>
/// �A�^�b�`���̃A�j���[�V�����̊Ǘ��N���X
/// </summary>
/// <author>�����h�l</author>
class AttachedAnimation
{
public:
	AttachedAnimation(int parentModel, const AnimInfo& info);
	~AttachedAnimation();

	virtual void Update() = 0;
	void RefreshDefaultBlendRate();

	inline float MaxFrame() const { return m_maxFrame; }
	inline bool IsLoop() const { return m_info.option.isLoop; }

	/*=== �A�N�Z�T ===*/
	// ���݂̐i�s�x�i0..1�j
	inline void SetFrameByRatio(float ratio) { m_frame = m_maxFrame * ratio; }
	// ���݂̐i�s�x�i0..1�j
	inline float FrameByRatio() const { return m_maxFrame > 0 ? m_frame / m_maxFrame : 1.0f; }
	// ���݂̃t���[��
	inline void SetFrame(float frame) { m_frame = frame; }
	// ���݂̃t���[��
	inline float Frame() const { return m_frame; }
	// �Đ����x
	inline void SetPlaySpeed(float speed) { m_playSpeed = speed; }
	// ���f���Ɋ|����u�����h��
	inline float BlendRate() const { return m_blendRate * m_defaultBlendRate; }
	// ���f���Ɋ|����u�����h��
	inline void SetBlendRate(float rate) { m_blendRate = rate; }
	// ���f���Ɋ|����u�����h��
	inline float DefaultBlendRate() const { return m_defaultBlendRate; }
	// ���f���Ɋ|����u�����h��
	inline void SetDefaultBlendRate(float rate) { m_defaultBlendRate = rate; }
	// �A�j���[�V�������
	inline const AnimInfo& Info() const { return m_info; }
	// �A�^�b�`�σA�j���[�V�����̃n���h��
	inline int AttachID() const { return m_attachID; }
protected:
	// �Đ���i�߂�
	void updateFrame();

	AnimInfo m_info;

	int m_parentModel;	// ���f���n���h��
	int m_attachID;	// �A�^�b�`�σA�j���[�V�����̃n���h��

	float m_frame;	// ���݃t���[���i����������j
	float m_maxFrame;	// �ő�t���[��
	float m_playSpeed;	// �Đ����x�{��
	float m_blendRate;	// �u�����h��
	float m_defaultBlendRate;	// �����u�����h��

	bool m_first;	// �ŏ��̃A�b�v�f�[�g��true

};

// �S�̂ɂ�����A�j���[�V����
class AttachedAnimation_Main : public AttachedAnimation
{
public:
	AttachedAnimation_Main(int parentModel, const AnimInfo& info);

	void Update() override;

	// ���[�g�t���[���̍s����X�V
	void UpdateRootMatrix();

	inline const MATRIX& RootMatrix() const { return m_rootMatrix; }
private:
	int m_hRoot;	// ���f���̃��[�g�t���[���̃n���h��
	MATRIX m_rootMatrix;	// ���[�g�t���[���ړ��p�̍s��
};

// ���C���A�j���[�V�������ꕔ�㏑������T�u�A�j���[�V����
class AttachedAnimation_Sub : public AttachedAnimation
{
public:
	AttachedAnimation_Sub(int parentModel, const AnimInfo& info, std::string target);

	void Update() override;
private:
	int m_targetID;	// �A�j���[�V������K�p����t���[��ID
};
