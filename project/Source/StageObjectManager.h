#pragma once

// ���ėp
#include "config.h"
#include "../Source/Util/Transform.h"
#include <list>
#include <vector>

// ���ʂŕK�v�ȕ�
#include "stageDefine.h"
#include "stageObject.h"

/// <summary>
/// �X�e�[�W�I�u�W�F�N�g���Ǘ�����N���X
/// </summary>
namespace StageObjectManager {

	//==========================================================================================
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
	/// �X�V����
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

	//==========================================================================================
	// �������蔻��֘A

	/// <summary>
	/// �J�v�Z���ƁAStageObject�Ƃ̓����蔻�������
	/// </summary>
	/// <param name="p1">�J�v�Z���̓_1</param>
	/// <param name="p2">�J�v�Z���̓_2</param>
	/// <param name="r">���a</param>
	/// <param name="push">�����Ԃ��x�N�g��</param>
	/// <returns>����������True</returns>
	bool CollCheckCapsule(Vector3 p1, Vector3 p2, float r, Vector3* push);

	/// <summary>
	/// �`��p���f���Ɠ����蔻��p���f����؂�ւ���
	/// </summary>
	void HitModelDrawSwitch();

	//==========================================================================================
	// ���f�[�^�Ǘ��֘A

	/// <summary>
	/// Csv�t�@�C������X�e�[�W�̃f�[�^��ǂݍ���
	/// </summary>
	void LoadToCsv(const std::string& filename);

	/// <summary>
    /// Json�t�@�C������X�e�[�W�̃f�[�^��ǂݍ���
	/// </summary>
	void LoadFromJson(const std::string& filename);

	/// <summary>
	/// Csv�t�@�C���ɃX�e�[�W�̃f�[�^��ۑ�����
	/// </summary>
	void SaveToCsv();

    /// <summary>
    /// Json�t�@�C���ɃX�e�[�W�̃f�[�^��ۑ�����
    /// </summary>
    void SaveToJson();

	/// <summary>
	/// Csv�t�@�C���ɃX�e�[�W�̃f�[�^�������o��
	/// </summary>
	void OutPutToCsv(const std::string& filename);

	/// <summary>
	/// �f�[�^�̕ۑ��Ɠǂݍ��݂���Ăɍs��
	/// </summary>
	void SaveAndLoad();

	/// <summary>
	/// Csv�f�[�^����ēx�ǂݍ��݂��s��
	/// </summary>
	void ReloadStageData();

	//==========================================================================================
	// ���Z�b�^�[

	void SetModelFilePath(const std::string& filename);

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �w�肵��ID�̃X�e�[�W�I�u�W�F�N�g�̏����擾����
	/// </summary>
	/// <param name="id">�擾�������X�e�[�W�I�u�W�F�N�g��ID</param>
	/// <returns>�w�肵��ID�����݂���ꍇ�A���̏���Ԃ��B���݂��Ȃ��ꍇ�Anullptr��Ԃ�</returns>
	StageObject* GetStageObject(int id);

	/// <summary>
	/// �z�u����Ă���X�e�[�W�I�u�W�F�N�g�̃C���X�^���X�������X�g���擾����
	/// </summary>
	std::vector<StageObject*> StageObjects();

	/// <summary>
	/// �z�u����Ă���X�e�[�W�I�u�W�F�N�g�̎�ޖ��������X�g���擾
	/// </summary>
	std::vector<std::string> StageObjectsTheString();

	//==========================================================================================
	// ���X�e�[�W�G�f�B�b�g�@�\

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g�𐶐�����
	/// </summary>
	/// <param name="type">�X�e�[�W�I�u�W�F�N�g�̎��</param>
	/// <param name="transform">����������W�E��]�E�g�k�̒l</param>
	/// <param name="collider">�����蔻���t���邩</param>
	void Create(StageObjInfo info, const Transform& transform, bool collider = false);
	void CreateTheString(const std::string& objName);

	/// <summary>
	/// �w�肵��ID�̃X�e�[�W�I�u�W�F�N�g���폜����
	/// </summary>
	/// <param name="id">�폜����X�e�[�W�I�u�W�F�N�g��ID</param>
	/// <returns>�폜������������True</returns>
	bool Erase(int id);
	bool EraseTheString(const std::string& id);

	/// <summary>
	/// �S�ẴX�e�[�W�I�u�W�F�N�g����������
	/// </summary>
	void EraseAll();

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g���폜�����ۂɁA���������Ԃ𖄂߂�B
	/// </summary>
	/// <param name="id">�폜���ꂽ�X�e�[�W�I�u�W�F�N�g��ID</param>
	void ShiftID(int id);

	/// <summary>
	/// �ҏW���[�h�̍ۂ̕`�揈��
	/// </summary>
	void DrawEditMode();

#ifdef IMGUI

	/// <summary>
	/// �f�o�b�O�Ŏg�p����ImGui�֘A�̋@�\�̏��������s��(�X�e�[�W�G�f�B�b�g�@�\)
	/// </summary>
	void InitImGui();

	/// <summary>
	/// �f�o�b�O�Ŏg�p����ImGui�֘A�̋@�\���X�V����
	/// </summary>
	void UpdateImGui();

#endif // IMGUI
}
