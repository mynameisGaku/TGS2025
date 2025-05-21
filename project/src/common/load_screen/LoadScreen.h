#pragma once
#include "framework/gameObject.h"

// ���ėp
#include "src/util/Utils.h"
#include "src/util/easing/Easing.h"
#include "src/util/font/Font.h"
#include "src/util/math/Vector2.h"

namespace {

	static const Vector2 DRAW_TEXT_POS = Vector2(4.0f, Screen::HEIGHT_F - 64.0f);	// �����̕\���ʒu
	static const int TEXT_COLOR = GetColor(255, 255, 255);

	static const Vector2 GAUGE_POS = Vector2(4.0f, Screen::HEIGHT_F - 24.0f);		// �Q�[�W�̕\���ʒu
	static const Vector2 GAUGE_SIZE = Vector2(Screen::WIDTH_HALF - 4.0f, 20.0f);	// �Q�[�W�̕\���T�C�Y
	static const int GAUGE_COLOR = GetColor(220, 0, 220);	// �Q�[�W�̐F
}

/// <summary>
/// ���[�f�B���O��ʂ̕\�����s��
/// </summary>
class LoadScreen : public GameObject {
public:
	//================================================================================
	// ����`�l

	/// <summary>
	/// �w�i�摜�̃X�N���[�����@
	/// </summary>
	enum class ScrollType {
		stNone =-1,	// ��
		stUp,		// ��X�N���[��
		stDown,		// ���X�N���[��
		stLeft,		// ���X�N���[��
		stRight,	// �E�X�N���[��
		stMax
	};

	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	LoadScreen();
	~LoadScreen();

	//================================================================================
	// ���e��֐�

	void Update() override;
	void Draw() override;

	/// <summary>
	/// �w�i�摜�̕`��
	/// </summary>
	void DrawBackGround();

	/// <summary>
	/// ���[�h���ɕ\�����镶���̕`��
	/// </summary>
	void DrawLoadText();

	/// <summary>
	/// ���[�h�Q�[�W�̕`��
	/// </summary>
	void DrawLoadGauge();

	/// <summary>
	/// �{�^���q���g�̕`��
	/// </summary>
	void DrawButtonTip();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���[�h�̊�����ݒ肷��
	/// </summary>
	/// <param name="_rate">0.0f�`1.0f�̊�</param>
	void SetRate(const float& rate);

	/// <summary>
	/// �w�i�摜��ݒ肷��
	/// </summary>
	/// <param name="filename">�w�i�摜�̃t�@�C���p�X</param>
	/// <param name="scroll">�w�i�摜���X�N���[�����邩</param>
	void SetBackGround(const std::string& filename, const ScrollType& scroll = ScrollType::stNone);

	/// <summary>
	/// �{�^�������Ńt�F�[�h�A�E�g�ֈڍs���邩��ݒ肷��
	/// </summary>
	inline void SetIsPushFadeOut(const bool& value) { isPushFadeOut = value; }

	/// <summary>
	/// �t�F�[�h�C������
	/// </summary>
	/// <param name="sec">���ʎ���(�b)</param>
	/// <param name="easingType">�C�[�W���O�̎��</param>
	inline void FadeIn(const float& sec, const EasingType& easingType = EasingType::Linear) { fadeEasing.SetEasing(0.0f, 255.0f, sec, easingType, true); }

	/// <summary>
	/// �t�F�[�h�A�E�g����
	/// </summary>
	/// <param name="sec">���ʎ���(�b)</param>
	/// <param name="easingType">�C�[�W���O�̎��</param>
	inline void FadeOut(const float& sec, const EasingType& easingType = EasingType::Linear) { fadeEasing.SetEasing(255.0f, 0.0f, sec, easingType, true); }

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���[�h���I��������
	/// </summary>
	/// <returns>���[�h���I�������Ȃ�true</returns>
	bool IsLoadEnd() const;

	/// <summary>
	/// �t�F�[�h����
	/// </summary>
	/// <returns>�t�F�[�h���Ȃ�true</returns>
	inline bool IsFade() const { return fadeEasing.info.isActive; }

private:
	EasingUtils::EasingFloat fadeEasing;	// �t�F�[�h�p�C�[�W���O���
	float rate;				// ���[�h����
	float textWidth;		// �\�������̉���
	std::string loadText;	// ���[�h���ɕ\�����镶��

	int hBackground;	// �w�i�摜
	bool isPushFadeOut;	// �{�^�������Ńt�F�[�h�A�E�g�ֈڍs����

	Vector2 bgPos;	// �w�i�摜�̕`����W
	ScrollType scrollType;	// �w�i�摜�̃X�N���[�����@

	FontInfo useFont;	// �g�p����t�H���g���

	const std::string LOADING_TEXT = "Now Loading... %3.0f";// ���[�h���ɕ\�����镶��
	const std::string LOAD_END_TEXT = "Load Complete!";		// ���[�h�I�����ɕ\�����镶��
	const std::string BUTTON_TIP = "�{�^���������Ď��֐i��";// �{�^���q���g
};