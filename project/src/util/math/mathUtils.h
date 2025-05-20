#pragma once

/**
	@brief	��ʓI�Ȑ��w�֐��ƒ萔���܂Ƃ߂����W���[���ł��B
*/
class Math
{
public:

	static const float PI;			///< �~����	(3.14159265...)
	static const float PIOver2;		///< �~���� / 2
	static const float EPSILON;		///< ���������_�̌덷

public:

	/// �x�����W�A���ɕϊ����܂��B
	static inline float ToRadians(float d) { return d * 0.017453292519943295769236907684886f; }	// 0.017�c ��180 �̋t�� * PI

	/// ���W�A����x�ɕϊ����܂��B
	static inline float ToDegrees(float r) { return r * 57.295779513082320876798154814105f; }	// 57.29�c ��PI �̋t�� * 180

	/// �l���w�肳�ꂽ�͈͓��ɐ������܂��B
	static inline float Clamp(float value, float minValue, float maxValue) { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

	/// �l��0.0�`1.0�͈͓̔��ɐ������܂��B
	static inline float Clamp01(float value) { return Clamp(value, 0.0f, 1.0f); }

	/// �w��l�ȏ�̍ŏ��� 2 �ׂ̂��搔��Ԃ��܂��B
	static unsigned int NextPow2(unsigned int value);

	/// �덷���l�����A���������_�̔�r���s���܂��B
	static bool EqualsFloat(float value1, float value2);

	/// �l�� NaN(��) ���𔻒肵�܂��B
	static bool IsNaN(float value);

	/// �l�� NaN(��) ���𔻒肵�܂��B
	static bool IsNaN(double value);

	/// �l�������傩�𔻒肵�܂��B
	static bool IsInf(float value);

	/// �l�������傩�𔻒肵�܂��B
	static bool IsInf(double value);

	/// �l�� NaN(��) �܂��� ������ ���𔻒肵�܂��B
	static bool IsNaNOrInf(float value) { return (IsNaN(value) || IsInf(value)); }

	/// �l�� NaN(��) �܂��� ������ ���𔻒肵�܂��B
	static bool IsNaNOrInf(double value) { return (IsNaN(value) || IsInf(value)); }

	/**
		@brief		�������x�^���̎��Œl���v�Z���܂��B
		@param[in]	p	: �J�n�l
		@param[in]	v	: �����x
		@param[in]	a	: �����x
		@param[in]	t	: ����
		@return		�v�Z����
	*/
	static float QuadAccel(float p, float v, float a, float t);

	/**
		@brief		2�̒l�̊Ԃ���`�⊮���܂��B
		@param[in]	v1	: �J�n�l
		@param[in]	v2	: �I���l
		@param[in]	t	: ���d�W��
		@return		��Ԍ��ʂ̒l
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static float Lerp(float v1, float v2, float t);

	/**
		@brief		�G���~�[�g�X�v���C����Ԃ����s���܂��B
		@param[in]	v1	: �J�n�l
		@param[in]	a1	: �J�n�l�̐ڐ��̌X��(���x)
		@param[in]	v2	: �I���l
		@param[in]	a2	: �I���l�̐ڐ��̌X��(���x)
		@param[in]	t	: ���d�W��
		@return		��Ԍ��ʂ̒l
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static float Hermite(float v1, float a1, float v2, float a2, float t);

	/**
		@brief		Catmull-Rom ��Ԃ����s���܂��B
		@param[in]	v1	: 1�Ԗڂ̈ʒu
		@param[in]	v2	: 2�Ԗڂ̈ʒu (t = 0.0 �̂Ƃ��̒l)
		@param[in]	v3	: 3�Ԗڂ̈ʒu (t = 1.0 �̂Ƃ��̒l)
		@param[in]	v4	: 4�Ԗڂ̈ʒu
		@param[in]	t	: ���d�W��
		@return		��Ԍ��ʂ̒l
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static float CatmullRom(float v1, float v2, float v3, float v4, float t);

};
