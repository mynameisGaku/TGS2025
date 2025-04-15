#pragma once
// ���p����
#include "UI_Canvas.h"

#include "Font.h"
#include "gauge.h"

class UI_Text;

/// <summary>
/// �Q�[�W�̏������s���N���X
/// </summary>
class UI_Gauge : public UI_Canvas {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// �Q�[�W�̐���������
	/// </summary>
	/// <param name="trs">�`��ʒu</param>
	/// <param name="value">���ݒl�̃A�h���X</param>
	/// <param name="minValue">�ŏ��l</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="gaugeSize">�Q�[�W�̑傫��</param>
	/// <param name="gaugeColor">�Q�[�W�̐F</param>
	/// <param name="delayColor">�������̐F</param>
	/// <param name="increaseColor">�������̐F</param>
	/// <param name="backColor">�w�i�F</param>
	/// <param name="gaugeThickness">�Q�[�W�̉��̑���</param>
	/// <param name="gaugeLateSpeed">�Q�[�W���x��Ă��鑬�x</param>
	UI_Gauge(const RectTransform& trs, float* value, const float& minValue, const float& maxValue, const Vector2& gaugeSize, const UI_Define::SliderColor& color, const float& gaugeThickness = 1.0f, const float& gaugeLateSpeed = 0.04f, const int& innerEdgeColor = GetColor(0, 0, 0), const int& outerEdgeColor = GetColor(200, 200, 200));
	
	/// <summary>
	/// �Q�[�W�̐���������
	/// </summary>
	/// <param name="trs">�`��ʒu</param>
	/// <param name="value">���ݒl�̃A�h���X</param>
	/// <param name="minValue">�ŏ��l</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="gaugeGraph">�Q�[�W�̉摜</param>
	/// <param name="delayGraph">�Q�[�W�̌������̉摜</param>
	/// <param name="increaseGraph">�Q�[�W�̑������̉摜</param>
	/// <param name="backGraph">�Q�[�W�̔w�i�摜</param>
	/// <param name="gaugeLateSpeed">�Q�[�W���x��Ă��鑬�x</param>
	UI_Gauge(const RectTransform& trs, float* value, const float& minValue, const float& maxValue, const int& gaugeGraph, const int& delayGraph, const int& increaseGraph, const int& backGraph, const float& gaugeLateSpeed = 0.04f);

	~UI_Gauge();

	//==========================================================================================
	// ���e��֐�

	void Update() override;
	void Draw() override;

	//==========================================================================================
	// ���Z�b�^�[

	inline void SetValue(float* setValue) { value = setValue; }
	inline void SetMaxValue(const float& setMaxValue) { maxValue = max(setMaxValue, minValue + 1); }
	inline void SetMinValue(const float& setMinValue) { minValue = min(setMinValue, maxValue + 1); }

	inline void SetBackColor(const int& color) { colors.backColor = color; }

	//==========================================================================================
	// ���Q�b�^�[

	inline const float Value() const { return *value; }
	inline float MaxValue() const { return maxValue; }
	inline float MinValue() const { return minValue; }

	inline UI_Define::SliderColor Colors() const { return colors; }

private:
	//==========================================================================================
	// �������o�ϐ�

	Gauge gauge;

	float* value;	// ���݂̒l
	float maxValue;	// �ő�l
	float minValue;	// �ŏ��l

	int hGauge;			// �Q�[�W�̉摜
	int hDelayColor;	// �������̉摜
	int hIncreaseColor;	// �������̉摜
	int hBackColor;		// �w�i�摜

	UI_Define::SliderColor colors;	// �F���
	int innerEdgeColor;	// �����̊p�̐F
	int outerEdgeColor;	// �O���̊p�̐F

	Vector2 gaugeSize;	// �Q�[�W�̑傫��

	float gaugeThickness;	// �Q�[�W�̉��̑���
	float gaugeLateSpeed;	// �Q�[�W���x��Ă��鑬�x
};