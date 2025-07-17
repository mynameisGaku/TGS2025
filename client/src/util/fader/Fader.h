#pragma once
#include "framework/gameObject.h"
#include "framework/myDxLib.h"
#include "src/util/time/GameTime.h"
#include "src/util/easing/Easing.h"

/// <summary>
/// �t�F�[�h���Ǘ�����
/// </summary>
namespace Fader {

	//================================================================================
	// ���e��֐�

	void Init();
	void Update();
	void Draw();
	void Release();

	//================================================================================
	// ���t�F�[�h�֘A

	/// <summary>
	/// �t�F�[�h���J�n����
	/// </summary>
	/// <param name="totalTime">�t�F�[�h����</param>
	/// <param name="easingType">��Ԏ��</param>
	/// <param name="begin">��ԑO�̕s�����x</param>
	/// <param name="end">��Ԍ�̕s�����x</param>
	/// <param name="rate">���Ԍo�ߗ���K�������邩</param>
	void FadeStart(float totalTime, EasingType easingType, float begin, float end, GameTime::AdditionMethod timeAddition);

	/// <summary>
	/// �t�F�[�h�C������
	/// </summary>
	/// <param name="time">�t�F�[�h����</param>
	/// <param name="easingType">��Ԏ��</param>
	/// <param name="rate">���Ԍo�ߗ���K�������邩</param>
	inline void FadeIn(float time, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { FadeStart(time, easingType, 255.0f, 0.0f, timeAddition); }

	/// <summary>
	/// �t�F�[�h�A�E�g����
	/// </summary>
	/// <param name="time">�t�F�[�h����</param>
	/// <param name="easingType">��Ԏ��</param>
	/// <param name="rate">���Ԍo�ߗ���K�������邩</param>
	inline void FadeOut(float time, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { FadeStart(time, easingType, 0.0f, 255.0f, timeAddition); }

	//================================================================================
	// �����^�[�{�b�N�X�֘A

	/// <summary>
	/// ���^�[�{�b�N�X�̕`����J�n����
	/// </summary>
	/// <param name="totalTime">�\������܂ł̎���</param>
	/// <param name="size">���̑傫��</param>
	/// <param name="easingType">��Ԏ��</param>
	/// <param name="rate">���Ԍo�ߗ���K�������邩</param>
	/// <param name="fadeIn">�t�F�[�h���@�BTrue�Ȃ�C���AFalse�Ȃ�A�E�g</param>
	void LetterBoxStart(float totalTime, float size, EasingType easingType, GameTime::AdditionMethod timeAddition, bool fadeIn);

	/// <summary>
	/// ���^�[�{�b�N�X�̕`����I������
	/// </summary>
	/// <param name="totalTime">�\������܂ł̎���</param>
	/// <param name="size">���̑傫��</param>
	/// <param name="easingType">��Ԏ��</param>
	/// <param name="rate">���Ԍo�ߗ���K�������邩</param>
	inline void LetterBoxFadeIn(float totalTime, float size, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { LetterBoxStart(totalTime, size, easingType, timeAddition, true); }
	inline void LetterBoxFadeOut(float totalTime, float size, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { LetterBoxStart(totalTime, size, easingType, timeAddition, false); }

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �t�F�[�h���Đ�����
	/// </summary>
	/// <returns>�Đ����̏ꍇ��True</returns>
	bool IsPlaying();

	/// <summary>
	/// ���^�[�{�b�N�X�̃t�F�[�h���Đ�����
	/// </summary>
	/// <returns>�Đ����̏ꍇ��True</returns>
	bool IsPlayingLetterBox();

	/// <summary>
	/// �t�F�[�h���\������
	/// </summary>
	/// <returns>�\�����̏ꍇ��True</returns>
	bool IsDisplay();
};
