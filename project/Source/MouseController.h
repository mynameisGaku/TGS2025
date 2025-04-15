#pragma once

#include <string>
#include "KeyDefine.h"
#include "Util/Vector2.h"

//==========================================================================================
// ����`�l

	static const Vector2 MOUSE_SENSI_MAX = Vector2(10.0f);	// �ő�}�E�X���x

	/// <summary>
	/// �}�E�X�̈ړ����@
	/// </summary>
	enum class MouseMovement {
		Free = 0,	// ���R�ɑ���\
		Fixed,		// ��ʒ����ŌŒ�
		OnScreen,	// ��ʓ��ɗ��߂�
	};

	/// <summary>
	/// �}�E�X�̏��
	/// </summary>
	struct MouseInfo {
		Vector2 position;		// �}�E�X�̍��W(Vector2�^)
		int newX, newY;			// �}�E�X�̍�����ʒu���擾����
		int oldX, oldY;			// �}�E�X���������ʒu���擾����
		int moveX, moveY;		// New - Old �ňړ������������擾����
		int wheelRot;			// �}�E�X�z�C�[���̉�]��
		Vector2 sensitivity;	// �}�E�X���x
		MouseMovement movement;	// �}�E�X�̈ړ����@
		POINT point;			// �}�E�X�̏��
		HWND hWnd;				// �E�B���h�E�̏��

		MouseInfo() :
			position(V2::ZERO),
			newX(0), newY(0),
			oldX(0), oldY(0),
			moveX(0), moveY(0),
			wheelRot(GetMouseWheelRotVol()),
			sensitivity(0.0f),
			movement(MouseMovement::Free),
			point({ 0 }),
			hWnd(0)
		{
		};

		/// <summary>
		/// ���x�ݒ荞�݂̈ړ���
		/// </summary>
		inline Vector2 Move() const { return Vector2(moveX * (sensitivity.x * 0.1f), moveY * (sensitivity.y * 0.1f)); }
	};


//==========================================================================================

namespace MouseController {

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

	void PushStatusUpdate();

	/// <summary>
	/// �}�E�X���N���b�N�����u�Ԃ����m����
	/// </summary>
	/// <param name="_keyInput">�L�[�{�[�h�̓��͏��</param>
	/// <returns>�N���b�N���Ă�����:True</returns>
	bool CheckClick(KeyDefine::KeyCode keyCode);

	/// <summary>
	/// �}�E�X�N���b�N�𒷉������Ă��邩�����m����
	/// </summary>
	/// <param name="_keyInput">�L�[�{�[�h�̓��͏��</param>
	/// <returns>�N���b�N���Ă�����:True</returns>
	bool CheckHold(KeyDefine::KeyCode keyCode);

	/// <summary>
	/// �}�E�X�̃N���b�N���������ꂽ�����m����
	/// </summary>
	/// <param name="_keyInput">�L�[�{�[�h�̓��͏��</param>
	/// <returns>�N���b�N�𗣂����u��:True</returns>
	bool CheckRelease(KeyDefine::KeyCode keyCode);

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �}�E�X�̓�����ݒ肷��
	/// </summary>
	void SetMouseMovement(MouseMovement move);

	/// <summary>
	/// �}�E�X���x��ݒ肷��
	/// </summary>
	void SetMouseSensitivity(Vector2 value);

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �}�E�X�̏����擾����
	/// </summary>
	const MouseInfo Info();

	//==========================================================================================
	// ������

	/// <summary>
	/// �}�E�X�Ƃ̓����蔻��(�~)
	/// </summary>
	/// <param name="pos">�������肽�����W(���S)</param>
	/// <param name="r">���a</param>
	/// <returns>�������Ă�����:True</returns>
	bool ColCheck_CursorToCircle(Vector2 pos, float r);

	/// <summary>
	/// �}�E�X�Ƃ̓����蔻��(�l�p)
	/// </summary>
	/// <param name="pos">�������肽�����W(����)</param>
	/// <param name="size">�T�C�Y(�E��)</param>
	/// <returns>�������Ă�����:True</returns>
	bool ColCheck_CursorToBox(Vector2 pos, Vector2 size);
}