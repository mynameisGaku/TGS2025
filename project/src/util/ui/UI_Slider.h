#pragma once
#include "src/util/ui/UI_Canvas.h"

// ���ʂŕK�v�ȕ�
#include "src/util/font/Font.h"
#include "src/util/input/KeyDefine.h"

class UI_Text;

/// <summary>
/// �X���C�_�[�̏������s���N���X
/// </summary>
/// <author> �~�b�` </author>
class UI_Slider : public UI_Canvas {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// �X���C�_�[�̐�������
	/// </summary>
	/// <param name="trs">�`��ʒu</param>
	/// <param name="value">�ϓ�������l�̃A�h���X</param>
	/// <param name="minValue">�ŏ��l</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="backSize">�X���C�_�[�̑傫��</param>
	/// <param name="color">�X���C�_�[�Ɋւ���F���</param>
	/// <param name="format">�����w��q</param>
	/// <param name="label">���g�̖��O</param>
	UI_Slider(const std::string& label, const RectTransform& trs, float* value, const float& minValue, const float& maxValue, const Vector2& size, const UI_Define::SliderColor& color, const std::string& format = "%.1f", const FontInfo& fontInfo = FontInfo());
	
	~UI_Slider();

	//==========================================================================================
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
	/// �w�i�̕`�揈��
	/// </summary>
	void DrawBack();

	/// <summary>
	/// ���������̕`��
	/// </summary>
	void DrawDivisions();

	/// <summary>
	/// �܂݂̕`��
	/// </summary>
	void DrawPoint();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �����w��q��ݒ肷��
	/// </summary>
	/// <param name="format">�����w��q</param>
	/// <param name="font">�t�H���g���</param>
	void SetFormatStr(const std::string& format, const FontInfo& font);
	
	/// <summary>
	/// �X���C�_�[�̕\������ݒ肷��
	/// </summary>
	/// <param name="label">�X���C�_�[�̕\����</param>
	/// <param name="font">�t�H���g���</param>
	void SetLabelText(const std::string& label, const FontInfo& font);

	inline void SetValue(float* setValue) { value = setValue; }
	inline void SetMaxValue(const float& setMaxValue) { maxValue = max(setMaxValue, minValue + 1); }
	inline void SetMinValue(const float& setMinValue) { minValue = min(setMinValue, maxValue + 1); }
	
	inline void SetPointEnabled(const bool& enabled) { pointEnabled = enabled; }
	inline void SetPointSize(const Vector2& size) { pointSize = size; }
	inline void SetBackSize(const Vector2& size) { backSize = size; }

	inline void SetBackColor(const int& color) { colors.backColor = color; }
	inline void SetGaugeColor(const int& color) { colors.gaugeColor = color; }
	inline void SetPointColor(const int& color) { colors.pointColor = color; }

	//==========================================================================================
	// ���Q�b�^�[

	inline Vector2 ScreenPos() const { return rectTransform->Global().position - backSize; }

	inline const float Value() const { return *value; }
	inline float MaxValue() const { return maxValue; }
	inline float MinValue() const { return minValue; }

	inline bool PointEnabled() const { return pointEnabled; }
	inline Vector2 PointSize() const { return pointSize; }
	inline Vector2 BackSize() const { return backSize; }

	inline UI_Define::SliderColor Colors() const { return colors; }

private:
	//==========================================================================================
	// �������o�ϐ�

	std::string label;	// �\�����閼�O

	float* value;	// ���݂̒l
	float maxValue;	// �ő�l
	float minValue;	// �ŏ��l

	UI_Define::SliderColor colors;	// �F���

	int divisions;	// ������

	bool pointEnabled;	// �܂݂̑��삪�L����
	bool isPointMove;	// �܂݂������Ă��邩
	bool front;

	Vector2 pointSize;	// �܂݂̑傫��
	Vector2 backSize;	// �X���C�_�[�̑傫��

	Vector2 pointPosition;	// �܂݂̍��W

	std::string format;		// �����w��q
	UI_Text* formatText;	// ���l�̕\���e�L�X�g
	UI_Text* labelText;		// ���g�̖��O
	FontInfo fontInfo;		// �t�H���g���

	KeyDefine::TouchPhase touchPhase;

	/// <summary>
	/// �܂݂��N���b�N��������
	/// </summary>
	void TouchBeginUpdate();

	/// <summary>
	/// �܂݂𓮂�������
	/// </summary>
	void TouchMovedUpdate();

	/// <summary>
	/// �܂݂��痣��������
	/// </summary>
	void TouchEndedUpdate();
};
