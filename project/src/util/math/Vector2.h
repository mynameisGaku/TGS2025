#pragma once
#include <DxLib.h>
#include <math.h>

/// <summary>
/// �񎲂̃x�N�g��(x,y)�������N���X
/// </summary>
class Vector2 {
public:

	float x;	// x��
	float y;	// y��

	//================================================================================
	// ���R���X�g���N�^

	// x,y��0�ŏ�����
	Vector2() : Vector2(0.0f, 0.0f) {}

	// ��̈�����x,y��������
	Vector2(float v) : Vector2(v, v) {}

	// ������x,y��������
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	};

	//================================================================================
	// ���ϊ�

	// dxlib��VECTOR�\���̂���Vector2�ւ̕ϊ�
	// VECTOR��Vector2�ɑ�����悤�Ƃ���ƁA���̃R���X�g���N�^�ňÖٓI�ɕϊ����Ă����
	inline Vector2(const VECTOR& v) : Vector2(v.x, v.y) {};

	// Vector2����dxlib��VECTOR�\���̂ւ̕ϊ�
	// Vector2��VECTOR�������悤�Ƃ���ƁA���̉��Z�q�ňÖٓI�ɕϊ����Ă����
	inline operator const VECTOR() const { return VGet(x, y, 0); }

	// if�̏�����bool�ϐ���Vector2��������
	// ���̏ꍇ�A�x�N�g���̑傫����0�łȂ����true�Ƃ��Ĉ���
	inline operator bool() const { return (SquareSize() != 0); }

	// Vector2��Vector2��������
	void operator=(const Vector2& value);

	// Vector2��VECTOR��������
	void operator=(const VECTOR& value);

	//================================================================================
	// ���v�Z

	// �x�N�g���̑����Z�Adxlib��VAdd�Ɠ���
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa + b == Vector2(a.x + b.x, a.y + b.y)
	const Vector2 operator+(const Vector2& value) const;

	// �x�N�g���̈����Z�Adxlib��VSub�Ɠ���
	// a, b�Ƃ����x�N�g��������Ƃ���ƁAa - b == Vector2(a.x - b.x, a.y - b.y)
	const Vector2 operator-(const Vector2& value) const;

	// �x�N�g���̊e���Ŋ|���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa * b == Vector2(a.x * b.x, a.y * b.y)
	const Vector2 operator*(const Vector2& value) const;

	// �x�N�g���̊e���Ŋ���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa / b == Vector2(a.x / b.x, a.y / b.y)
	const Vector2 operator/(const Vector2& value) const;

	// �x�N�g���̃X�P�[�����O�Adxlib��VScale�Ɠ���
	// �x�N�g���̊e���ɓ����l���|����B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa * b == Vector2(a.x * b, a.y * b)
	const Vector2 operator*(float value) const;
	const Vector2 operator*(int value) const;

	// �x�N�g���̊e���ɓ����l�Ŋ���B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa / b == Vector2(a.x / b, a.y / b)
	const Vector2 operator/(float value) const;
	const Vector2 operator/(int value) const;

	//================================================================================
	// ���C�R�[���t���̌v�Z

	// �x�N�g���̑����Z�Adxlib��VAdd�Ɠ���
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa + b == Vector2(a.x + b.x, a.y + b.y)
	void operator+=(const Vector2& value);

	// �x�N�g���̈����Z�Adxlib��VSub�Ɠ���
	// a, b�Ƃ����x�N�g��������Ƃ���ƁAa - b == Vector2(a.x - b.x, a.y - b.y)
	void operator-=(const Vector2& value);

	// �x�N�g���̊e���Ŋ|���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa * b == Vector2(a.x * b.x, a.y * b.y)
	void operator*=(const Vector2& value);

	// �x�N�g���̊e���Ŋ���Z
	// a,b�Ƃ����x�N�g��������Ƃ���ƁAa / b == Vector2(a.x / b.x, a.y / b.y)
	void operator/=(const Vector2& value);

	// �x�N�g���̃X�P�[�����O�Adxlib��VScale�Ɠ���
	// �x�N�g���̊e���ɓ����l���|����B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa * b == Vector2(a.x * b, a.y * b)
	void operator*=(float value);
	void operator*=(int value);

	// �x�N�g���̊e���ɓ����l�Ŋ���B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa / b == Vector2(a.x / b, a.y / b)
	void operator/=(float value);
	void operator/=(int value);

	// �x�N�g���̊e���ɓ����l�����B
	// a�Ƃ����x�N�g���Ab�Ƃ����l������Ƃ���ƁAa = b == Vector2(a.x = b, a.y = b)
	void operator=(float value);
	void operator=(int value);

	//================================================================================
	// ���_��

	// ��̃x�N�g���̒l�������Ȃ�true
	bool operator==(const Vector2& value) const;
	bool operator==(const VECTOR& value) const;

	// ��̃x�N�g���̒l���قȂ��Ă����true
	bool operator!=(const Vector2& value) const;
	bool operator!=(const VECTOR& value) const;

	//================================================================================
	// ���e��֐�

	//�@�e�v�f�̍��Z�l���擾����
	inline const float Total() const { return x + y; }

	//�@�e�v�f�̕��ϒl���擾����
	inline const float Average() const { return Total() / 2.0f; }

	// �x�N�g���̒������擾���鎞�A�O�����̒藝�́A�Ō�̕���������镔�����΂����l�B�܂�A������2��̒l
	// �������r�������ʂ́A������2����r�������ʂƓ����Ȃ̂ŁA���̓����蔻��ȂǂŒ����̔�r������Ƃ���p�\
	// �������͌��\�d�������Ȃ̂ŁA��p�\�ȏꏊ�ł�Size()���SquareSize()���g���̂��D�܂���(�w����i�̋K�͂Ȃ�덷�ȋC�����邪)
	// dxlib��VSquareSize�Ɠ���
	inline const float SquareSize() const { return x * x + y * y; }

	// �x�N�g���̒������擾
	// dxlib��VSize�Ɠ���
	inline const float Size() const { return sqrtf(SquareSize()); }

	// �x�N�g���̌����͂��̂܂܁A������1�ɂ����x�N�g��(���K���x�N�g��)���擾
	// �[���x�N�g������ꂽ�ꍇ�[���x�N�g����Ԃ�
	// dxlib��VNorm�Ɠ���
	inline const Vector2 Norm() const {
		float size = Size();
		if (size == 0.0f)
			return Vector2(0.0f, 0.0f);

		return Vector2(x / size, y / size);
	}

	// �����Ƃ̍����擾����
	inline const Vector2 Distance(Vector2 value) const { return Vector2(x - value.x, y - value.y); }

	// �������玩�g�̕��֌����p�x���擾����
	inline const float Direction(Vector2 value) {
		Vector2 v = Distance(value);
		return atan2f(v.y, v.x);
	}

	// �x�N�g���̃T�C�Y��0�ȏ�Ȃ�True���擾����
	inline const bool IsValue() const { return (SquareSize() != 0.0f); }

	// i�̒l�ɂ���ėv�f���擾(0 -> x, 1 -> y)
	// ���[�v�����Ŏg�p
	float Get(int i) const;

	// i�̒l�ɂ���ėv�f��value���Z�b�g(0 -> x, 1 -> y)
	// ���[�v�����Ŏg�p
	void Set(int i, float value);
};

// Vector2��⏕����萔�A�֐�	
namespace V2 {

	//================================================================================
	// ���萔

	// �[���x�N�g��
	static const Vector2 ZERO = Vector2(0.0f);

	// �S��1�̃x�N�g��
	static const Vector2 ONE = Vector2(1.0f);

	//================================================================================
	// ���e��֐�

	// X���W�̗v�f�����ݒ肷��
	// ���̂ق��̗v�f��0�ɂȂ�
	inline const Vector2 SetX(float _x) { return Vector2(_x, 0.0f); }

	// Y���W�̗v�f�����ݒ肷��
	// ���̂ق��̗v�f��0�ɂȂ�
	inline const Vector2 SetY(float _y) { return Vector2(0.0f, _y); }

	// ��̃x�N�g���̓���
	inline float Dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
}
