#pragma once

// ���ėp
#include <string>
#include "../Library/time.h"

// ���ʂŕK�v�ȕ�
#include "KeyDefine.h"

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

	/// <summary>
	/// �p�b�h�̐U����ݒ肷��
	/// </summary>
	/// <param name="padNumber">�U��������p�b�h�̎��ʎq</param>
	/// <param name="power">�U���̋���(0�`1000)</param>
	/// <param name="time">���ʎ���(-1�̏ꍇ�AStopVibration()���Ă΂��܂ł̊ԐU������)</param>
	/// <param name="effectIndex">�U�������郂�[�^�[�̔ԍ�</param>
	inline void SetVibration(int padNumber, int power, float time, int effectIndex = -1) {
		StartJoypadVibration(padNumber, power, static_cast<int>(Time::SecToFrame(time)), effectIndex);
	}

	/// <summary>
	/// �p�b�h�̐U�����~����
	/// </summary>
	/// <param name="padNumber">�U��������p�b�h�̎��ʎq</param>
	/// <param name="effectIndex">�U�������郂�[�^�[�̔ԍ�</param>
	inline void StopVibration(int padNumber, int effectIndex = -1) {
		StopJoypadVibration(padNumber, effectIndex);
	}

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