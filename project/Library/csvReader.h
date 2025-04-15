#pragma once
#include "../Library/myDxLib.h"
#include <string>
#include <vector>

/// <summary>
/// CSV�t�@�C����ǂݍ��ރN���X
/// </summary>
class CsvReader {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	CsvReader();
	CsvReader(std::string _filename);
	~CsvReader();

	//================================================================================
	// ���e��֐�

	/// <summary> 
	/// CSV�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="_filename">�t�@�C���p�X</param>
	void Load(std::string _filename);

	/// <summary>
	/// �����f�[�^������������
	/// </summary>
	void Reset();

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="column">��</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̕�����</returns>
	std::string GetString(int column, int row);

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="type">��ɏ����ꂽ����</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̕�����</returns>
	std::string GetString(std::string type, int row);

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="column">��</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̐���</returns>
	int GetInt(int column, int row);

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="type">��ɏ����ꂽ����</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̐���</returns>
	int GetInt(std::string type, int row);

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="column">��</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̏���</returns>
	float GetFloat(int column, int row);

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="type">��ɏ����ꂽ����</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̏���</returns>
	float GetFloat(std::string type, int row);

	/// <summary>
	/// �ۂ��擾����
	/// </summary>
	/// <param name="column">��</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̉�</returns>
	bool GetBool(int column, int row);

	/// <summary>
	/// �ۂ��擾����
	/// </summary>
	/// <param name="type">��ɏ����ꂽ����</param>
	/// <param name="row">�s</param>
	/// <returns>�w�肵���s��̉�</returns>
	bool GetBool(std::string type, int row);

	// �񐔂��擾����
	int Column() const { return static_cast<int>(records.size()); }

	// �s�����擾����
	int Row() const { return static_cast<int>(records[max(Column() - 1, 0)].size()); }

	bool CheckSize(int colume, int row) {

		if (records.size() <= colume)
			return false;
		if (records[colume].size() <= row)
			return false;

		return true;
	}

	std::vector<std::vector<std::string>> Records() const { return records; }

private:
	std::vector<std::vector<std::string>> records;	// CSV�t�@�C���ɏ����ꂽ������ێ�����
	std::string filename;	// CSV�̃t�@�C���p�X
};