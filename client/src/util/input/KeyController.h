#pragma once

// ���ėp
#include <string>

// ���ʂŕK�v�ȕ�
#include "src/util/input/KeyDefine.h"

namespace KeyController {

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
	/// ���݂̃L�[�{�[�h���͂��m�F
	/// </summary>
	/// <param name="_keyInput">�L�[�{�[�h�̓��͏��</param>
	/// <param name="keyCode">���͂̎��</param>
	/// <returns>������Ă�����:True</returns>
	bool CheckPushStatusCurrent(KeyDefine::KeyCode keyCode);

	/// <summary>
	/// 1�t���[���O�̃L�[�{�[�h���͂��m�F
	/// </summary>
	/// <param name="_keyInput">�L�[�{�[�h�̓��͏��</param>
	/// <param name="keyCode">���͂̎��</param>
	/// <returns>������Ă�����:True</returns>
	bool CheckPushStatusBefore(KeyDefine::KeyCode keyCode);
}