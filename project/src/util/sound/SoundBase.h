#pragma once

// ���p����
#include "../Library/gameObject.h"

// ���ėp
#include "easing.h"
#include "../Source/Util/Vector3.h"

// ���ʂŕK�v�ȕ�
#include "soundDefine.h"

/// <summary>
/// �T�E���h���Đ�����N���X
/// </summary>
class SoundBase : public GameObject {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	SoundBase();
	~SoundBase();

	//================================================================================
	// ���e��֐�

	void Update() override;

	/// <summary>
	/// �T�E���h���Đ�����
	/// </summary>
	/// <param name="info">�Đ�����T�E���h�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	void Play(SoundDefine::SoundInfo* info, const std::string& label);

	/// <summary>
	/// �p����ݒ肵�čĐ�����
	/// </summary>
	/// <param name="info">�Đ�����T�E���h�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	/// <param name="pos">�Đ�������W</param>
	void PlaySetPan(SoundDefine::SoundInfo* info, const std::string& label, Vector3* pos);

	/// <summary>
	/// ���g����ݒ肵�čĐ�����
	/// </summary>
	/// <param name="id">�T�E���h�̎��</param>
	/// <param name="frequency">���g��</param>
	/// <param name="label">���ʃ��x��</param>
	void PlaySetFrequency(SoundDefine::SoundInfo* info, const std::string& label, const float& frequency = -1.0f);

	/// <summary>
	/// �T�E���h�̎��g����ݒ肷��
	/// </summary>
	/// <param name="frequency">���g��</param>
	void SetFrequency(const float& frequency);

	/// <summary>
	/// �Đ� �t�F�[�h�C����
	/// </summary>
	/// <param name="info">�T�E���h�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	/// <param name="duration">�Đ��܂ł̎���(�b)</param>
	/// <param name="easingType">�C�[�W���O�̎��</param>
	/// <param name="beginVolume">�t�F�[�h�J�n���̉���</param>
	/// <param name="endVolume">�t�F�[�h�I�����̉���</param>
	void FadeIn(SoundDefine::SoundInfo* info, const std::string& label, const SoundDefine::SoundFade& fade);

	/// <summary>
	/// ��~ �t�F�[�h�A�E�g��
	/// </summary>
	/// <param name="duration">��~�܂ł̎���(�b)</param>
	/// <param name="easingtype">�C�[�W���O�̎��</param>
	/// <param name="beginVolume">�t�F�[�h�J�n���̉���</param>
	/// <param name="endVolume">�t�F�[�h�I�����̉���</param>
	/// <param name="isFadeOutEnd">�t�F�[�h�I�����ɃT�E���h���~�����邩</param>
	void FadeOut(const SoundDefine::SoundFade& fade, const bool& isFadeOutEnd = true);

	/// <summary>
	/// �T�E���h���~�߂�
	/// </summary>
	void Stop();

	/// <summary>
	/// �p���̐ݒ���s��
	/// </summary>
	/// <param name="pos">�Đ����W</param>
	void SetPan(Vector3* pos);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���g�̎��G�t�F�N�g�����擾����
	/// </summary>
	inline const SoundDefine::SoundInfo* Info() const { return info; }

	/// <summary>
	/// ���ʔԍ����擾����
	/// </summary>
	inline const std::string Label() const { return label; }

	/// <summary>
	/// �t�F�[�h�������擾����
	/// </summary>
	inline const bool IsFade() const { return fade.info.isActive; }

	/// <summary>
	/// �Đ�����Ă��邩���擾����
	/// </summary>
	/// <returns>�Đ�����Ă�����true</returns>
	bool IsPlaying() const;

	/// <summary>
	/// �������`�F�b�N���s��
	/// </summary>
	/// <param name="typeName">���O</param>
	/// <param name="label">���ʃ��x��</param>
	/// <returns>�����Ă����true</returns>
	bool CheckConsistency(const std::string& typeName, const std::string& label) const;

private:

	/// <summary>
	/// �t�F�[�h�ɂ�鉹�ʕω�����
	/// </summary>
	void applyFadeVolume();

	//================================================================================
	// �������o�ϐ�

	SoundDefine::SoundInfo* info;	// ���g�̃T�E���h���
	SoundDefine::SoundFade fade;	// �t�F�[�h�֘A

	std::string label;	// ���ʃ��x��
	bool isFadeOutEnd;	// �t�F�[�h�A�E�g�I����ɃT�E���h���~���邩
	bool isSetPan;		// �p���ݒ���s����
	Vector3* playPos;	// �Đ����W
};