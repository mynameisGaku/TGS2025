#pragma once

#include <stdio.h>
#include <DxLib.h>

class Vector4;
class Matrix;

/**
	@brief	2�����̃x�N�g�����`���܂��B
*/
class Vector2
{
public:

	float	x;		///< X �v�f
	float	y;		///< Y �v�f

public:

	static const Vector2   Zero;		///< Vector2(0, 0)
	static const Vector2   UnitX;		///< Vector2(1, 0)
	static const Vector2   UnitY;		///< Vector2(0, 1)
	static const Vector2   Ones;		///< Vector2(1, 1)

public:

	/**
		@brief		���ׂĂ̗v�f�� 0.0 �ɐݒ肵�ăC���X�^���X�����������܂��B
	*/
	Vector2();

	/**
		@brief		�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector2(float x, float y);

	/**
		@brief		�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector2(int x, int y);

	/**
		@brief		�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector2(float xy);

public:

	/**
		@brief		�e�v�f�ɒl��ݒ肵�܂��B
	*/
	void Set(float x, float y);

	/**
		@brief		���̃x�N�g���̒�����Ԃ��܂��B
	*/
	float GetLength() const;

	/**
		@brief		���̃x�N�g���̒����� 2 ���Ԃ��܂��B
	*/
	float GetLengthSquared() const;

	/**
		@brief		���̃x�N�g���𐳋K�����܂��B
		@details	�x�N�g���̒����� 0 �̏ꍇ�͐��K�����s���܂���B
	*/
	Vector2 Normalize();

	/**
		@brief		�w�肳�ꂽ�s����g�p���Ă��̃x�N�g�������W�ϊ����܂��B
		@param[in]	mat		: ���W�ϊ��s��
		@details	�x�N�g���� (X, Y, 0.0, 1.0) �Ƃ��č��W�ϊ����s���A���ʂ� w �ŏ��Z���܂��B
	*/
	void TransformCoord(const Matrix& mat);

	/**
		@brief		�v�f�̂����ꂩ�� NaN �܂��� Inf ���𔻕ʂ��܂��B
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "%f, %f\n" ���g�p���܂��B
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

	// i�̒l�ɂ���ėv�f���擾�i0 -> x, 1 -> y�j
	// ���[�v�����Ŏg�p
	/**
		@brief		i�̒l�ɂ���ėv�f���擾�i0 -> x, 1 -> y�j
		@param[in]	i	: �擾�������v�f�ԍ�
		@details	���[�v�����Ŏg�p
	*/
	float Get(int i) const;

	// i�̒l�ɂ���ėv�f��value���Z�b�g�i0 -> x, 1 -> y�j
	// ���[�v�����Ŏg�p
	/**
		@brief		i�̒l�ɂ���ėv�f��value���Z�b�g�i0 -> x, 1 -> y�j
		@param[in]	i		: ���Ԗڂ̗v�f�ɃZ�b�g���邩
		@param[in]	value	: �Z�b�g����l(float)
		@details	���[�v�����Ŏg�p
	*/
	void Set(int i, float value);

	//�@�e�v�f�̍��Z�l���擾����
	inline const float Total() const { return x + y; }

	//�@�e�v�f�̕��ϒl���擾����
	inline const float Average() const { return Total() / 2.0f; }

public:

	/**
		@brief		�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@return		���K�����ꂽ�x�N�g��
	*/
	static Vector2 Normalize(const Vector2& vec);

	/**
		@brief		2�̃x�N�g���̓��ς��v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���̓���
	*/
	static float Dot(const Vector2& vec1, const Vector2& vec2);

	/**
		@brief		2�̃x�N�g���̍ŏ��l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ŏ��l����쐬���ꂽ�x�N�g��
	*/
	static Vector2 Min(const Vector2& vec1, const Vector2& vec2);

	/**
		@brief		2�̃x�N�g���̍ő�l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ő�l����쐬���ꂽ�x�N�g��
	*/
	static Vector2 Max(const Vector2& vec1, const Vector2& vec2);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector4 Transform(const Vector2& vec, const Matrix& mat);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@details	�x�N�g���� (X, Y, 0.0, 1.0) �Ƃ��č��W�ϊ����s���A���ʂ� w �ŏ��Z���܂��B
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector2 TransformCoord(const Vector2& vec, const Matrix& mat);

	/**
		@brief		2 �̃x�N�g���Ԃ̐��`��Ԃ��s���܂��B
		@param[in]	start	: �J�n�x�N�g�� (t = 0.0 �̂Ƃ��̒l)
		@param[in]	end		: �I���x�N�g�� (t = 1.0 �̂Ƃ��̒l)
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃x�N�g��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

	/**
		@brief		�w�肳�ꂽ�x�N�g�����g�p���� �G���~�[�g�X�v���C����Ԃ����s���܂��B
		@param[in]	v1	: �J�n�x�N�g��
		@param[in]	a1	: �J�n�x�N�g���̐ڐ��x�N�g��(���x)
		@param[in]	v2	: �I���x�N�g��
		@param[in]	a2	: �I���x�N�g���̐ڐ��x�N�g��(���x)
		@param[in]	t	: ���d�W��
		@return		��Ԍ��ʂ̒l
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector2 Hermite(const Vector2& v1, const Vector2& a1, const Vector2& v2, const Vector2& a2, float t);

	/**
		@brief		�w�肳�ꂽ�x�N�g�����g�p���� Catmull-Rom ��Ԃ��s���܂��B
		@param[in]	vec1	: 1�Ԗڂ̈ʒu
		@param[in]	vec2	: 2�Ԗڂ̈ʒu (t = 0.0 �̂Ƃ��̒l)
		@param[in]	vec3	: 3�Ԗڂ̈ʒu (t = 1.0 �̂Ƃ��̒l)
		@param[in]	vec4	: 4�Ԗڂ̈ʒu
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃x�N�g��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector2 CatmullRom(const Vector2& vec1, const Vector2& vec2, const Vector2& vec3, const Vector2& vec4, float t);

public:

	Vector2& operator += (const Vector2& v);
	Vector2& operator += (float v);
	Vector2& operator -= (const Vector2& v);
	Vector2& operator -= (float v);
	Vector2& operator *= (const Vector2& v);
	Vector2& operator *= (float v);
	Vector2& operator /= (const Vector2& v);
	Vector2& operator /= (float v);

	friend Vector2 operator + (const Vector2& v1, const Vector2& v2);
	friend Vector2 operator + (const Vector2& v1, float v2);
	friend Vector2 operator + (float v1, const Vector2& v2);
	friend Vector2 operator - (const Vector2& v1, const Vector2& v2);
	friend Vector2 operator - (const Vector2& v1, float v2);
	friend Vector2 operator - (float v1, const Vector2& v2);
	friend Vector2 operator * (const Vector2& v1, const Vector2& v2);
	friend Vector2 operator * (const Vector2& v1, float v2);
	friend Vector2 operator * (float v1, const Vector2& v2);
	friend Vector2 operator / (const Vector2& v1, const Vector2& v2);
	friend Vector2 operator / (const Vector2& v1, float v2);
	friend Vector2 operator / (float v1, const Vector2& v2);

	friend Vector2 operator - (const Vector2& v1);

	bool operator == (const Vector2& v) const;
	bool operator != (const Vector2& v) const;

	Vector2(const VECTOR& v);
	operator const VECTOR() const;
	operator bool() const;
};
