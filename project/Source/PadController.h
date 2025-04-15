#pragma once

// ���ėp
#include <string>
#include "Util/Vector2.h"

// ���ʂŕK�v�ȕ�
#include "KeyDefine.h"

//==========================================================================================
// ����`�l

namespace {

	static constexpr int PAD_NUMBER_MAX = 2;		// �p�b�h�̍ő�ڑ���
	static constexpr float STICK_DEADZONE = 0.2f;	// �X�e�B�b�N�̃f�b�h�]�[����`
	static constexpr float TRIGGER_DEADZONE = 0.2f;	// �g���K�[�̃f�b�h�]�[����`

	static const Vector2 STICK_SENSI_MAX = Vector2(10.0f);	// �ő�X�e�B�b�N���x
}

//==========================================================================================

namespace PadController {

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	//==========================================================================================
	// �����͏��̎擾

	/// <summary>
	/// ���݂̃p�b�h���͂��m�F
	/// </summary>
	/// <param name="keyCode">���͂̎��</param>
	/// <returns>������Ă�����:True</returns>
	bool CheckPushStatusCurrent(KeyDefine::KeyCode keyCode, int padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// 1�t���[���O�̃p�b�h���͂��m�F
	/// </summary>
	/// <param name="keyCode">���͂̎��</param>
	/// <returns>������Ă�����:True</returns>
	bool CheckPushStatusBefore(KeyDefine::KeyCode keyCode, int padNumber = DX_INPUT_PAD1);

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �X�e�B�b�N���x��ݒ肷��
	/// </summary>
	void SetStickSensitivity(Vector2 value);

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �X�e�B�b�N���x���擾����
	/// </summary>
	Vector2 StickSensitivity();

	/// <summary>
	/// ���X�e�B�b�N�̓��͒l
	/// </summary>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	/// <returns>1����-1�̊Ԃ̒l</returns>
	Vector2 NormalizedLeftStick(int num = DX_INPUT_PAD1);

	/// <summary>
	/// �E�X�e�B�b�N�̓��͒l
	/// </summary>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	/// <returns>1����-1�̊Ԃ̒l</returns>
	Vector2 NormalizedRightStick(int num = DX_INPUT_PAD1);

	/// <summary>
	/// ���g���K�[�̓��͒l
	/// </summary>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	/// <returns>1����0�̊Ԃ̒l</returns>
	float NormalizedLeftTrigger(int num = DX_INPUT_PAD1);

	/// <summary>
	/// �E�g���K�[�̓��͒l
	/// </summary>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	/// <returns>1����0�̊Ԃ̒l</returns>
	float NormalizedRightTrigger(int num = DX_INPUT_PAD1);

	/// <summary>
	/// �p�b�h�̓��͏���Ԃ�
	/// </summary>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	/// <returns>���͏��</returns>
	XINPUT_STATE Input(int num = DX_INPUT_PAD1);

	/// <summary>
	/// �����̃p�b�h�ԍ���0�ȏォ�A�ڑ�����Ă���p�b�h�̐��ȉ��̎��ATrue��Ԃ�
	/// </summary>
	inline const bool CheckPadNumber(int num) { return (0 < num) && (num <= GetJoypadNum()); }
}