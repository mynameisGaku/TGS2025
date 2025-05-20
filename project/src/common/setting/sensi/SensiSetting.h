#pragma once

// ���p����
#include "SettingBase.h"

/// <summary>
/// ���x�ݒ���
/// </summary>
class SensiSetting : public SettingBase {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^
	
	SensiSetting();
	~SensiSetting();

	//==========================================================================================
	// ���e��֐�

	void Update() override;

	void LoadCsv(std::string filename) override;
	void SaveCsv(std::string filename) override;
	void Apply() override;

private:
	Vector2 mouseSensitivity;	// �}�E�X���x
	Vector2 stickSensitivity;	// �X�e�B�b�N���x

	enum CsvElement {
		Label = 0,		// �ݒ荀�ږ�
		MouseSensiX,	// �}�E�X���x X��
		MouseSensiY,	// �}�E�X���x Y��
		StickSensiX,	// �X�e�B�b�N���x X��
		StickSensiY,	// �X�e�B�b�N���x Y��
	};
};