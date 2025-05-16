#include "AttachedAnimation.h"
#include "Library/myDxLib.h"
#include "Library/time.h"
#include <assert.h>

namespace
{
	//ToDo:�A�j���[�^�[������炤�悤�ɂ���
	static const std::string ROOT_NAME = "mixamorig9:Hips";
	static const int ANIM_FRAMERATE = 30;
}

AttachedAnimation::AttachedAnimation(int parentModel, const AnimInfo& info)
{
	assert(parentModel >= 0);

	m_info = info;

	m_parentModel = parentModel;	// ���f���n���h��
	m_attachID = -1;	// �A�^�b�`�σA�j���[�V�����̃n���h��

	m_frame = 0.0f;	// ���݃t���[���i����������j
	m_maxFrame = MV1GetAnimTotalTime(info.handle, 0);	// �ő�t���[��
	m_playSpeed = 1.0f;	// �Đ����x�{��
	m_blendRate = 0.0f;	// �u�����h��
	m_defaultBlendRate = 1.0f;	// �����u�����h��

	m_first = true;	// �ŏ��̃A�b�v�f�[�g��true
}

AttachedAnimation::~AttachedAnimation()
{
	if (m_attachID >= 0)
		MV1DetachAnim(m_parentModel, m_attachID);
}

void AttachedAnimation::updateFrame()
{
	// ���Ԃ�i�߂�
	m_frame += Time::DeltaTimeLapseRate() * m_playSpeed * ANIM_FRAMERATE;

	// �A�j���[�V�����I�����Ԃ𒴂�����
	if (m_frame >= m_maxFrame) {
		// ���[�v�Ȃ�߂�
		if (m_info.option.isLoop) {
			m_frame -= m_maxFrame;
		}
		// ���[�v����Ȃ���΍ő�l�Ŏ~�߂�
		else {
			m_frame = m_maxFrame;
		}
	}
	// �t�Đ����̃��[�v����
	else if (m_frame < 0) {
		// ���[�v�Ȃ�߂�
		if (m_info.option.isLoop) {
			m_frame += m_maxFrame;
		}
		// ���[�v����Ȃ���΍ŏ��l�Ŏ~�߂�
		else {
			m_frame = 0;
		}
	}
}

void AttachedAnimation::RefreshDefaultBlendRate()
{
	m_defaultBlendRate = BlendRate();
}

//===============================================================================

AttachedAnimation_Main::AttachedAnimation_Main(int parentModel, const AnimInfo& info) : AttachedAnimation(parentModel, info)
{
	m_hRoot = MV1SearchFrame(m_parentModel, ROOT_NAME.c_str());	// ���f���̃��[�g�t���[���̃n���h��
	m_rootMatrix = MGetIdent();	// ���[�g�t���[���ړ��p�̍s��
}

void AttachedAnimation_Main::Update()
{
	if (m_first)
	{
		m_attachID = MV1AttachAnim(m_parentModel, 0, m_info.handle);

		m_first = false;
	}

	// ���Ԃ�i�߂�
	updateFrame();

	// �Đ����Ԃ�K�p
	MV1SetAttachAnimTime(m_parentModel, m_attachID, m_frame);
	MV1SetAttachAnimBlendRate(m_parentModel, m_attachID, m_blendRate * m_defaultBlendRate);
}

void AttachedAnimation_Main::UpdateRootMatrix()
{
	// ���[�g�̈ړ��s����擾
	m_rootMatrix = MV1GetFrameLocalMatrix(m_parentModel, m_hRoot);

	// ���␳���̍��W���擾
	const Vector3 framePos = MV1GetAttachAnimFrameLocalPosition(m_parentModel, m_attachID, m_hRoot);
	// ���W�ړ���ł�����
	m_rootMatrix *= MGetTranslate(framePos * -1.0f);
	Vector3 matPos = framePos;

	// �Œ胋�[�g�Ȃ�0�ɂ���
	for (int i = 0; i < 3; i++)
	{
		if (m_info.option.isFixedRoot[i])
		{
			matPos.Set(i, 0);
		}
	}
	// �I�t�Z�b�g���ړ��ʂɑ����ēK�p
	matPos += m_info.option.offset;
	m_rootMatrix *= MGetTranslate(matPos);
}

//===============================================================================

AttachedAnimation_Sub::AttachedAnimation_Sub(int parentModel, const AnimInfo& info, std::string target) : AttachedAnimation(parentModel, info)
{
	m_targetID = MV1SearchFrame(parentModel, target.c_str());
}

void AttachedAnimation_Sub::Update()
{
	if (m_first)
	{
		m_attachID = MV1AttachAnim(m_parentModel, 0, m_info.handle);
		MV1SetAttachAnimBlendRate(m_parentModel, m_attachID, 0.0f);

		m_first = false;
	}

	// ���Ԃ�i�߂�
	updateFrame();

	// �Đ����Ԃ�K�p
	MV1SetAttachAnimTime(m_parentModel, m_attachID, m_frame);
	MV1SetAttachAnimBlendRateToFrame(m_parentModel, m_attachID, m_targetID, m_blendRate * m_defaultBlendRate);
}
