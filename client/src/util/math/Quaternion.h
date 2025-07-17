#pragma once
#include <stdio.h>
#include "mathCommon.h"
#include "vector3.h"

class Vector3;
class Matrix;

/**
	@brief		�N�H�[�^�j�I�����`���܂��B

	@details	���̃N�H�[�^�j�I���N���X�̏�Z�̏��Ԃ͍�����E�ł��B
				�܂�Aq1 * q2 �́A��� q1�A���� q2 �̉�]��K�p���鎖�Ɠ����ł��B

				���̏�Z�����͕W���I�Ȏl�����̒�`����͋t�s���Ă��܂����A
				�Q�[���v���O���������C���Ƃ���3D�O���t�B�b�N�X�����̐��`�㐔���C�u�����Ƃ��Ă�
				���̓��삪�قڈ�ʓI�Ȃ̂ŁA����ɍ��킹�Ă��܂��B<br>
				(DirectX(D3DX)�AXNA�AUnity�Acocos2dx ���͂��̓���)
*/
class Quaternion
{
public:

	float	x;		///< X �v�f
	float	y;		///< Y �v�f
	float	z;		///< Z �v�f
	float	w;		///< W �v�f

public:

	static const Quaternion Identity;	///< �P�ʃN�H�[�^�j�I�� (0, 0, 0, 1)

public:

	/**
		@brief		�P�ʃN�H�[�^�j�I����ݒ肵�ăC���X�^���X�����������܂��B
	*/
	Quaternion();

	/**
		@brief		�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Quaternion(float x, float y, float z, float w);

	/**
		@brief		��]���Ɖ�]�p�x���w�肵�ăC���X�^���X�����������܂��B
		@param[in]  axis	: ��]��
		@param[in]  r		: ��]�p�x (���W�A���P��)
		@details	axis ���P�ʃx�N�g���łȂ���ΐ��K�����Ă���v�Z���s���܂��B
	*/
	Quaternion(const Vector3& axis, float r);

public:

	/**
		@brief		�e�v�f�ɒl��ݒ肵�܂��B
	*/
	void Set(float x, float y, float z, float w);

	/**
		@brief		���̃N�H�[�^�j�I���̒�����Ԃ��܂��B
	*/
	float GetLength() const;

	/**
		@brief		���̃N�H�[�^�j�I���̒����� 2 ���Ԃ��܂��B
	*/
	float GetLengthSquared() const;

	/**
		@brief		���̃N�H�[�^�j�I���𐳋K�����܂��B
	*/
	void Normalize();

	/**
		@brief		���̃N�H�[�^�j�I�����P�ʃN�H�[�^�j�I���ł��邩�𔻒肵�܂��B
		@return		true �̏ꍇ�A�P�ʃN�H�[�^�j�I���ł���
	*/
	bool IsIdentity() const;

	/**
		@brief		���̃N�H�[�^�j�I���Ɏw�肳�ꂽ�N�H�[�^�j�I������Z���܂��B
		@param[in]	qua		: ��Z����N�H�[�^�j�I��
	*/
	void Multiply(const Quaternion& qua);

	/**
		@brief		���̃N�H�[�^�j�I���� X ���ŉ�]���܂��B
		@param[in]	angle	: ��]�p�x (���W�A���P��)
	*/
	void RotateX(float angle);

	/**
		@brief		���̃N�H�[�^�j�I���� Y ���ŉ�]���܂��B
		@param[in]	angle	: ��]�p�x (���W�A���P��)
	*/
	void RotateY(float angle);

	/**
		@brief		���̃N�H�[�^�j�I���� Z ���ŉ�]���܂��B
		@param[in]	angle	: ��]�p�x (���W�A���P��)
	*/
	void RotateZ(float angle);

	/**
		@brief		���̃N�H�[�^�j�I������]���Ɗp�x���w�肵�ĉ�]���܂��B
		@param[in]  axis	: ��]��
		@param[in]  r		: ��]�p�x (���W�A���P��)
		@details	axis ���P�ʃx�N�g���łȂ���ΐ��K�����Ă���v�Z���s���܂��B
	*/
	void RotateAxis(const Vector3& axis, float r);

	/**
		@brief		���̃N�H�[�^�j�I���� XYZ �e���̃I�C���[�p�x�ɕϊ����܂��B
		@param[in]  order		: ��]����
		@param[in]  locked		: �W���o�����b�N�̔����L�����i�[����ϐ��ւ̃|�C���^ (�W���o�����b�N�����������ꍇ true)
		@return		XYZ �e���̃I�C���[�p�x (���W�A���P��) ���i�[�����x�N�g��
		@details	�N�H�[�^�j�I���� order �Ɏw�肳�ꂽ�����ŏ�Z���ꂽ���̂łȂ���Ζ{���̉�]�p�x���擾�ł��Ȃ����Ƃɒ��ӂ��Ă��������B<br>
					�Ⴆ�� RotationYawPitchRoll() �́A��]�� Z �� Y �� X �̏��ɉ�]���s���܂����A
					���ꂩ�猳�̊p�x�����o���ɂ� RotationOrder_ZXY ���w�肵�܂��B
	*/
	Vector3 ToEulerAngles(RotationOrder order = RotationOrder::ZXY, bool* locked = nullptr) const;

	/**
		@brief		���̃N�H�[�^�j�I���̉�]���Ɖ�]�p�x���擾���܂��B
		@param[in]	axis		: ��]�����i�[����x�N�g���̃|�C���^
		@param[in]	angle		: ��]�p�x���i�[����ϐ��̃|�C���^
		@details	axis �ɂ͒P�ʃx�N�g�����i�[���܂��B
	*/
	void ToAxisAngle(Vector3* axis, float* angle) const;

	/**
		@brief		�v�f�̂����ꂩ�� NaN �܂��� Inf ���𔻕ʂ��܂��B
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "(%f, %f, %f, %f)\n" ���g�p���܂��B
	*/
	void Print(const char* format = nullptr, FILE* stream = nullptr) const;

public:

	/**
		@brief		�w��N�H�[�^�j�I���𐳋K�������N�H�[�^�j�I����Ԃ��܂��B
		@param[in]	qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		���K�����ꂽ�N�H�[�^�j�I��
	*/
	static Quaternion Normalize(const Quaternion& qua);

	/**
		@brief		�w��N�H�[�^�j�I���̋�����Ԃ��܂��B
		@param[in]	qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		�����N�H�[�^�j�I��
	*/
	static Quaternion Conjugate(const Quaternion& qua);

	/**
		@brief		2�̃N�H�[�^�j�I���̓��ς��v�Z���܂��B
		@param[in]	qua1	: �����̊�ɂȂ�N�H�[�^�j�I��
		@param[in]	qua2	: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		2�̃N�H�[�^�j�I���̓���
	*/
	static float Dot(const Quaternion& qua1, const Quaternion& qua2);

	/**
		@brief		2�̃N�H�[�^�j�I���̐ς��v�Z���܂��B
		@param[in]	qua1	: �����̊�ɂȂ�N�H�[�^�j�I��
		@param[in]	qua2	: �����̊�ɂȂ�N�H�[�^�j�I��
		@details	Multiply(qua1, qua2) �́A��� qua1�A���� qua2 �̉�]���s���N�H�[�^�j�I�����쐬���܂��B
	*/
	static Quaternion Multiply(const Quaternion& qua1, const Quaternion& qua2);

	/**
		@brief		�N�H�[�^�j�I�����������A�Đ��K�����܂��B
		@param[in]	qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		�t���N�H�[�^�j�I��
	*/
	static Quaternion MakeInverse(const Quaternion& qua);

	/**
		@brief		��]���Ɗp�x���w�肵�ăN�H�[�^�j�I�����쐬���܂��B
		@param[in]  axis	: ��]��
		@param[in]  r		: ��]�p�x (���W�A���P��)
		@return		�V�����쐬���ꂽ�N�H�[�^�j�I��
		@details	axis ���P�ʃx�N�g���łȂ���ΐ��K�����Ă���v�Z���s���܂��B
	*/
	static Quaternion MakeFromRotationAxis(const Vector3& axis, float r);

	/**
		@brief		��]�s�񂩂�N�H�[�^�j�I�����쐬���܂��B
		@param[in]  mat		: �����̊�ɂȂ�s��
		@return		�V�����쐬���ꂽ�N�H�[�^�j�I��
	*/
	static Quaternion MakeFromRotationMatrix(const Matrix& mat);

	/**
		@brief		���[�A�s�b�`�A���[�����w�肵�ăN�H�[�^�j�I�����쐬���܂��B
		@param[in]	yaw		: y ���𒆐S�Ƃ��郈�[ (���W�A���P��)
		@param[in]	pitch	: x ���𒆐S�Ƃ���s�b�` (���W�A���P��)
		@param[in]	roll	: z ���𒆐S�Ƃ��郍�[�� (���W�A���P��)
		@return		�V�����쐬���ꂽ�N�H�[�^�j�I��
		@details	��]������ Z �� X �� Y �ł��B�����̕��тƂ͋t���ł��邱�Ƃɒ��ӂ��Ă��������B
					�����  D3DXQuaternionRotationYawPitchRoll �̉�]�����Ɠ����ŁA
					RotationEulerAngles �� RotationOrder_ZXY ���w�肵�Čv�Z�������ʂƂ������ł��B
	*/
	static Quaternion MakeFromYawPitchRoll(float yaw, float pitch, float roll);

	/**
		@brief		XYZ �e���̃I�C���[�p����N�H�[�^�j�I�����쐬���܂��B
		@param[in]  angles	: XYZ �e���̉�]�p�x (���W�A���P��)
		@param[in]  order	: ��]����
		@return		���Z���ʂ̃N�H�[�^�j�I��
	*/
	static Quaternion MakeFromEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);

	/**
		@brief		���ʐ��`��Ԃ��g�p���āA2 �̃N�I�[�^�j�I���Ԃ��Ԃ��܂��B
		@param[in]	qua1	: �����̊�ɂȂ�N�I�[�^�j�I�� (t = 0.0 �̂Ƃ��̒l)
		@param[in]	qua2	: �����̊�ɂȂ�N�I�[�^�j�I�� (t = 1.0 �̂Ƃ��̒l)
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃N�H�[�^�j�I��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Quaternion Slerp(const Quaternion& qua1, const Quaternion& qua2, float t);

	/**
		@brief		�w�肵���������������߂̉�]���v�Z���܂��B
		@param[in]	forward	: �I�u�W�F�N�g�������ׂ��O�������x�N�g���B
		@param[in]	up		: �I�u�W�F�N�g�̏�����������x�N�g���i�f�t�H���g��Y�������j�B
		@return		�w�肳�ꂽ�������������߂̃N�H�[�^�j�I��
		@details	forward��up�x�N�g�����g�p���āA�I�u�W�F�N�g���w�肳�ꂽ�������������߂̉�]���v�Z���܂��B
	*/
	static Quaternion LookRotation(const Vector3& forward, const Vector3& up = Vector3(0, 1, 0));

public:

	Quaternion& operator += (const Quaternion& v);
	Quaternion& operator += (float v);
	Quaternion& operator -= (const Quaternion& v);
	Quaternion& operator -= (float v);
	Quaternion& operator *= (const Quaternion& v);	///< this->Multiply(v); �Ɠ����ł��B
	Quaternion& operator *= (float v);
	Quaternion& operator /= (float v);

	friend Quaternion operator + (const Quaternion& v1, const Quaternion& v2);
	friend Quaternion operator + (const Quaternion& v1, float v2);
	friend Quaternion operator + (float v1, const Quaternion& v2);
	friend Quaternion operator - (const Quaternion& v1, const Quaternion& v2);
	friend Quaternion operator - (const Quaternion& v1, float v2);
	friend Quaternion operator - (float v1, const Quaternion& v2);
	friend Quaternion operator * (const Quaternion& v1, const Quaternion& v2);	///< Quaternion::Multiply(v1, v2); �Ɠ����ł��B
	friend Quaternion operator * (const Quaternion& v1, float v2);
	friend Quaternion operator * (float v1, const Quaternion& v2);
	friend Quaternion operator / (const Quaternion& v1, float v2);

	friend Quaternion operator - (const Quaternion& v1);

	bool operator == (const Quaternion& v) const;
	bool operator != (const Quaternion& v) const;

};