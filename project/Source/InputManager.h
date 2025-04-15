#pragma once

#include "Util/Vector3.h"
#include <string>

#include "KeyDefine.h"

using namespace KeyDefine;

//==========================================================================================
// ����`�l

namespace {

	static constexpr float HOLD_TIME = 0.25f;				// �{�^���̒������𔻒肷�鎞��
	static constexpr float ADVANCED_ENTRY_TIME = 0.2f;		// ��s���͂̕ێ�����

	/// <summary>
	/// ���͏��
	/// </summary>
	struct InputData {
		KeyDefine::KeyInfo keyInfo;	// ���͏��
		float putTime;				// ������Ă���b��
		bool isPut;					// ������Ă��邩

		InputData() : InputData(KeyDefine::KeyInfo(), 0.0f, false) {};

		InputData(KeyDefine::KeyInfo info, float time, bool put) :
			keyInfo(info),
			putTime(time),
			isPut(put)
		{
		};
	};

	/// <summary>
	/// ��s���͏��
	/// </summary>
	struct AdvancedEntryInfo {
		KeyDefine::KeyInfo keyInfo;	// ���͏��
		float saveTime;				// �ێ�����b��

		AdvancedEntryInfo() : AdvancedEntryInfo(KeyDefine::KeyInfo(), 0.0f) {};

		AdvancedEntryInfo(KeyDefine::KeyInfo info, float time) :
			keyInfo(info),
			saveTime(time)
		{
		};
	};
}

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
	/// �������
	/// </summary>
	void Release();

	//==========================================================================================
	// �����͏��̎擾

	/// <summary>
	/// �����ꂽ1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="keyCode">���͏��̎��</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Push(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// �����ꂽ1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="name">���z�{�^���̖��O</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Push(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// ������Ă���Ԃ̂�true��Ԃ�
	/// </summary>
	/// <param name="keyCode">���͏��̎��</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Hold(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// ������Ă���Ԃ̂�true��Ԃ�
	/// </summary>
	/// <param name="name">���z�{�^���̖��O</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Hold(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// ������1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="keyCode">���͏��̎��</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Release(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// ������1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="name">���z�{�^���̖��O</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Release(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// �����������ꂽ�ꍇ�Atrue��Ԃ�
	/// </summary>
	/// <param name="keyCodes">��������������͏��̎��</param>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	bool Simultaneously(std::list<KeyDefine::KeyCode> keyCodes, int num = DX_INPUT_PAD1);

	/// <summary>
	/// ���z�{�^���ɓo�^����Ă�����͎�ނ��擾����
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	std::vector<KeyDefine::KeyCode> KeyList(std::string name);

	//==========================================================================================
	// �����͏��̊m�F

	/// <summary>
	/// ���߂̓��͏����X�V����
	/// </summary>
	/// <param name="advancedEntry">��s���͂̕ێ�����</param>
	void AdvancedEntryUpdate(int num, float advancedEntry);

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���߂̓��͏����擾����
	/// </summary>
	/// <param name="num">�p�b�h�̔ԍ�</param>
	std::list<KeyDefine::KeyInfo> LatestInput(int num = DX_INPUT_PAD1);

	/// <summary>
	/// ��s���͂���Ă�����͏����擾����
	/// </summary>
	bool AdvancedEntry(KeyDefine::KeyCode keyCode, int num = DX_INPUT_PAD1);

	/// <summary>
	/// ��s���͂���Ă�����͏����擾����
	/// </summary>
	bool AdvancedEntry(std::string name, int num = DX_INPUT_PAD1);

	/// <summary>
	/// �A�i���O�X�e�B�b�N�̌X�����擾����
	/// </summary>
	Vector3 AnalogStick();

	//==========================================================================================
	// ���f�o�b�O
	
	/// <summary>
	/// �f�o�b�O�p�ɓ��͏���`�悷��
	/// </summary>
	/// <param name="x">�`����W(X��)</param>
	/// <param name="y">�`����W(Y��)</param>
	void DrawTest(float x, float y);

};
