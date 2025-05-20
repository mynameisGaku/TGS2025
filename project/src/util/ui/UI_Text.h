#pragma once
// ���p����
#include "src/util/ui/UI_Canvas.h"

// ���ėp
#include "src/util/Utils.h"

// �����o�E�@�\
#include "src/util/font/Font.h"

/// <summary>
/// �e�L�X�g�\�����s��UI�N���X
/// </summary>
/// <author> �~�b�` </author>
class UI_Text : public UI_Canvas {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// �e�L�X�g�I�u�W�F�N�g�𐶐�����
	/// </summary>
	/// <param name="text">�\��������������</param>
	/// <param name="trs">�`��ʒu</param>
	/// <param name="fontInfo">�t�H���g�̏��</param>
	/// <param name="scrollTotalTime">��������̕b��</param>
	/// <param name="scrollSound">��������̍ۂɗ������ʉ�</param>
	UI_Text(const std::string& text, const RectTransform& trs, const FontInfo& fontInfo, const float& scrollTotalTime = -1.0f, const std::string& scrollSE = "");

	~UI_Text();

	//================================================================================
	// ���e��֐�

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �������`�悷��
	/// </summary>
	void DrawTexts();

	/// <summary>
	/// �������菈��
	/// </summary>
	void ScrollTextProcess();

	/// <summary>
	/// �T�u�g�[���̐ݒ�������̏�Ԃɍ��킹�����̂ɓK��������
	/// </summary>
	void ApplySubtone();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �\�����镶�����ݒ肷��(���̑��̐ݒ�͕ύX���Ȃ�)
	/// </summary>
	void SetText(const std::string& text);

	/// <summary>
	/// �\�����镶����ƃt�H���g����ݒ肷��
	/// </summary>
	/// <param name="text">������</param>
	/// <param name="font">�t�H���g���</param>
	void SetText(const std::string& text, const FontInfo& font);

	// �����̐F��ύX����
	inline void SetTextColor(const int& color) { fontInfo.color = color; }

	// �X�N���[����ݒ肷��
	inline void SetScrollTime(const float& value) { scrollTime = scrollTotalTime = value; }

	// �X�N���[�����ɍĐ�������ʉ���ݒ肷��
	inline void SetScrollSE(const std::string& se) { scrollSE = se; }

	/// <summary>
	/// �T�u�g�[����ݒ肷��
	/// </summary>
	void SetSubtone(const UI_Subtone& sub) override;

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ��������擾����
	/// </summary>
	inline std::string Text() const { return text; }

	/// <summary>
	/// ���������擾����
	/// </summary>
	inline int Len() const { return static_cast<int>(text.length()); }

	/// <summary>
	/// �����̐F���擾����
	/// </summary>
	inline int Color() const { return fontInfo.color; }

	/// <summary>
	/// ��������̌��ʎ��Ԃ̊���
	/// </summary>
	inline float ScrollRate() const { return scrollTime / scrollTotalTime; }

	/// <summary>
	/// �`�悷�镶����̕����擾����
	/// </summary>
	inline Vector2 TextSize() const { return textSize; }

	/// <summary>
	/// �������肵�Ă��邩���擾����
	/// </summary>
	inline bool IsScroll() const { return scrollTime > 0.0f; }

	// �A���J�[�|�C���g�ɂ��Y���l���擾����
	Vector2 DisplacementByAnchorPoint() override;

private:
	//================================================================================
	// ��������֘A

	std::string text;	// ������
	Vector2 textSize;	// ������
	int length;			// ������

	FontInfo fontInfo;	// �t�H���g�̏��

	//================================================================================
	// ����������֘A

	std::vector<String::Letter> letters;	// ��������ꕶ�����ۑ�����
	std::string scrollSE;	// �������肷��ۂ̃T�E���h

	int afterScrollText;	// 1�t���[���O�̃X�N���[���ς̕�����
	float scrollTime;		// ��������X�N���[�����鎞��
	float scrollTotalTime;	// ��������X�N���[������S�̎���

};
