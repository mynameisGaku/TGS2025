#pragma once

// ���ėp
#include <Dxlib.h>
#include <unordered_map>

namespace KeyDefine {

	/// <summary>
	/// ���͂̎�� 
	/// </summary>
	enum class KeyCode {
		None = -1,
		Up,				// �f�W�^�������{�^����
		Down,			// �f�W�^�������{�^����
		Left,			// �f�W�^�������{�^����
		Right,			// �f�W�^�������{�^���E
		Start,			// START�{�^��
		Back,			// BACK�{�^��
		LeftThumb,		// ���X�e�B�b�N��������
		RightThumb,		// �E�X�e�B�b�N��������
		LeftShoulder,	// LB�{�^��
		RightShoulder,	// RB�{�^��
		ButtonA,		// A�{�^��
		ButtonB,		// B�{�^��
		ButtonX,		// X�{�^��
		ButtonY,		// Y�{�^��

		A,				// A�L�[
		B,				// B�L�[
		C,				// C�L�[
		D,				// D�L�[
		E,				// E�L�[
		F,				// F�L�[
		G,				// G�L�[
		H,				// H�L�[
		I,				// I�L�[
		J,				// J�L�[
		K,				// K�L�[
		L,				// L�L�[
		M,				// M�L�[
		N,				// N�L�[
		O,				// O�L�[
		P,				// P�L�[
		Q,				// Q�L�[
		R,				// R�L�[
		S,				// S�L�[
		T,				// T�L�[
		U,				// U�L�[
		V,				// V�L�[
		W,				// W�L�[
		X,				// X�L�[
		Y,				// Y�L�[
		Z,				// Z�L�[

		Alpha0,			// 0�L�[
		Alpha1,			// 1�L�[
		Alpha2,			// 2�L�[
		Alpha3,			// 3�L�[
		Alpha4,			// 4�L�[
		Alpha5,			// 5�L�[
		Alpha6,			// 6�L�[
		Alpha7,			// 7�L�[
		Alpha8,			// 8�L�[
		Alpha9,			// 9�L�[

		F1,				// F1�L�[
		F2,				// F2�L�[
		F3,				// F3�L�[
		F4,				// F4�L�[
		F5,				// F5�L�[
		F6,				// F6�L�[
		F7,				// F7�L�[
		F8,				// F8�L�[
		F9,				// F9�L�[
		F10,			// F10�L�[
		F11,			// F11�L�[
		F12,			// F12�L�[

		BackSpace,		// BackSpace�L�[
		Tab,			// Tab�L�[
		Enter,			// Enter�L�[

		LeftShift,		// ��Shift�L�[
		RightShift,		// �EShift�L�[
		LeftControl,	// ��Ctrl�L�[
		RightControl,	// �ECtrl�L�[
		Escape,			// Esc�L�[
		Space,			// �X�y�[�X�L�[
		PageUp,			// PageUp�L�[
		PageDown,		// PageDown�L�[
		End,			// End�L�[
		Home,			// Home�L�[
		UpArrow,		// ��L�[
		DownArrow,		// ���L�[
		LeftArrow,		// ���L�[
		RightArrow,		// �E�L�[
		Insert,			// Insert�L�[
		Delete,			// Delete�L�[

		LeftClick,		// ���N���b�N
		RightClick,		// �E�N���b�N
		MiddleClick,	// �z�C�[���N���b�N

		MouseButton1,	// �}�E�X�P�{�^��
		MouseButton2,	// �}�E�X�Q�{�^��
		MouseButton3,	// �}�E�X�R�{�^��
		MouseButton4,	// �}�E�X�S�{�^��
		MouseButton5,	// �}�E�X�T�{�^��
	};

	/// <summary>
	/// ���͏�
	/// </summary>
	enum class TouchPhase {
		None = -1,
		Begin,		// �������u��
		Moved,		// �ړ���
		Stationary,	// �����Ă��邪�ړ����Ă��Ȃ�
		Ended,		// �����ꂽ�u��
		Canceled,	// �L�����Z��
	};

	/// <summary>
	/// ���͂���f�o�C�X�̎��
	/// </summary>
	enum class DeviceType {
		None = -1,
		Key,		// �L�[�{�[�h
		Pad,		// �p�b�h
		Mouse,		// �}�E�X
	};

	/// <summary>
	/// ���͏��
	/// </summary>
	struct KeyInfo {
		int* value;				// ���͂̒l
		KeyCode keyCode;		// ���͂̎��
		DeviceType deviceType;	// ���͂���f�o�C�X�̎��
	};

	//============================================================

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	void SetKeyInputValue();
	void SetPadInputValue();
	void SetMouseInputValue();

	/// <summary>
	/// ���͎�ނ�����̓f�o�C�X���擾����
	/// </summary>
	/// <param name="key">���͎��</param>
	/// <returns>���̓f�o�C�X</returns>
	DeviceType KeyCodeToDeviceType(KeyCode key);

	/// <summary>
	/// ���̓f�o�C�X������͎�ނ��擾����
	/// </summary>
	/// <param name="type">���̓f�o�C�X</param>
	/// <returns>���͎��</returns>
	std::list<KeyInfo> DeviceTypeToKeyInfo(DeviceType type);

	/// <summary>
	/// �S�Ă̓��͏����擾����
	/// </summary>
	std::unordered_map<KeyCode, KeyInfo> GetKeyAll();
};