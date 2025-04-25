#pragma once
#include "EffectBase.h"
#include "EffectDefine.h"

#include "config.h"

using namespace EffectDefine;

/// <summary>
/// �G�t�F�N�g�̓ǂݍ��݁E�Đ��E�Ǘ����s��
/// </summary>
namespace EffectManager {

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
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	//================================================================================
	// ���ǂݍ��ݏ���

	/// <summary>
	/// �G�t�F�N�g��ǂݍ���
	/// </summary>
	/// <param name="info">�G�t�F�N�g���</param>
	void Load(const EffectInfo& info);

	/// <summary>
	/// Csv�f�[�^����G�t�F�N�g�����擾���ēǂݍ���
	/// </summary>
	/// <param name="filename">Csv�f�[�^�����݂���t�@�C���p�X</param>
	void LoadToCsv(const std::string& filename);

	/// <summary>
	/// Json����G�t�F�N�g�����擾���ēǂݍ���
	/// </summary>
	/// <param name="filename">Json�̃t�@�C���p�X</param>
	void LoadFromJson(const std::string& filename);

	//================================================================================
	// ���Đ�����

	/// <summary>
	/// �G�t�F�N�g��3D��ԏ�ɐ�������
	/// </summary>
	/// <param name="typeName">�G�t�F�N�g�̃f�[�^��</param>
	/// <param name="trs">���W�E��]�E�g�k�̏��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="isLoop">���[�v�Đ����s����</param>
	/// <returns>���������G�t�F�N�g�̎���</returns>
	EffectBase* Play3D(const std::string& typeName, const Transform& trs, const std::string& label, const bool& isLoop = false);

	/// <summary>
	/// �G�t�F�N�g��2D��ԏ�ɐ�������
	/// </summary>
	/// <param name="typeName">�G�t�F�N�g�̃f�[�^��</param>
	/// <param name="trs">���W�E��]�E�g�k�̏��</param>
	/// <param name="label">���ʖ�</param>
	/// <param name="isLoop">���[�v�Đ����s����</param>
	/// <returns>���������G�t�F�N�g�̎���</returns>
	EffectBase* Play2D(const std::string& typeName, const Transform& trs, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// �G�t�F�N�g���Đ�����Ă��邩
	/// </summary>
	/// <param name="typeName">�G�t�F�N�g�̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	/// <returns>�Đ�����Ă����ꍇ�A���̎��̂�Ԃ��B�Đ�����Ă��Ȃ����nullptr��Ԃ��B</returns>
	EffectBase* IsPlaying(const std::string& typeName, const std::string& label = "");

	//================================================================================
	// ����~����

	/// <summary>
	/// �G�t�F�N�g���~����
	/// </summary>
	/// <param name="typeName">�G�t�F�N�g�̃f�[�^��</param>
	/// <param name="label">���ʖ�</param>
	void Stop(const std::string& typeName, const std::string& label);

	/// <summary>
	/// �S�ẴG�t�F�N�g���~����
	/// </summary>
	void StopAll();

	//================================================================================
	// ���f�[�^�������

	/// <summary>
	/// Csv�f�[�^����ēx�ǂݍ��݂��s��
	/// </summary>
	void Reload();

	/// <summary>
	/// �S�ẴG�t�F�N�g�f�[�^���������
	/// </summary>
	void AllReleaseInfo();

#ifdef IMGUI
	
	/// <summary>
	/// �f�o�b�O�Ŏg�p����ImGui�֘A�̋@�\�̏��������s��(�ǂݍ��񂾃G�t�F�N�g�̃v���r���[���s��)
	/// </summary>
	void InitImGui();

#endif // IMGUI

}