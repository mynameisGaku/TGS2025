#pragma once

#include <stdio.h>
#include <DxLib.h>

class Vector2;
class Vector3;
class Matrix;

/**
	@brief	4�����̃x�N�g�����`���܂��B
*/
class Vector4
{
public:

	float	x;		///< X �v�f
	float	y;		///< Y �v�f
	float	z;		///< Z �v�f
	float	w;		///< W �v�f

public:

	/**
		@brief		���ׂĂ̗v�f�� 0.0 �ɐݒ肵�ăC���X�^���X�����������܂��B
	*/
	Vector4();

	/**
		@brief		�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector4(float x, float y, float z, float w);
	
	/**
		@brief		�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector4(float xyzw);

	/**
		@brief		Vector2 �� z, w �l���w�肵�ăC���X�^���X�����������܂��B
	*/
	Vector4(const Vector2& vec, float z, float w);

	/**
		@brief		Vector3 �� w �l���w�肵�ăC���X�^���X�����������܂��B
	*/
	Vector4(const Vector3& vec, float w);

public:

	/**
		@brief		�e�v�f�ɒl��ݒ肵�܂��B
	*/
	void Set(float x, float y, float z, float w);

	/**
		@brief		���̃x�N�g���� X Y �v�f�� Vector2 �Ƃ��ĕԂ��܂��B
	*/
	const Vector2& GetXY() const;

	/**
		@brief		���̃x�N�g���� X Y Z �v�f�� Vector3 �Ƃ��ĕԂ��܂��B
	*/
	const Vector3& GetXYZ() const;

	/**
		@brief		�x�N�g���̒�����Ԃ��܂��B
	*/
	float GetLength() const;

	/**
		@brief		�x�N�g���̒����� 2 ���Ԃ��܂��B
	*/
	float GetLengthSquared() const;

	/**
		@brief		���̃x�N�g�����w�肳�ꂽ�ő�l�ƍŏ��l�͈̔͂ɃN�����v���܂��B
	*/
	void Clamp(float min_xyzw, float max_xyzw);

	/**
		@brief		���̃x�N�g�����w�肳�ꂽ�ő�l�ƍŏ��l�͈̔͂ɃN�����v���܂��B
		@param[in]	minVec	: �ŏ��l
		@param[in]	maxVec	: �ő�l
	*/
	void Clamp(const Vector4& minVec, const Vector4& maxVec);

	/**
		@brief		�v�f�̂����ꂩ�� NaN �܂��� Inf ���𔻕ʂ��܂��B
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "%f, %f, %f, %f\n" ���g�p���܂��B
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

public:

	/**
		@brief		�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@return		���K�����ꂽ�x�N�g��
	*/
	static Vector4 Normalize(const Vector4& vec);

	/**
		@brief		2�̃x�N�g���̓��ς��v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���̓���
	*/
	static float Dot(const Vector4& vec1, const Vector4& vec2);

	/**
		@brief		2�̃x�N�g���̍ŏ��l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ŏ��l����쐬���ꂽ�x�N�g��
	*/
	static Vector4 Min(const Vector4& vec1, const Vector4& vec2);

	/**
		@brief		2�̃x�N�g���̍ő�l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ő�l����쐬���ꂽ�x�N�g��
	*/
	static Vector4 Max(const Vector4& vec1, const Vector4& vec2);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector4 Transform(const Vector4& vec, const Matrix& mat);

	/**
		@brief		2 �̃x�N�g���Ԃ̐��`��Ԃ��s���܂��B
		@param[in]	start	: �J�n�x�N�g�� (t = 0.0 �̂Ƃ��̒l)
		@param[in]	end		: �I���x�N�g�� (t = 1.0 �̂Ƃ��̒l)
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃x�N�g��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector4 Lerp(const Vector4& start, const Vector4& end, float t);

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
	static Vector4 Hermite(const Vector4& v1, const Vector4& a1, const Vector4& v2, const Vector4& a2, float t);

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
	static Vector4 CatmullRom(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4, float t);

public:

	Vector4& operator += (const Vector4& v);
	Vector4& operator += (float v);
	Vector4& operator -= (const Vector4& v);
	Vector4& operator -= (float v);
	Vector4& operator *= (const Vector4& v);
	Vector4& operator *= (float v);
	Vector4& operator /= (const Vector4& v);
	Vector4& operator /= (float v);

	friend Vector4 operator + (const Vector4& v1, const Vector4& v2);
	friend Vector4 operator + (const Vector4& v1, float v2);
	friend Vector4 operator + (float v1, const Vector4& v2);
	friend Vector4 operator - (const Vector4& v1, const Vector4& v2);
	friend Vector4 operator - (const Vector4& v1, float v2);
	friend Vector4 operator - (float v1, const Vector4& v2);
	friend Vector4 operator * (const Vector4& v1, const Vector4& v2);
	friend Vector4 operator * (const Vector4& v1, float v2);
	friend Vector4 operator * (float v1, const Vector4& v2);
	friend Vector4 operator / (const Vector4& v1, const Vector4& v2);
	friend Vector4 operator / (const Vector4& v1, float v2);
	friend Vector4 operator / (float v1, const Vector4& v2);

	friend Vector4 operator - (const Vector4& v1);

	bool operator == (const Vector4& v) const;
	bool operator != (const Vector4& v) const;
};
