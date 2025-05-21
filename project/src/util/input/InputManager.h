#pragma once

#include "src/util/math/vector3.h"
#include <string>

#include "src/util/input/KeyDefine.h"

//==========================================================================================

/// <summary>
/// ���͂��Ǘ�����namespace
/// </summary>
namespace InputManager {

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// ���͏��̍X�V����
	/// </summary>
	void InputDataUpdate();

	/// <summary>
	/// ��s���͏��̍X�V����
	/// </summary>
	/// <param name="advancedEntry">��s���͂̕ێ�����</param>
	void AdvancedEntryUpdate(const int& padNumber, const float& advancedEntry);

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	//==========================================================================================
	// �����͏��̎擾

	/// <summary>
	/// �����ꂽ1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="keyCode">���͏��̎��</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Push(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// �����ꂽ1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="name">���z�{�^���̖��O</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Push(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ������Ă���Ԃ̂�true��Ԃ�
	/// </summary>
	/// <param name="keyCode">���͏��̎��</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Hold(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ������Ă���Ԃ̂�true��Ԃ�
	/// </summary>
	/// <param name="name">���z�{�^���̖��O</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Hold(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ������1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="keyCode">���͏��̎��</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Release(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ������1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="name">���z�{�^���̖��O</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Release(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// �����������ꂽ�ꍇ�Atrue��Ԃ�
	/// </summary>
	/// <param name="keyCodes">��������������͏��̎��</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	bool Simultaneously(const std::list<KeyDefine::KeyCode>& keyCodes, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ���z�{�^���ɓo�^����Ă�����͎�ނ��擾����
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	std::vector<KeyDefine::KeyCode> KeyList(const std::string& name);

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���߂̓��͏����擾����
	/// </summary>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	std::list<KeyDefine::InputData> LatestInput(const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ��s���͂���Ă�����͏����擾����
	/// </summary>
	bool AdvancedEntry(const KeyDefine::KeyCode& keyCode, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// ��s���͂���Ă�����͏����擾����
	/// </summary>
	bool AdvancedEntry(const std::string& name, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// �A�i���O�X�e�B�b�N�̌X�����擾����
	/// </summary>
	Vector3 AnalogStick(int padNumber = DX_INPUT_PAD1);

#ifdef _DEBUG

	//==========================================================================================
	// ���f�o�b�O

	/// <summary>
	/// �f�o�b�O�p�ɓ��͏���`�悷��
	/// </summary>
	/// <param name="x">�`����W(X��)</param>
	/// <param name="y">�`����W(Y��)</param>
	void DrawTest(const float& x, const float& y);

#endif // _DEBUG
};
