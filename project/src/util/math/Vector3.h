#pragma once
#include <DxLib.h>
#include <math.h>

/// <summary>
/// �O���̃x�N�g��(x,y,z)�������N���X
/// </summary>
class Vector3 {
public:

	float x;	// x��
	float y;	// y��
	float z;	// z��

	//================================================================================
	// ���R���X�g���N�^

	// x,y,z��0�ŏ�����
	Vector3() : Vector3(0.0f, 0.0f, 0.0f) {}

	// ������x,y,z��������
	Vector3(float v) : Vector3(v, v, v) {}

	// ������x,y,z��������
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	};

	//================================================================================
	// ���ϊ�

	// dxlib��VECTOR�\���̂���Vector3�ւ̕ϊ�
	// VECTOR��Vector3�ɑ�����悤�Ƃ���ƁA���̃R���X�g���N�^�ňÖٓI�ɕϊ����Ă����
	Vector3(const VECTOR& v) : Vector3(v.x, v.y, v.z) {};

	// Vector3����dxlib��VECTOR�\���̂ւ̕ϊ�
	// Vector3��VECTOR�������悤�Ƃ���ƁA���̉��Z�q�ňÖٓI�ɕϊ����Ă����
	inline operator const VECTOR() const { return VGet(x, y, z); }

	// if�̏�����bool�ϐ���Vector3��������
	// ���̏ꍇ�A�x�N�g���̑傫����0�łȂ����true�Ƃ��Ĉ���
	inline operator bool() const { return (SquareSize() != 0); }

	// Vector3��Vector3��������
	void operator=(const Vector3& value);

	// Vector3��VECTOR��������
	void operator=(const VECTOR& value);

	//================================================================================
	// ���v�Z

	// �x�N�g���̑����Z�Adxlib��VAdd�Ɠ���
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa + b == Vector3(a.x + b.x, a.y + b.y, a.z + b.z)
	const Vector3 operator+(const Vector3& value) const;
	const Vector3 operator+(const VECTOR& value) const;

	// �x�N�g���̈����Z�Adxlib��VSub�Ɠ���
	// a, b�Ƃ����x�N�g��������Ƃ���ƁAa - b == Vector3(a.x - b.x, a.y - b.y, a.z - b.z)
	const Vector3 operator-(const Vector3& value) const;
	const Vector3 operator-(const VECTOR& value) const;

	// �x�N�g���̊e���Ŋ|���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa * b == Vector3(a.x * b.x, a.y * b.y, a.z * b.z)
	// �x�N�g������xz�����������o���������ɁAvec * Vector3(1, 0, 1)���Ċ����Ŏg�p
	const Vector3 operator*(const Vector3& value) const;
	const Vector3 operator*(const VECTOR& value) const;

	// �x�N�g���̊e���Ŋ���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa / b == Vector3(a.x / b.x, a.y / b.y, a.z / b.z)
	const Vector3 operator/(const Vector3& value) const;
	const Vector3 operator/(const VECTOR& value) const;

	// �x�N�g���̃X�P�[�����O�Adxlib��VScale�Ɠ���
	// �x�N�g���̊e���ɓ����l���|����B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa * b == Vector3(a.x * b, a.y * b, a.z * b)
	const Vector3 operator*(float value) const;
	const Vector3 operator*(int value) const;

	// �x�N�g���̊e���ɓ����l�Ŋ���B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa / b == Vector3(a.x / b, a.y / b, a.z / b)
	const Vector3 operator/(float value) const;
	const Vector3 operator/(int value) const;

	// �x�N�g���ɍs���K��
	// dxlib��VTransform�Ɠ���
	const Vector3 operator*(const MATRIX& mat) const;

	//================================================================================
	// ���C�R�[���t���̌v�Z

	// �x�N�g���̑����Z�Adxlib��VAdd�Ɠ���
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa + b == Vector3(a.x + b.x, a.y + b.y, a.z + b.z)
	void operator+=(const Vector3& value);
	void operator+=(const VECTOR& value);

	// �x�N�g���̈����Z�Adxlib��VSub�Ɠ���
	// a, b�Ƃ����x�N�g��������Ƃ���ƁAa - b == Vector3(a.x - b.x, a.y - b.y, a.z - b.z)
	void operator-=(const Vector3& value);
	void operator-=(const VECTOR& value);

	// �x�N�g���̊e���Ŋ|���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa * b == Vector3(a.x * b.x, a.y * b.y, a.z * b.z)
	// �x�N�g������xz�����������o���������ɁAvec * Vector3(1, 0, 1)���Ċ����Ŏg�p
	void operator*=(const Vector3& value);
	void operator*=(const VECTOR& value);

	// �x�N�g���̊e���Ŋ���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa / b == Vector3(a.x / b.x, a.y / b.y, a.z / b.z)
	void operator/=(const Vector3& value);
	void operator/=(const VECTOR& value);

	// �x�N�g���̃X�P�[�����O�Adxlib��VScale�Ɠ���
	// �x�N�g���̊e���ɓ����l���|����B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa * b == Vector3(a.x * b, a.y * b, a.z * b)
	void operator*=(float value);
	void operator*=(int value);

	// �x�N�g���̊e���ɓ����l�Ŋ���B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa / b == Vector3(a.x / b, a.y / b, a.z / b)
	void operator/=(float value);
	void operator/=(int value);

	// �x�N�g���ɍs���K��
	// dxlib��VTransform�Ɠ���
	void operator*=(const MATRIX& mat);

	// �x�N�g���̊e���ɓ����l�����B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa = b == Vector3(a.x = b, a.y = b, a.z = b)
	void operator=(float value);
	void operator=(int value);

	//================================================================================
	// ���_��

	// ��̃x�N�g���̒l�������Ȃ�true
	bool operator==(const Vector3& value) const;
	bool operator==(const VECTOR& value) const;

	// ��̃x�N�g���̒l���قȂ��Ă����true
	bool operator!=(const Vector3& value) const;
	bool operator!=(const VECTOR& value) const;

	//================================================================================
	// ���e��֐�

	//�@�e�v�f�̍��Z�l���擾����
	inline const float Total() const { return x + y + z; }

	//�@�e�v�f�̕��ϒl���擾����
	inline const float Average() const { return Total() / 3.0f; }
	
	// �x�N�g���̒������擾���鎞�A�O�����̒藝�́A�Ō�̕���������镔�����΂����l�B�܂�A������2��̒l
	// �������r�������ʂ́A������2����r�������ʂƓ����Ȃ̂ŁA���̓����蔻��ȂǂŒ����̔�r������Ƃ���p�\
	// �������͌��\�d�������Ȃ̂ŁA��p�\�ȏꏊ�ł�Size()���SquareSize()���g���̂��D�܂���
	// dxlib��VSquareSize�Ɠ���
	inline const float SquareSize() const { return x * x + y * y + z * z; }

	// �x�N�g���̒������擾
	// dxlib��VSize�Ɠ���
	inline const float Size() const { return sqrtf(SquareSize()); }

	// �x�N�g���̌����͂��̂܂܁A������1�ɂ����x�N�g��(���K���x�N�g��)���擾
	// �[���x�N�g������ꂽ�ꍇ�[���x�N�g����Ԃ�
	// dxlib��VNorm�Ɠ���
	inline const Vector3 Norm() const {
		float size = Size();
		if (size == 0.0f)
			return Vector3(0.0f, 0.0f, 0.0f);

		return Vector3(x / size, y / size, z / size);
	}

	// �����Ƃ̍����擾����
	inline const Vector3 Distance(Vector3 value) { return Vector3(x - value.x, y - value.y, z - value.z); }

	// �������玩�g�̕��֌����p�x���擾����
	inline const float Direction(Vector3 value) {
		Vector3 v = Distance(value);
		return atan2f(v.x, v.z);
	}

	// �x�N�g���̃T�C�Y��0�ȏ�Ȃ�True���擾����
	inline const bool IsValue() const { return (SquareSize() != 0.0f); }

	// �����֌W�ɂ���H
	inline const bool IsVertical(const Vector3& r) const {
		float d = VDot(*this, r);
		return (-0.000001f < d && d < 0.000001f);	// �덷�͈͓��Ȃ琂���Ɣ���
	}

	// ���s�֌W�ɂ���H
	inline const bool IsParallel(const Vector3& r) const {
		Vector3 cross = VCross(*this, r);
		float d = cross.SquareSize();
		return (-0.000001f < d && d < 0.000001f);	// �덷�͈͓��Ȃ畽�s�Ɣ���
	}

	// �s�p�֌W�H
	inline const bool IsSharpAngle(const Vector3& r) const { return (VDot(*this, r) >= 0.0f); }

	// i�̒l�ɂ���ėv�f���擾(0 -> x, 1 -> y, 2 -> z)
	// ���[�v�����Ŏg�p
	float Get(int i) const;

	// i�̒l�ɂ���ėv�f��value���Z�b�g(0 -> x, 1 -> y, 2 -> z)
	// ���[�v�����Ŏg�p
	void Set(int i, float value);
	
	/**
	@brief		2 �̃x�N�g���Ԃ̐��`��Ԃ��s���܂��B
	@param[in]	start	: �J�n�x�N�g�� (t = 0.0 �̂Ƃ��̒l)
	@param[in]	end		: �I���x�N�g�� (t = 1.0 �̂Ƃ��̒l)
	@param[in]	t		: ���d�W��
	@return		��Ԍ��ʂ̃x�N�g��
	@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

};

// Vector3��⏕����萔�A�֐�
namespace V3 {

	//================================================================================
	// ���萔

	// �[���x�N�g��
	static const Vector3 ZERO = Vector3(0.0f);

	// �S��1�̃x�N�g��
	static const Vector3 ONE = Vector3(1.0f);

	// ����w���P�ʃx�N�g��
	static const Vector3 UP = Vector3(0.0f, 1.0f, 0.0f);

	// �����w���P�ʃx�N�g��
	static const Vector3 DOWN = Vector3(0.0f, -1.0f, 0.0f);

	// �����w���P�ʃx�N�g��
	static const Vector3 LEFT = Vector3(-1.0f, 0.0f, 0.0f);

	// �E���w���P�ʃx�N�g��
	static const Vector3 RIGHT = Vector3(1.0f, 0.0f, 0.0f);

	// �O�����w���P�ʃx�N�g��
	static const Vector3 FORWARD = Vector3(0.0f, 0.0f, 1.0f);

	// ������w���P�ʃx�N�g��
	static const Vector3 BACK = Vector3(0.0f, 0.0f, -1.0f);

	// X����Z�����w���P�ʃx�N�g��
	static const Vector3 HORIZONTAL = Vector3(1.0f, 0.0f, 1.0f);

	//================================================================================
	// ���e��֐�

	// X���W�̗v�f�����ݒ肷��
	// ���̂ق��̗v�f��0�ɂȂ�
	inline const Vector3 SetX(float _x) { return Vector3(_x, 0.0f, 0.0f); }

	// Y���W�̗v�f�����ݒ肷��
	// ���̂ق��̗v�f��0�ɂȂ�
	inline const Vector3 SetY(float _y) { return Vector3(0.0f, _y, 0.0f); }

	// Z���W�̗v�f�����ݒ肷��
	// ���̂ق��̗v�f��0�ɂȂ�
	inline const Vector3 SetZ(float _z) { return Vector3(0.0f, 0.0f, _z); }

	// ��̃x�N�g���̓���
	inline float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	// ��̃x�N�g���̊O��
	inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {

		Vector3 result;

		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;

		return result;
	}
}