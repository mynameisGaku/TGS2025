#pragma once

#include <string>
#include "src/util/input/KeyDefine.h"
#include "src/util/math/vector2.h"

namespace MouseController {

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
		KeyDefine::MouseMovement movement;	// �}�E�X�̈ړ����@
		POINT point;			// �}�E�X�̏��
		HWND hWnd;				// �E�B���h�E�̏��

		MouseInfo() :
			position(Vector2::Zero),
			newX(0), newY(0),
			oldX(0), oldY(0),
			moveX(0), moveY(0),
			wheelRot(GetMouseWheelRotVol()),
			sensitivity(Vector2::Ones),
			movement(KeyDefine::MouseMovement::Free),
			point({ 0 }),
			hWnd(0)
		{
		};

		/// <summary>
		/// ���x�ݒ荞�݂̈ړ���
		/// </summary>
		inline Vector2 Move() const { return Vector2(moveX * (sensitivity.x * 0.1f), moveY * (sensitivity.y * 0.1f)); }
	};

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
	void SetMouseMovement(KeyDefine::MouseMovement move);

	/// <summary>
	/// �}�E�X���x��ݒ肷��
	/// </summary>
	void SetMouseSensitivity(Vector2 value);

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �}�E�X�̏����擾����
	/// </summary>
	const MouseController::MouseInfo Info();

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