#pragma once

#include <stdio.h>
#include <DxLib.h>
#include <string>

class Vector2;
class Vector4;
class Quaternion;
class Matrix;

/**
	@brief	3�����̃x�N�g�����`���܂��B
*/
class Vector3
{
public:

	float	x;		///< X �v�f
	float	y;		///< Y �v�f
	float	z;		///< Z �v�f

public:

	static const Vector3   Zero;		///< Vector3(0, 0, 0)
	static const Vector3   UnitX;		///< Vector3(1, 0, 0)
	static const Vector3   UnitY;		///< Vector3(0, 1, 0)
	static const Vector3   UnitZ;		///< Vector3(0, 0, 1)
	static const Vector3   Ones;		///< Vector3(1, 1, 1)
	static const Vector3   Horizontal;	///< Vector3(1, 0, 1)

public:

	/**
		@brief	���ׂĂ̗v�f�� 0.0 �ɐݒ肵�ăC���X�^���X�����������܂��B
	*/
	Vector3();

	/**
		@brief	�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector3(float x, float y, float z);

	/**
		@brief	�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector3(int x, int y, int z);

	/**
		@brief	�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector3(float xyz);

	/**
		@brief	Vector2 �� z �l���w�肵�ăC���X�^���X�����������܂��B
	*/
	Vector3(const Vector2& vec, float z);

public:

	/**
		@brief		�e�v�f�ɒl��ݒ肵�܂��B
	*/
	void SetElement(float x, float y, float z);

	/**
		@brief		���̃x�N�g���� X Y �v�f�� Vector2 �Ƃ��ĕԂ��܂��B
	*/
	const Vector2& GetXY() const;

	/**
		@brief		�x�N�g���̒�����Ԃ��܂��B
	*/
	float GetLength() const;

	/**
		@brief		�x�N�g���̒����� 2 ���Ԃ��܂��B
	*/
	float GetLengthSquared() const;

	/**
		@brief		���̃x�N�g���𐳋K�����܂��B
		@details	�x�N�g���̒����� 0 �̏ꍇ�͐��K�����s���܂���B
	*/
	Vector3 Normalize();

	/**
		@brief		���̃x�N�g�����w�肳�ꂽ�ő�l�ƍŏ��l�͈̔͂ɃN�����v���܂��B
		@param[in]	minVec	: �ŏ��l
		@param[in]	maxVec	: �ő�l
	*/
	Vector3 Clamp(const Vector3& minVec, const Vector3& maxVec);

	/**
		@brief		�w�肳�ꂽ�s����g�p���Ă��̃x�N�g�������W�ϊ����܂��B
		@param[in]	mat		: �����̊�ɂȂ�s��
		@details	�x�N�g���� (X, Y, Z, 1.0) �Ƃ��č��W�ϊ����s���A���ʂ� w �ŏ��Z���܂��B
	*/
	Vector3 TransformCoord(const Matrix& mat);

	/**
		@brief		�v�f�̂����ꂩ�� NaN �܂��� Inf ���𔻕ʂ��܂��B
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "%f, %f, %f\n" ���g�p���܂��B
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

	// 
	// ���[�v�����Ŏg�p
	/**
		@brief		i�̒l�ɂ���ėv�f���擾
					���[�v�����Ŏg�p
		@param[in]	i	: �i0 -> x, 1 -> y, 2 -> z�j
	*/
	float Get(int i) const;

	// i�̒l�ɂ���ėv�f��value���Z�b�g�i0 -> x, 1 -> y, 2 -> z�j
	// ���[�v�����Ŏg�p
	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "%f, %f, %f\n" ���g�p���܂��B
	*/
	void Set(int i, float value);

	/**
		@brief		�w�肳�ꂽ�N�H�[�^�j�I�����g�p���Ă��̃x�N�g�������W�ϊ����܂��B
		@param[in]	qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	Vector3 Transform(const Quaternion& qua);

	/**
		@brief		�w�肳�ꂽ�s����g�p���Ă��̃x�N�g�������W�ϊ����܂��B
		@param[in]	mat		: �����̊�ɂȂ�s��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	Vector3 Transform(const Matrix& mat);

	/* �g���~�`�]�[�� */

	// �����Ƃ̍����擾����
	inline const Vector3 Distance(const Vector3& value) { return Vector3(x - value.x, y - value.y, z - value.z); }

	// �������玩�g�̕��֌����p�x���擾����
	inline const float Direction(const Vector3& value) {
		Vector3 v = Distance(value);
		return atan2f(v.x, v.z);
	}

	// �x�N�g���̃T�C�Y��0�ȏ�Ȃ�True���擾����
	inline const bool IsValue() const { return (GetLengthSquared() != 0.0f); }

	// �����֌W�ɂ���H
	inline const bool IsVertical(const Vector3& r) const {
		float d = Dot(*this, r);
		return (-0.000001f < d && d < 0.000001f);	// �덷�͈͓��Ȃ琂���Ɣ���
	}

	// ���s�֌W�ɂ���H
	inline const bool IsParallel(const Vector3& r) const {
		Vector3 cross = VCross(*this, r);
		float d = cross.GetLengthSquared();
		return (-0.000001f < d && d < 0.000001f);	// �덷�͈͓��Ȃ畽�s�Ɣ���
	}

	// �s�p�֌W�H
	inline const bool IsSharpAngle(const Vector3& r) const { return (Dot(*this, r) >= 0.0f); }

	//�@�e�v�f�̍��Z�l���擾����
	inline const float Total() const { return x + y + z; }

	//�@�e�v�f�̕��ϒl���擾����
	inline const float Average() const { return Total() / 3.0f; }

public:

	/**
		@brief		�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
		@param[in]	x		: �����̊�ɂȂ�x�N�g���� X �v�f
		@param[in]	y		: �����̊�ɂȂ�x�N�g���� Y �v�f
		@param[in]	z		: �����̊�ɂȂ�x�N�g���� Z �v�f
		@return		���K�����ꂽ�x�N�g��
	*/
	static Vector3 Normalize(float x, float y, float z);

	/**
		@brief		�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@return		���K�����ꂽ�x�N�g��
	*/
	static Vector3 Normalize(const Vector3& vec);

	/**
		@brief		2�̃x�N�g���̓��ς��v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���̓���
	*/
	static float Dot(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2�̃x�N�g���̊O�ς��v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���̊O��
	*/
	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2�̃x�N�g���̍ŏ��l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ŏ��l����쐬���ꂽ�x�N�g��
	*/
	static Vector3 Min(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2�̃x�N�g���̍ő�l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ő�l����쐬���ꂽ�x�N�g��
	*/
	static Vector3 Max(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		���˃x�N�g���Ɩ@���x�N�g�����甽�˃x�N�g�����v�Z���܂��B
		@param[in]	vec		: ���˃x�N�g��
		@param[in]	normal	: �@���x�N�g��
		@return		���˃x�N�g��
	*/
	static Vector3 Reflect(const Vector3& vec, const Vector3& normal);

	/**
		@brief		���˃x�N�g���Ɩ@���x�N�g�����犊��x�N�g�����v�Z���܂��B
		@param[in]	vec		: ���˃x�N�g��
		@param[in]	normal	: �@���x�N�g��
		@return		����x�N�g��
	*/
	static Vector3 Slide(const Vector3& vec, const Vector3& normal);

	/**
		@brief		�w�肳�ꂽ�N�H�[�^�j�I�����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector3 Transform(const Vector3& vec, const Quaternion& qua);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector4 Transform(const Vector3& vec, const Matrix& mat);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@details	�x�N�g���� (X, Y, Z, 1.0) �Ƃ��č��W�ϊ����s���A���ʂ� w �ŏ��Z���܂��B
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector3 TransformCoord(const Vector3& vec, const Matrix& mat);

	/**
		@brief		2 �̃x�N�g���Ԃ̐��`��Ԃ��s���܂��B
		@param[in]	start	: �J�n�x�N�g�� (t = 0.0 �̂Ƃ��̒l)
		@param[in]	end		: �I���x�N�g�� (t = 1.0 �̂Ƃ��̒l)
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃x�N�g��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

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
	static Vector3 Hermite(const Vector3& v1, const Vector3& a1, const Vector3& v2, const Vector3& a2, float t);

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
	static Vector3 CatmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t);

	/**
		@brief		�I�u�W�F�N�g���(3D���)�̃x�N�g�����X�N���[�����(2D���)�̃x�N�g���ɕϊ�����B
		@param[in]	point			: �I�u�W�F�N�g��ԏ�̍��W
		@param[in]	worldViewProj	: �����ς݂� ���[���h - �r���[ - �v���W�F�N�V�����s��
		@param[in]	x				: �r���[�|�[�g�̍��� X ���W
		@param[in]	y				: �r���[�|�[�g�̍��� Y ���W
		@param[in]	width			: �r���[�|�[�g�̕�
		@param[in]	height			: �r���[�|�[�g�̍���
		@param[in]	minZ			: �r���[�|�[�g�̍ŏ��[�x
		@param[in]	maxZ			: �r���[�|�[�g�̍ő�[�x
	*/
	static Vector3 Project(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

	/**
		@brief		�X�N���[�����(2D���)�̃x�N�g�����I�u�W�F�N�g���(3D���)�̃x�N�g���ɕϊ�����B
		@param[in]	point			: �X�N���[����ԏ�̍��W
		@param[in]	worldViewProj	: �����ς݂� ���[���h - �r���[ - �v���W�F�N�V�����s��
		@param[in]	x				: �r���[�|�[�g�̍��� X ���W
		@param[in]	y				: �r���[�|�[�g�̍��� Y ���W
		@param[in]	width			: �r���[�|�[�g�̕�
		@param[in]	height			: �r���[�|�[�g�̍���
		@param[in]	minZ			: �r���[�|�[�g�̍ŏ��[�x
		@param[in]	maxZ			: �r���[�|�[�g�̍ő�[�x
	*/
	static Vector3 Unproject(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

	/**
		@brief		x�v�f�ɒl���Z�b�g���A���̃x�N�g����Ԃ� (x, 0, 0)
		@param[in]	x				: x�v�f
	*/
	static Vector3 SetX(float x);
	/**
		@brief		x�v�f�ɒl���Z�b�g���A���̃x�N�g����Ԃ� (x, 0, 0)
		@param[in]	x				: x�v�f
	*/
	static Vector3 SetX(int x);

	/**
		@brief		y�v�f�ɒl���Z�b�g���A���̃x�N�g����Ԃ� (0, y, 0)
		@param[in]	y				: y�v�f
	*/
	static Vector3 SetY(float y);
	/**
		@brief		y�v�f�ɒl���Z�b�g���A���̃x�N�g����Ԃ� (0, y, 0)
		@param[in]	y				: y�v�f
	*/
	static Vector3 SetY(int y);

	/**
		@brief		z�v�f�ɒl���Z�b�g���A���̃x�N�g����Ԃ� (0, 0, z)
		@param[in]	z				: z�v�f
	*/
	static Vector3 SetZ(float z);
	/**
		@brief		z�v�f�ɒl���Z�b�g���A���̃x�N�g����Ԃ� (0, 0, z)
		@param[in]	z				: z�v�f
	*/
	static Vector3 SetZ(int z);

public:

	Vector3& operator += (const Vector3& v);
	Vector3& operator += (float v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator -= (float v);
	Vector3& operator *= (const Vector3& v);
	Vector3& operator *= (float v);
	Vector3& operator /= (const Vector3& v);
	Vector3& operator /= (float v);

	friend Vector3 operator + (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator + (const Vector3& v1, float v2);
	friend Vector3 operator + (float v1, const Vector3& v2);
	friend Vector3 operator - (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator - (const Vector3& v1, float v2);
	friend Vector3 operator - (float v1, const Vector3& v2);
	friend Vector3 operator * (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator * (const Vector3& v1, float v2);
	friend Vector3 operator * (float v1, const Vector3& v2);
	friend Vector3 operator / (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator / (const Vector3& v1, float v2);
	friend Vector3 operator / (float v1, const Vector3& v2);

	friend Vector3 operator - (const Vector3& v1);

	bool operator == (const Vector3& v) const;
	bool operator != (const Vector3& v) const;

	// DxLib��VECTOR�ƌ݊�������������
	Vector3(const VECTOR& v);
	operator const VECTOR() const;
	operator bool() const;
	Vector3 operator + (const VECTOR& v) const;
	Vector3& operator += (const VECTOR& v);
	Vector3 operator - (const VECTOR& v) const;
	Vector3& operator -= (const VECTOR& v);
	Vector3 operator * (const VECTOR& v) const;
	Vector3& operator *= (const VECTOR& v);
	Vector3 operator / (const VECTOR& v) const;
	Vector3& operator /= (const VECTOR& v);


	const friend Vector3 operator *(const Vector3& v, const MATRIX& m1);
	friend Vector3& operator *=(Vector3& v, const MATRIX& m1);
};