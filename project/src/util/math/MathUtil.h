#pragma once
#include <list>
#include "src/util/math/Vector3.h"

/**
	@brief	��ʓI�Ȑ��w�֐��ƒ萔���܂Ƃ߂����W���[���ł��B
*/
class MathUtil
{
public:

	static const float PI;			///< �~����	(3.14159265...)
	static const float PIOver2;		///< �~���� / 2
	static const float EPSILON;		///< ���������_�̌덷
	static const float PI_TW;		///< �~�����̂Q�{
	static const float PI_SQ;		///< �~�����̂Q��
	static const float GRAVITY_2D;	///< �d��(2D)
	static const float GRAVITY_3D;	///< �d��(3D)

public:

	/// �x�����W�A���ɕϊ����܂��B
	static inline float ToRadians(float d) { return d * 0.017453292519943295769236907684886f; }	// 0.017�c ��180 �̋t�� * PI

	/// ���W�A����x�ɕϊ����܂��B
	static inline float ToDegrees(float r) { return r * 57.295779513082320876798154814105f; }	// 57.29�c ��PI �̋t�� * 180

	/// �l���w�肳�ꂽ�͈͓��ɐ������܂��B
	static inline float Clamp(float value, float minValue, float maxValue) { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

	/// �l��0.0�`1.0�͈͓̔��ɐ������܂��B
	static inline float Clamp01(float value) { return Clamp(value, 0.0f, 1.0f); }

	static inline void ClampAssing(float* value, float minValue,float maxValue) { *value = Clamp(*value, minValue, maxValue); }

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

	static float LerpAngle(float a, float b, float t);

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
	static Vector3 CatmullRomVec3(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	// �����ɂ���
	static float Half(float value);

	// �l���ɂ���
	static float Quarter(float value);

	/// <summary>
	/// �m���̌v�Z
	/// </summary>
	/// <param name="probability">�m��(���S����)</param>
	/// <returns>���������ꍇ�ATrue</returns>
	static bool Probability(int probability);

	static float RateDecrease(float start, float rate);

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="value">�������擾����l</param>
	static int Digit(int value);

	/// <summary>
	/// �l�̌ܓ����s��
	/// </summary>
	/// <param name="value">�l�̌ܓ����s���l</param>
	/// <param name="decimalPoint">�����_���ʂ܂Ŏl�̌ܓ����邩</param>
	/// <returns>�l�̌ܓ����s�����l���擾����</returns>
	static float Rounding(float value, int decimalPoint);

	/// <summary>
	/// �p�x�����X�ɕω������鏈���B
	/// ���݂̒l���I�_�p�x�ɂȂ�܂ŁA��]���x�����Z��������B
	/// </summary>
	/// <param name="current">���݂̒l</param>
	/// <param name="terminus">�I�_�p�x</param>
	/// <param name="rotSpeed">��]���x</param>
	static float RotAngle(float current, float terminus, float rotSpeed);

	/// <summary>
	/// ��]�p�x�ɐ�����������(-PI�`PI)
	/// </summary>
	static void RotLimit(float* rot);

	static int CalcList(const std::list<int>& list);
};
