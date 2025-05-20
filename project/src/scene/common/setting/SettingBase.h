#pragma once

// ���p����
#include "../Library/gameObject.h"

// ���ėp
#include <fstream>
#include <assert.h>
#include "../Library/csvReader.h"

#include "Util/Vector2.h"
#include "UI_Manager.h"
#include "EasingUtils.h"

/// <summary>
/// �ݒ荀�ڂ̊��N���X
/// </summary>
class SettingBase : public GameObject {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	SettingBase();
	virtual ~SettingBase();

	//==========================================================================================
	// ���e��֐�

	virtual void Update() override;

	/// <summary>
	/// Csv����ݒ荀�ڂ̐��l���擾����
	/// </summary>
	virtual void LoadCsv(std::string filename) {}

	/// <summary>
	/// Csv�ɐݒ荀�ڂ̐��l��ۑ�����
	/// </summary>
	virtual void SaveCsv(std::string filename) {}

	/// <summary>
	/// �ݒ肵�����ڂ�K��������
	/// </summary>
	virtual void Apply() {}

	/// <summary>
	/// �ݒ荀�ڂ��t�F�[�h�C��������
	/// </summary>
	void FadeIn();

	/// <summary>
	/// �ݒ荀�ڂ��t�F�[�h�A�E�g������
	/// </summary>
	void FadeOut();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �����\����ݒ肷��
	/// </summary>
	inline void SetIsAdjustable(bool value) { isAdjustable = value; }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �����\�����擾����
	/// </summary>
	/// <returns>�����\�Ȃ�true</returns>
	inline bool IsAdjustable() const { return isAdjustable; }

protected:
	std::list<UI_Slider*> sliders;		// �X���C�_�[�̎���
	UI_Define::SliderColor sliderColor;	// �X���C�_�[�̐F���
	FontInfo fontInfo;					// �t�H���g���
	EasingUtils::EasingInt alphaEasing;	// �s�����x

	bool isAdjustable;	// �����\��
};