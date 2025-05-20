#pragma once
#include "../Library/myDxLib.h"
#include "../Source/Util/Vector2.h"
#include <vector>

/// <summary>
/// ���[�V�����u���[���Đ�����
/// </summary>
namespace BlurScreen {

	//==========================================================================================
	// ���e��֐�

	// ������
	void Init(float _alpha);

	// �ď�����
	void ReplayInit(float _duration, float fadeDuration = 0.25f);

	void Update();
	void Draw();

	// �u���[�p�X�N���[�����폜����
	void Release();

	void Reset();

	// �����_�����O�O�̃X�N���[���̕`��
	void PreRenderBlurScreen();

	// �����_�����O��̃X�N���[���̕`��
	void PostRenderBlurScreen();

	// �u���[���t�F�[�h�������s���āA�I������
	void FadeOut(float duration);

	//==========================================================================================
	// ���Q�b�^�[

	// �u���[�p�X�N���[���̏����擾����
	int ScreenInfo(int i);

	// �u���[�̌��ʎ��Ԃ��擾����
	float Duration();

	// �t�F�[�h�������s����
	bool IsFadeOut();

	// �u���[���g�p���Ă��邩
	bool IsUse();

};
