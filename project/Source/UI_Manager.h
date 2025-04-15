#pragma once

// ���ėp
#include <vector>

// ���ʂŕK�v�ȕ�
#include "UI_Base.h"
#include "UI_Canvas.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Gauge.h"
#include "UI_Slider.h"
#include "UI_BillBoard.h"

/// <summary>
/// UI�̏����Ǘ�����
/// </summary>
/// <author> �~�b�` </author>
namespace UI_Manager {

	//================================================================================
	// ���e��֐�

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �Ǘ����Ă���UI���X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �Ǘ����Ă���UI��`�悷��(�w��)
	/// </summary>
	void DrawBack();

	/// <summary>
	/// �Ǘ����Ă���UI��`�悷��(����)
	/// </summary>
	void DrawFront();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// �Ǘ�����UI��ǉ�����
	/// </summary>
	/// <param name="uiData">UI���</param>
	/// <returns>�ǉ�����UI���̃C���X�^���X</returns>
	UI_Canvas* Add(UI_Canvas* uiData, const bool& front = false);

	/// <summary>
	/// �Ǘ����Ă���UI��؂藣��
	/// </summary>
	/// <param name="uiData">UI���</param>
	/// <returns>�؂藣�����ꍇ:True</returns>
	bool Detach(UI_Canvas* uiData);

	/// <summary>
	/// �Ǘ����Ă���UI���������Ď擾����
	/// </summary>
	/// <param name="tag">��������^�O</param>
	/// <returns>���݂���΂��̎��Ԃ�Ԃ��B�������nullptr��Ԃ��B</returns>
	UI_Canvas* Find(const std::string& tag);

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �@�\�����邩��ݒ肷��
	/// </summary>
	void SetIsActive(const bool& value);

	/// <summary>
	/// �X�V���邩��ݒ肷��
	/// </summary>
	void SetIsUpdate(const bool& value);

	/// <summary>
	/// �`�悷�邩��ݒ肷��
	/// </summary>
	void SetIsDraw(const bool& value);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �@�\�����邩���擾����
	/// </summary>
	bool IsActive();

	/// <summary>
	/// �X�V���邩���擾����
	/// </summary>
	bool IsUpdate();

	/// <summary>
	/// �`�悷�邩���擾����
	/// </summary>
	bool IsDraw();

	/// <summary>
	/// �`��D��x�ɉ��������ёւ����s��
	/// </summary>
	void Sort();

	/// <summary>
	/// �`��D��x�ɉ��������ёւ����s������
	/// </summary>
	void UI_Canvas_CombSort(std::vector<UI_Canvas*>& ui);
};
