#pragma once
#include "src/util/sound/SoundBase.h"
#include "src/util/sound/SoundDefine.h"

#include <unordered_map>
#include <vector>

using namespace SoundDefine;

/// <summary>
/// �����̓ǂݍ��݁E�Đ��E�Ǘ����s��
/// </summary>
namespace SoundManager {

	//================================================================================
	// ���e��֐�

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// Update�̑O�Ɉ�x�����Ăяo�����
	/// </summary>
	void Start();

	/// <summary>
	// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// �J�e�S���[�ʂ̉��ʔ{����K��������
	/// </summary>
	void ApplyCategoryVolume();

	//================================================================================
	// ���ǂݍ��ݏ���

	/// <summary>
	/// ������ǂݍ���
	/// </summary>
	/// <param name="info">�������</param>
	void Load(const SoundInfo& info);

	/// <summary>
	/// Csv�f�[�^���特�������擾���ēǂݍ���
	/// </summary>
	/// <param name="filename">Csv�f�[�^�����݂���t�@�C���p�X</param>
	void LoadToCsv(const std::string& filename);

	//================================================================================
	// ���Đ�����

	/// <summary>
	/// �������Đ�����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <returns>�������������̎���</returns>
	SoundBase* Play(const std::string& typeName, const std::string& label);

	/// <summary>
	/// �����������_���ɍĐ�����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <returns>�������������̎���</returns>
	SoundBase* PlayRandom(const std::vector<std::string>& typeName, const std::string& label);

	/// <summary>
	/// �����𗧑̉����ōĐ�����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="playPos">�Đ����W</param>
	/// <returns>�������������̎���</returns>
	SoundBase* PlaySetPan(const std::string& typeName, const std::string& label, Vector3* playPos);

	/// <summary>
	/// �����̐������W��ݒ肷��
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="playPos">�Đ����W</param>
	void SetPan(const std::string& typeName, const std::string& label, Vector3* playPos);

	/// <summary>
	/// �����̎��g����ύX���čĐ�����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="frequency">�Đ����g��(�w���c�P�ʁB100�`100,000 : -1�Ńf�t�H���g�ɖ߂�)</param>
	/// <remarks>
	/// �l���������قǁA�x���Ⴍ�A�l���傫���قǑ��������Đ�����܂��B
	/// ���̌��̍Đ����g����22.05KHz�������ꍇ�́y22050�z���w�肵���ꍇ���ʏ�̍Đ��l�ƂȂ�A
	/// 44.10KHz�������ꍇ�́y44100�z���ʏ�̍Đ��l�ƂȂ�B
	/// </remarks>
	/// <returns>�������������̎���</returns>
	SoundBase* PlaySetFrequency(const std::string& typeName, const std::string& label, const float& frequency);

	/// <summary>
	/// �����̎��g����ݒ肷��
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="frequency">�Đ����g��(�w���c�P�ʁB100�`100,000 : -1�Ńf�t�H���g�ɖ߂�)</param>
	void SetFrequency(const std::string& typeName, const std::string& label, const float& frequency);

	/// <summary>
	/// �����̉��ʂ����X�ɏグ�Ȃ���Đ�����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="sec">���ʎ���(�b)</param>
	/// <param name="easing">��ԕ��@�̎��</param>
	/// <returns>�������������̎���</returns>
	SoundBase* FadeIn(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing = EasingType::Linear);

	/// <summary>
	/// �����̉��ʂ����X�ɉ�����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="sec">���ʎ���(�b)</param>
	/// <param name="easing">��ԕ��@�̎��</param>
	/// <param name="isFadeOutEnd">���ʂ�0�ɂȂ������ɁA�����̍Đ����I�����邩</param>
	/// <returns>�������������̎���</returns>
	SoundBase* FadeOut(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing = EasingType::Linear, const bool& isFadeOutEnd = true);

	/// <summary>
	/// �������Đ�����Ă��邩
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <returns>�Đ�����Ă����ꍇ�A���̎��̂�Ԃ��B�Đ�����Ă��Ȃ����nullptr��Ԃ��B</returns>
	SoundBase* IsPlaying(const std::string& typeName, const std::string& label);

	//================================================================================
	// ����~����

	/// <summary>
	/// �������~����
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	void Stop(const std::string& typeName, const std::string& label);

	/// <summary>
	/// ���ނ��Ƃ̉������~����
	/// </summary>
	/// <param name="category">��~���鉹���̕���</param>
	void StopCategory(const SoundCategory& category);

	/// <summary>
	/// �S�Ẳ������~����
	/// </summary>
	void StopAll();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���ނ��Ƃ̉��ʂ�ݒ肷��
	/// </summary>
	/// <param name="category">�ݒ肷�鉹���̕���</param>
	/// <param name="rate">���ʂ̔{��(0..1)</param>
	void SetVolumeRate(const SoundCategory& category, const float& rate);

	//================================================================================
	// ���Q�b�^�[

	std::unordered_map<SoundCategory, float>* CategoryVolumeRate();

	//================================================================================
	// ���m�F����

	/// <summary>
	/// �f�[�^���ǂݍ��܂�Ă��邩�𔻒肷��
	/// </summary>
	/// <param name="typeName">�����̃f�[�^��</param>
	/// <returns>�ǂݍ��܂�Ă����ꍇ�Atrue�B</returns>
	bool CheckLoadDate(const std::string& typeName);

	//================================================================================
	// ���f�[�^�������

	/// <summary>
	/// Csv�f�[�^����ēx�ǂݍ��݂��s��
	/// </summary>
	void Reload();

	/// <summary>
	/// �S�Ẳ����f�[�^���������
	/// </summary>
	void AllReleaseInfo();

#ifdef IMGUI
	
	/// <summary>
	/// �f�o�b�O�Ŏg�p����ImGui�֘A�̋@�\�̏��������s��(�ǂݍ��񂾃T�E���h�̃v���r���[���s��)
	/// </summary>
	void InitImGui();

#endif // IMGUI

};