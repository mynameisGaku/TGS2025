#pragma once
#include "DxLib.h"
#include <math.h>

class Quaternion {
public:

	float x;   // x��
	float y;   // y��
	float z;   // z��
	float w;   // �C�ӎ�

	float yaw;
	float pitch;
	float roll;

	VECTOR xAxis, yAxis, zAxis;

	MATRIX matTrans;
	MATRIX matRot;
	MATRIX matScale;
	MATRIX mat;

	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Quaternion();
	Quaternion(float x, float y, float z, float t);
	Quaternion(VECTOR vec, float t);

	~Quaternion();

	//================================================================================
	// ���ϊ�

	// dxlib��VECTOR�\���̂���Quaternion�ւ̕ϊ�
	// VECTOR��Quaternion�ɑ�����悤�Ƃ���ƁA���̃R���X�g���N�^�ňÖٓI�ɕϊ����Ă����
	inline Quaternion(const VECTOR& v) : Quaternion(v.x, v.y, v.z, 0.0f) {};

	// Vector3����dxlib��VECTOR�\���̂ւ̕ϊ�
	// Vector3��VECTOR�������悤�Ƃ���ƁA���̉��Z�q�ňÖٓI�ɕϊ����Ă����
	operator const VECTOR();

	// if�̏�����bool�ϐ���Vector3��������
	// ���̏ꍇ�A�x�N�g���̑傫����0�łȂ����true�Ƃ��Ĉ���
	operator bool() const;

	// �N�H�[�^�j�I����VECTOR��������
	void operator=(const VECTOR& value);

	//================================================================================
	// ���v�Z

	// �x�N�g���̑����Z
	Quaternion  operator+(const VECTOR& value);

	// �x�N�g���̈����Z
	Quaternion operator-(const VECTOR& value);

	// �x�N�g���̊|���Z
	Quaternion operator*(const VECTOR& value);

	// �x�N�g���̊���Z
	Quaternion operator/(const VECTOR& value);

	// �x�N�g���̑����Z
	void operator+=(const VECTOR& value);

	// �x�N�g���̈����Z
	void operator-=(const VECTOR& value);

	// �x�N�g���̊|���Z
	void operator*=(const VECTOR& value);

	// �x�N�g���̊���Z
	void operator/=(const VECTOR& value);

	//�N�H�[�^�j�I�����m�̊|���Z
	Quaternion operator*(const Quaternion& q);
	void operator*=(const Quaternion& q);

	//�N�H�[�^�j�I�������]�s���
	MATRIX ToMatrix();
};
