#pragma once

/// <summary>
/// ���ԂɊւ��鏈�����܂Ƃ߂�namespace
/// </summary>
namespace Time {

	/// <summary>
	/// ���ԉ��Z���@
	/// </summary>
	enum class AdditionMethod {
		None = 0,	// ���Z���Ȃ�
		Usual,		// ���Z����(���Ԍo�ߗ����܂܂Ȃ�)
		Rate		// ���Z����(���Ԍo�ߗ����܂�)
	};

	void Init();
	void Refresh();

	/// <summary>
	/// �f���^�^�C��
	/// </summary>
	float DeltaTime();

	/// <summary>
	/// ���Ԍo�ߗ���ݒ肷��
	/// </summary>
	/// <param name="rate">0.0f�`1.0f�̊�</param>
	void SetLapseRate(float rate);

	/// <summary>
	/// ���Ԍo�ߗ�
	/// </summary>
	float LapseRate();

	/// <summary>
	///  �f���^�^�C���Ɏ��Ԍo�ߗ�������������
	/// </summary>
	float DeltaTimeLapseRate();

	/// <summary>
	/// �q�b�g�X�g�b�v��ݒ肷��
	/// </summary>
	/// <param name="frame">�t���[����</param>
	void SetHitStop(int frame);

	/// <summary>
	/// ���݂̃q�b�g�X�g�b�v�̃t���[�������擾����
	/// </summary>
	int HitStop();

	/// <summary>
	/// �q�b�g�X�g�b�v����
	/// </summary>
	/// <returns>�q�b�g�X�g�b�v���Ȃ�true</returns>
	bool IsHitStop();

	//==================================================
	// ���ϊ�

	// �t���[����b�ɕϊ�
	template<typename Ty = float>
	inline float FrameToSec(Ty frame) { return static_cast<float>(frame) / 60.0f; }

	// �t���[���𕪂ɕϊ�
	template<typename Ty = float>
	inline float FrameToMin(Ty frame) { return static_cast<float>(frame) / 60.0f * 60.0f; }

	// �t���[�������Ԃɕϊ�
	template<typename Ty = float>
	inline float FrameToHour(Ty frame) { return static_cast<float>(frame) / 3600.0f * 60.0f; }

	// �b���t���[���ɕϊ�
	template<typename Ty = float>
	inline float SecToFrame(Ty sec) { return static_cast<float>(sec) * 60.0f; }

	// �����t���[���ɕϊ�
	template<typename Ty = float>
	inline float MinToFrame(Ty min) { return static_cast<float>(min) * 60.0f * 60.0f; }

	// ���Ԃ��t���[���ɕϊ�
	template<typename Ty = float>
	inline float HourToFrame(Ty hour) { return static_cast<float>(hour) * 60.0f * 60.0f * 60.0f; }
};
