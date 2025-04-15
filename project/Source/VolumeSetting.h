#pragma once

// ���p����
#include "SettingBase.h"

/// <summary>
/// ���ʐݒ���
/// </summary>
class VolumeSetting : public SettingBase {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^
	
	VolumeSetting();
	~VolumeSetting();

	//==========================================================================================
	// ���e��֐�

	void Update() override;

	void LoadCsv(std::string filename) override;
	void SaveCsv(std::string filename) override;
	void Apply() override;

private:
	enum CsvElement {
		Label = 0,		// �ݒ荀�ږ�
		Volume_Master,	// �S�̉���
		Volume_BGM,		// �w�i����
		Volume_SE,		// ���ʉ���
		Volume_Voice,	// ������
		Volume_ENV,		// ������
	};
};