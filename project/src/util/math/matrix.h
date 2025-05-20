#pragma once
#include <stdio.h>
#include <string.h>
#include "Vector4.h"
#include "mathCommon.h"

class Vector3;
class Quaternion;
class Transform;
class Plane;

/**
	@brief      4x4 �̍s����`���܂��B
	@details	�s��͍s�D��ŁA����� DirectX �̒�`�Ɠ����ł��B

				���̃N���X�͎����@�\�� static �֐��ƃC���X�^���X�֐�������܂��B
				�Ⴆ�� RotationX(static �֐�) �� RotateX(�C���X�^���X�֐�) �ł��B
				�O�҂͐V�����s����쐬���ĕԂ��̂ɑ΂��A��҂͌��݂̍s���ύX���܂��B
				�Ⴆ�΁A�ȉ��� m1 �� m2 �͓������ʂɂȂ�܂��B

				@code
				Matrix m1 = Matrix::RotationX(0.1) * Matrix::RotationY(0.2) * Matrix::RotationZ(0.3);

				Matrix m2;
				m2.RotateX(0.1);
				m2.RotateY(0.2);
				m2.RotateZ(0.3);
				@endcode

				�Ȃ��A��҂͍s��̐����Ə�Z���܂Ƃ߂čs���悤�ɍœK������Ă���A�����ɓ��삵�܂��B
*/
class Matrix
{
public:

	union
	{
		struct
		{
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};
		float m[4][4];
		float elements[16];
	};

public:

	static const Matrix Identity;	///< �P�ʍs��

public:

	/**
		@brief	�P�ʍs���ݒ肵�ăC���X�^���X�����������܂��B
	*/
	Matrix();

	/**
		@brief	�e�v�f���w�肵�ăC���X�^���X�����������܂��B
	*/
	Matrix(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);

	/**
		@brief	�e����w�肵�ăC���X�^���X�����������܂��B
	*/
	Matrix(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);

public:

	/**
		@brief	�e�v�f��ݒ肵�܂��B
	*/
	void Set(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);

	/**
		@brief		���̍s�񂪒P�ʍs��ł��邩�𔻒肵�܂��B
		@return		true �̏ꍇ�A�P�ʍs��ł���
	*/
	bool IsIdentity() const;

	/**
		@brief		���̍s��̉E�����������x�N�g�����擾���܂��B
	*/
	const Vector3& GetRight() const;

	/**
		@brief		���̍s��̏�����������x�N�g�����擾���܂��B
	*/
	const Vector3& GetUp() const;

	/**
		@brief		���̍s��̐��ʕ����������x�N�g�����擾���܂��B
	*/
	const Vector3& GetFront() const;

	/**
		@brief		���̍s��̕��s�ړ��ʂ������x�N�g�����擾���܂��B
	*/
	const Vector3& GetPosition() const;

	/**
		@brief		���̍s��̍s�v�f��ݒ肵�܂��B
	*/
	void SetRow(int index, const Vector4& row);

	/**
		@brief		���̍s��̍s�v�f���擾���܂��B
	*/
	const Vector4& GetRow(int index) const;

	/**
		@brief		���̍s��𕽍s�ړ����܂��B
		@param[in]	x		: X ���̈ړ���
		@param[in]	y		: Y ���̈ړ���
		@param[in]	z		: Z ���̈ړ���
	*/
	void Translate(float x, float y, float z);

	/**
		@brief		���̍s��𕽍s�ړ����܂��B
		@param[in]	vec		: �ړ���
	*/
	void Translate(const Vector3& vec);

	/**
		@brief		���̍s��� X ���ŉ�]���܂��B
		@param[in]	r		: ��]�p�x (���W�A���P��)
	*/
	void RotateX(float r);

	/**
		@brief		���̍s��� Y ���ŉ�]���܂��B
		@param[in]	r		: ��]�p�x (���W�A���P��)
	*/
	void RotateY(float r);

	/**
		@brief		���̍s��� Z ���ŉ�]���܂��B
		@param[in]	r		: ��]�p�x (���W�A���P��)
	*/
	void RotateZ(float r);

	/**
		@brief		���̍s��� XYZ �e���̃I�C���[�p���w�肵�ĉ�]���܂��B
		@param[in]  x		: X ���̉�]�p�x (���W�A���P��)
		@param[in]  y		: Y ���̉�]�p�x (���W�A���P��)
		@param[in]  z		: Z ���̉�]�p�x (���W�A���P��)
		@param[in]  order	: ��]����
		@return		���Z���ʂ̍s��
	*/
	void RotateEulerAngles(float x, float y, float z, RotationOrder order = RotationOrder::ZXY);

	/**
		@brief		���̍s��� XYZ �e���̃I�C���[�p���w�肵�ĉ�]���܂��B
		@param[in]  angles	: XYZ �e���̉�]�p�x (���W�A���P��)
		@param[in]  order	: ��]����
		@return		���Z���ʂ̍s��
	*/
	void RotateEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);

	/**
		@brief		���̍s���C�ӎ��ŉ�]���܂��B
		@param[in]  axis	: ��]��
		@param[in]  r		: ��]�p�x (���W�A���P��)
		@details	axis ���P�ʃx�N�g���łȂ��ꍇ�͐��K�����Ă���v�Z���s���܂��B
	*/
	void RotateAxis(const Vector3& axis, float r);

	/**
		@brief		���̍s����w�肵���N�H�[�^�j�I���ŉ�]���܂��B
		@param[in]  qua		: �����̊�ɂȂ�N�H�[�^�j�I��
	*/
	void RotateQuaternion(const Quaternion& qua);

	/**
		@brief		���̍s����e���ɂ����ăX�P�[�����O���܂��B
		@param[in]	x		: X ���̊g�k��
		@param[in]	y		: Y ���̊g�k��
		@param[in]	z		: Z ���̊g�k��
	*/
	void Scale(float x, float y, float z);

	/**
		@brief		���̍s����e���ɂ����ăX�P�[�����O���܂��B
		@param[in]	vec		: �e���̊g�k��
	*/
	void Scale(const Vector3& vec);

	/**
		@brief		���̍s����e���ɂ����ăX�P�[�����O���܂��B
		@param[in]	xyz		: �S�Ă̎��̊g�k��
	*/
	void Scale(float xyz);

	/**
		@brief		���̍s����t�s�񉻂��܂��B
	*/
	void Inverse();

	/**
		@brief		���̍s���]�u���܂��B
	*/
	void Transpose();

	/**
		@brief		���̍s����X�P�[�����O�A��]�A�ړ��v�f�ɕ������܂��B
		@param[in]	scale		: �X�P�[�����O�v�f���i�[����x�N�g���̃|�C���^
		@param[in]	rotation	: ��]�v�f���i�[����N�H�[�^�j�I���̃|�C���^
		@param[in]	translation	: ���s�ړ��v�f���i�[����x�N�g���̃|�C���^
		@details	�e�����͕K�v��������� NULL ���w��ł��܂��B
	*/
	void Decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

	/**
		@brief		���̍s����X�P�[�����O�A��]�A�ړ��s��ɕ������܂��B
		@param[in]	scale		: �X�P�[�����O�s����i�[����ϐ��̃|�C���^
		@param[in]	rotation	: ��]�s����i�[����ϐ��̃|�C���^
		@param[in]	translation	: ���s�ړ��s����i�[����ϐ��̃|�C���^
		@details	�e�����͕K�v��������� NULL ���w��ł��܂��B
	*/
	void DecomposeMatrices(Matrix* scale, Matrix* rotation, Matrix* translation) const;

	/**
		@brief		���̉�]�s��� XYZ �e���̃I�C���[�p�x�ɕϊ����܂��B
		@param[in]  order		: ��]����
		@param[in]  locked		: �W���o�����b�N�̔����L�����i�[����ϐ��ւ̃|�C���^ (�W���o�����b�N�����������ꍇ true)
		@return		XYZ �e���̃I�C���[�p�x (���W�A���P��) ���i�[�����x�N�g��
		@details	��]�s��� order �Ɏw�肳�ꂽ�����ŏ�Z���ꂽ���̂łȂ���Ζ{���̉�]�p�x���擾�ł��Ȃ����Ƃɒ��ӂ��Ă��������B<br>
					�Ⴆ�� RotationYawPitchRoll() �́A��]�� Z �� Y �� X �̏��ɉ�]���s���܂����A
					���ꂩ�猳�̊p�x�����o���ɂ� RotationOrder_ZXY ���w�肵�܂��B
	*/
	Vector3 ToEulerAngles(RotationOrder order = RotationOrder::ZXY, bool* locked = nullptr) const;

	/**
		@brief		���̍s��̍s�v�����]�s������o���܂��B
		@details	�����ł͂Ȃ��A�P����3x3�̕������R�s�[���܂��B
					����ȊO�̗v�f�͒P�ʍs��̒l�ŏ���������܂��B
	*/
	Matrix GetRotationMatrix() const;

	/**
		@brief		�v�f�̂����ꂩ�� NaN �܂��� Inf ���𔻕ʂ��܂��B
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "%f, %f, %f, %f\n %f, %f, %f, %f\n %f, %f, %f, %f\n %f, %f, %f, %f\n" ���g�p���܂��B
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

public:

	/**
		@brief		2�̃N�s��̐ς��v�Z���܂��B
		@param[in]	mat1	: �����̊�ɂȂ�s��
		@param[in]	mat2	: �����̊�ɂȂ�s��
		@return		���Z���ʂ̍s��
	*/
	static Matrix Multiply(const Matrix& mat1, const Matrix& mat2);

	/**
		@brief		���s�ړ��s����쐬���܂��B
		@param[in]	x		: X ���̈ړ���
		@param[in]	y		: Y ���̈ړ���
		@param[in]	z		: Z ���̈ړ���
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeTranslation(float x, float y, float z);

	/**
		@brief		���s�ړ��s����쐬���܂��B
		@param[in]	vec		: �ړ���
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeTranslation(const Vector3& vec);

	/**
		@brief		X ������]���ɂ��ĉ�]����s����쐬���܂��B
		@param[in]	r		: ��]�p�x (���W�A���P��)
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeRotationX(float r);

	/**
		@brief		Y ������]���ɂ��ĉ�]����s����쐬���܂��B
		@param[in]	r		: ��]�p�x (���W�A���P��)
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeRotationY(float r);

	/**
		@brief		Z ������]���ɂ��ĉ�]����s����쐬���܂��B
		@param[in]	r		: ��]�p�x (���W�A���P��)
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeRotationZ(float r);

	/**
		@brief		��]���Ɗp�x���w�肵�čs����쐬���܂��B
		@param[in]  axis	: ��]��
		@param[in]  r		: ��]�p�x (���W�A���P��)
		@return		���Z���ʂ̍s��
		@details	axis ���P�ʃx�N�g���łȂ��ꍇ�͐��K�����Ă���v�Z���s���܂��B
	*/
	static Matrix MakeRotationAxis(const Vector3& axis, float r);

	/**
		@brief		�N�H�[�^�j�I�������]�s����쐬���܂��B
		@param[in]  qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeRotationQuaternion(const Quaternion& qua);

	/**
		@brief		XYZ �e���̃I�C���[�p�����]�s����쐬���܂��B
		@param[in]  x		: X ���̉�]�p�x (���W�A���P��)
		@param[in]  y		: Y ���̉�]�p�x (���W�A���P��)
		@param[in]  z		: Z ���̉�]�p�x (���W�A���P��)
		@param[in]  order	: ��]����
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeRotationEulerAngles(float x, float y, float z, RotationOrder order = RotationOrder::ZXY);

	/**
		@brief		XYZ �e���̃I�C���[�p�����]�s����쐬���܂��B
		@param[in]  angles	: XYZ �e���̉�]�p�x (���W�A���P��)
		@param[in]  order	: ��]����
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeRotationEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);

	/**
		@brief		���[�A�s�b�`�A���[�����w�肵�čs����쐬���܂��B
		@param[in]	yaw		: y ���𒆐S�Ƃ��郈�[ (���W�A���P��)
		@param[in]	pitch	: x ���𒆐S�Ƃ���s�b�` (���W�A���P��)
		@param[in]	roll	: z ���𒆐S�Ƃ��郍�[�� (���W�A���P��)
		@return		���Z���ʂ̍s��
		@details	��]������ Z �� X �� Y �ł��B�����̕��тƂ͋t���ł��邱�Ƃɒ��ӂ��Ă��������B
					�����  D3DXMatrixRotationYawPitchRoll �̉�]�����Ɠ����ŁA
					RotationEulerAngles �� RotationOrder_ZXY ���w�肵�Čv�Z�������ʂƂ������ł��B
	*/
	static Matrix MakeRotationYawPitchRoll(float yaw, float pitch, float roll);

	/**
		@brief		�e���ɂ����ăX�P�[�����O����s����쐬���܂��B
		@param[in]	x		: X ���̊g�k��
		@param[in]	y		: Y ���̊g�k��
		@param[in]	z		: Z ���̊g�k��
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeScaling(float x, float y, float z);

	/**
		@brief		�e���ɂ����ăX�P�[�����O����s����쐬���܂��B
		@param[in]	vec		: �e���̊g�k��
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeScaling(const Vector3& vec);

	/**
		@brief		�e���ɂ����ăX�P�[�����O����s����쐬���܂��B
		@param[in]	xyz		: �S�Ă̎��̊g�k��
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeScaling(float xyz);

	/**
		@brief		�w�肳�ꂽ�s��̋t�s����쐬���܂��B
		@param[in]  mat		: �����̊�ɂȂ�s��
		@return		�쐬���ꂽ�t�s��
	*/
	static Matrix MakeInverse(const Matrix& mat);

	/**
		@brief		�w�肳�ꂽ�s��̓]�u�s����쐬���܂��B
		@param[in]  mat		: �����̊�ɂȂ�s��
		@return		�쐬���ꂽ�]�u�s��
	*/
	static Matrix MakeTranspose(const Matrix& mat);

	/**
		@brief		���ʂɑ΂��č��W�n�𔽓]�����s����쐬���܂��B
		@param[in]  mat			: �����̊�ɂȂ�s��
		@param[in]  plane		: �����̊�ɂȂ镽��
		@return		���Z���ʂ̍s��
		@details	plane �͐��K�����Ă���v�Z���s���܂��B
	*/
	static Matrix MakeReflection(const Plane& plane);

	/**
		@brief		������W�n�r���[�s����쐬���܂��B
		@param[in]	position	: ���_�̈ʒu
		@param[in]	lookAt		: �����_
		@param[in]	up			: �����
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeLookAtLH(const Vector3& position, const Vector3& lookAt, const Vector3& up);

	/**
		@brief		�E����W�n�r���[�s����쐬���܂��B
		@param[in]	position	: ���_�̈ʒu
		@param[in]	lookAt		: �����_
		@param[in]	up			: �����
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeLookAtRH(const Vector3& position, const Vector3& lookAt, const Vector3& up);

	/**
		@brief		������W�n�p�[�X�y�N�e�B�u�ˉe�s����쐬���܂��B
		@param[in]  fovY		: Y �����ւ̎���p (���W�A���P��)
		@param[in]  aspect		: �A�X�y�N�g�� (����/����)
		@param[in]  near		: �߂��r���[���ʂ� Z �l
		@param[in]  far			: �����r���[���ʂ� Z �l
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakePerspectiveFovLH(float fovY, float aspect, float near, float far);

	/**
		@brief		�E����W�n�p�[�X�y�N�e�B�u�ˉe�s����쐬���܂��B
		@param[in]  fovY		: Y �����ւ̎���p (���W�A���P��)
		@param[in]  aspect		: �A�X�y�N�g�� (����/����)
		@param[in]  near		: �߂��r���[���ʂ� Z �l
		@param[in]  far			: �����r���[���ʂ� Z �l
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakePerspectiveFovRH(float fovY, float aspect, float near, float far);

	/**
		@brief		������W�n���ˉe�s����쐬���܂��B
		@param[in]  width		: �X�N���[���̕�
		@param[in]  height		: �X�N���[���̍���
		@param[in]  near		: ��ԋ߂��r���[���ʂ� Z �l
		@param[in]  far			: ��ԉ����r���[���ʂ� Z �l
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeOrthoLH(float width, float height, float near, float far);

	/**
		@brief		�E����W�n���ˉe�s����쐬���܂��B
		@param[in]  width		: �X�N���[���̕�
		@param[in]  height		: �X�N���[���̍���
		@param[in]  near		: ��ԋ߂��r���[���ʂ� Z �l
		@param[in]  far			: ��ԉ����r���[���ʂ� Z �l
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeOrthoRH(float width, float height, float near, float far);

	/**
		@brief		2D ���W�n�p�̍�����W�n���ˉe�s����쐬���܂��B
		@param[in]  width		: �X�N���[���̕�
		@param[in]  height		: �X�N���[���̍���
		@param[in]  near		: ��ԋ߂��r���[���ʂ� Z �l
		@param[in]  far			: ��ԉ����r���[���ʂ� Z �l
		@return		���Z���ʂ̍s��
		@details	�r���[�̍�������_�Ƃ��A�E������ X+�A�������� Y+�A��O������ Z+ �Ƃ��܂��B
	*/
	static Matrix MakePerspective2DLH(float width, float height, float near, float far);

	/**
		@brief		2D ���W�n�p�̉E����W�n���ˉe�s����쐬���܂��B
		@param[in]  width		: �X�N���[���̕�
		@param[in]  height		: �X�N���[���̍���
		@param[in]  near		: ��ԋ߂��r���[���ʂ� Z �l
		@param[in]  far			: ��ԉ����r���[���ʂ� Z �l
		@return		���Z���ʂ̍s��
		@details	�r���[�̍�������_�Ƃ��A�E������ X+�A�������� Y+�A�������� Z+ �Ƃ��܂��B
	*/
	static Matrix MakePerspective2DRH(float width, float height, float near, float far);

	/**
		@brief		�A�t�B���ϊ��s����쐬���܂��B
		@param[in]  scaling			: �X�P�[�����O�W��
		@param[in]  rotationCenter	: ��]�̒��S�ʒu
		@param[in]  rotation		: ��]
		@param[in]  translation		: ���s�ړ�
		@return		���Z���ʂ̍s��
	*/
	static Matrix MakeAffineTransformation(const Vector3& scaling, const Vector3& rotationCenter, const Quaternion& rotation, const Vector3& translation);

public:

	Matrix& operator *= (const Matrix& mat);

	friend Matrix operator * (const Matrix& mat1, const Matrix& mat2);
	friend Matrix operator * (const Matrix& mat1, float v);

	bool operator == (const Matrix& mat) const;
	bool operator != (const Matrix& mat) const;

    Matrix(const MATRIX& mat)
    {
        memcpy(m, mat.m, sizeof(float) * 16);
    }

    operator MATRIX() const
    {
        MATRIX mat;
        memcpy(mat.m, m, sizeof(float) * 16);
        return mat;
    }

    operator bool() const
    {
        return !IsIdentity();
    }

};