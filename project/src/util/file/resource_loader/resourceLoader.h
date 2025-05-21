#pragma once
#include <string>

/// <summary>
/// �摜�A�T�E���h�A���f�����̃f�[�^��ǂݍ���
/// ���ɓǂݍ���ł��镨���A�ēx�ǂݍ��܂ꂽ�ꍇ�A���ɂ��镨�̃R�s�[��n��
/// </summary>
namespace ResourceLoader {

	void Init();

	//==========================================================================================
	// ���摜�֘A

	int LoadGraph(std::string filename);
	void DeleteGraph(int handle);

	//==========================================================================================
	// ���T�E���h�֘A

	int LoadSoundMem(std::string filename);
	void DeleteSoundMem(int handle);

	//==========================================================================================
	// �����f���֘A

	int MV1LoadModel(std::string filename);
	void MV1DeleteModel(int handle);

	//==========================================================================================
	// ���e��֐�

	void LoadFolderFile(std::string folder, bool loadSubFolder = false);

	/// <summary>
	/// �񓯊��ǂݍ��݂����邩��ݒ肷��
	/// </summary>
	/// <param name="async">�񓯊��ǂݍ��݂���ꍇtrue</param>
	void SetAsync(bool async);

	/// <summary>
	/// �񓯊��Ńt�@�C����ǂ�ł��邩
	/// </summary>
	/// <returns>�t�@�C����ǂ�ł���r����true</returns>
	bool IsLoading();

	/// <summary>
	/// �ǂ񂾃t�@�C����S������
	/// </summary>
	void ReleaseAllFile();
};