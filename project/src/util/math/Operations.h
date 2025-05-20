// ���v�Z���֘A��
#pragma once
#include "Quaternion.h"
#include "Transform.h"
#include "RectTransform.h"

namespace Operations {

	//======================================================================
	// 
	// �� Vector2��Vector3�̌v�Z
	//
	//======================================================================

	// Vector2��Vector3�̑����Z
	inline Vector2& operator +(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;

		return result;
	}

	// Vector2��Vector3�̑����Z���
	inline Vector2& operator +=(Vector2& v1, const Vector3& v2) {

		v1.x += v2.x;
		v1.y += v2.y;

		return v1;
	}

	// Vector2��Vector3�̈����Z
	inline Vector2& operator -(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;

		return result;
	}

	// Vector2��Vector3�̈����Z���
	inline Vector2& operator -=(Vector2& v1, const Vector3& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;

		return v1;
	}

	// Vector2��Vector3�̊|���Z
	inline Vector2& operator *(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;

		return result;
	}

	// Vector2��Vector3�̊|���Z���
	inline Vector2& operator *=(Vector2& v1, const Vector3& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;

		return v1;
	}

	// Vector2��Vector3�̊���Z
	inline Vector2& operator /(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;

		return result;
	}

	// Vector2��Vector3�̊���Z���
	inline Vector2& operator /=(Vector2& v1, const Vector3& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;

		return v1;
	}

	//======================================================================
	// 
	// �� Vector3��Vector2�̌v�Z
	//
	//======================================================================

	// Vector3��Vector2�̑����Z
	inline Vector3& operator +(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3��Vector2�̑����Z���
	inline Vector3& operator +=(Vector3& v1, const Vector2& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z;

		return v1;
	}

	// Vector3��Vector2�̈����Z
	inline Vector3& operator -(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3��Vector2�̈����Z���
	inline Vector3& operator -=(Vector3& v1, const Vector2& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z;

		return v1;
	}

	// Vector3��Vector2�̊|���Z
	inline Vector3& operator *(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3��Vector2�̊|���Z���
	inline Vector3& operator *=(Vector3& v1, const Vector2& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z;

		return v1;
	}

	// Vector3��Vector2�̊���Z
	inline Vector3& operator /(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3��Vector2�̊���Z���
	inline Vector3& operator /=(Vector3& v1, const Vector2& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z;

		return v1;
	}

	//======================================================================
	// 
	// �� Vector3��Quaternion�̌v�Z
	//
	//======================================================================

	// Vector3��Quaternion�̑����Z
	inline Vector3& operator +(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;

		return result;
	}

	// Vector3��Quaternion�̑����Z���
	inline Vector3& operator +=(Vector3& v1, const Quaternion& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;

		return v1;
	}

	// Vector3��Quaternion�̈����Z
	inline Vector3& operator -(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;

		return result;
	}

	// Vector3��Quaternion�̈����Z���
	inline Vector3& operator -=(Vector3& v1, const Quaternion& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;

		return v1;
	}

	// Vector3��Quaternion�̊|���Z
	inline Vector3& operator *(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;

		return result;
	}

	// Vector3��Quaternion�̊|���Z���
	inline Vector3& operator *=(Vector3& v1, const Quaternion& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;

		return v1;
	}

	// Vector3��Quaternion�̊���Z
	inline Vector3& operator /(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;

		return result;
	}

	// Vector3��Quaternion�̊���Z���
	inline Vector3& operator /=(Vector3& v1, const Quaternion& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;

		return v1;
	}

	//======================================================================
	// 
	// �� Quaternion��Vector3�̌v�Z
	//
	//======================================================================

	// Quaternion��Vector3�̑����Z
	inline Quaternion& operator +(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion��Vector3�̑����Z���
	inline Quaternion& operator +=(Quaternion& v1, const Vector3& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		v1.w;

		return v1;
	}

	// Quaternion��Vector3�̈����Z
	inline Quaternion& operator -(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion��Vector3�̈����Z���
	inline Quaternion& operator -=(Quaternion& v1, const Vector3& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		v1.w;

		return v1;
	}

	// Quaternion��Vector3�̊|���Z
	inline Quaternion& operator *(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion��Vector3�̊|���Z���
	inline Quaternion& operator *=(Quaternion& v1, const Vector3& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;
		v1.w;

		return v1;
	}

	// Quaternion��Vector3�̊���Z
	inline Quaternion& operator /(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion��Vector3�̊���Z���
	inline Quaternion& operator /=(Quaternion& v1, const Vector3& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;
		v1.w;

		return v1;
	}

	//======================================================================
	// 
	// �� Transform��RectTransform�̌v�Z
	//
	//======================================================================

	// Transform��RectTransform�̑����Z
	inline Transform& operator +(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position + rtrs.position;
		result.rotation = trs.rotation + V3::SetY(rtrs.rotation);
		result.scale = trs.scale + rtrs.scale;

		return result;
	}

	// Transform��RectTransform�̑����Z���
	inline Transform& operator +=(Transform& trs, const RectTransform& rtrs) {

		trs.position += rtrs.position;
		trs.rotation += V3::SetY(rtrs.rotation);
		trs.scale += rtrs.scale;

		return trs;
	}

	// Transform��RectTransform�̈����Z
	inline Transform& operator -(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position - rtrs.position;
		result.rotation = trs.rotation - V3::SetY(rtrs.rotation);
		result.scale = trs.scale - rtrs.scale;

		return result;
	}

	// Transform��RectTransform�̈����Z���
	inline Transform& operator -=(Transform& trs, const RectTransform& rtrs) {

		trs.position -= rtrs.position;
		trs.rotation -= V3::SetY(rtrs.rotation);
		trs.scale -= rtrs.scale;

		return trs;
	}

	// Transform��RectTransform�̊|���Z
	inline Transform& operator *(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position * rtrs.position;
		result.rotation = trs.rotation * Vector3(1.0f, rtrs.rotation, 1.0f);
		result.scale = trs.scale * rtrs.scale;

		return result;
	}

	// Transform��RectTransform�̊|���Z���
	inline Transform& operator *=(Transform& trs, const RectTransform& rtrs) {

		trs.position *= rtrs.position;
		trs.rotation *= Vector3(1.0f, rtrs.rotation, 1.0f);
		trs.scale *= rtrs.scale;

		return trs;
	}

	// Transform��RectTransform�̊���Z
	inline Transform& operator /(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position / rtrs.position;
		result.rotation = trs.rotation / Vector3(1.0f, rtrs.rotation, 1.0f);
		result.scale = trs.scale / rtrs.scale;

		return result;
	}

	// Transform��RectTransform�̊���Z���
	inline Transform& operator /=(Transform& trs, const RectTransform& rtrs) {

		trs.position /= rtrs.position;
		trs.rotation /= Vector3(1.0f, rtrs.rotation, 1.0f);
		trs.scale /= rtrs.scale;

		return trs;
	}

	//======================================================================
	// 
	// �� RectTransform��Transform�̌v�Z
	//
	//======================================================================

	// RectTransform��Transform�̑����Z
	inline RectTransform& operator +(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position + trs.position;
		result.rotation = rtrs.rotation + trs.rotation.y;
		result.scale = rtrs.scale + trs.scale;

		return result;
	}

	// Transform��RectTransform�̑����Z���
	inline RectTransform& operator +=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position += trs.position;
		rtrs.rotation += trs.rotation.y;
		rtrs.scale += trs.scale;

		return rtrs;
	}

	// RectTransform��Transform�̈����Z
	inline RectTransform& operator -(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position - trs.position;
		result.rotation = rtrs.rotation - trs.rotation.y;
		result.scale = rtrs.scale - trs.scale;

		return result;
	}

	// Transform��RectTransform�̈����Z���
	inline RectTransform& operator -=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position -= trs.position;
		rtrs.rotation -= trs.rotation.y;
		rtrs.scale -= trs.scale;

		return rtrs;
	}

	// RectTransform��Transform�̊|���Z
	inline RectTransform& operator *(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position * trs.position;
		result.rotation = rtrs.rotation * trs.rotation.y;
		result.scale = rtrs.scale * trs.scale;

		return result;
	}

	// Transform��RectTransform�̊|���Z���
	inline RectTransform& operator *=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position *= trs.position;
		rtrs.rotation *= trs.rotation.y;
		rtrs.scale *= trs.scale;

		return rtrs;
	}

	// RectTransform��Transform�̊���Z
	inline RectTransform& operator /(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position / trs.position;
		result.rotation = rtrs.rotation / trs.rotation.y;
		result.scale = rtrs.scale / trs.scale;

		return result;
	}

	// Transform��RectTransform�̊���Z���
	inline RectTransform& operator /=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position /= trs.position;
		rtrs.rotation /= trs.rotation.y;
		rtrs.scale /= trs.scale;

		return rtrs;
	}

	//======================================================================
	// 
	// �� Quaternion�̌v�Z
	//
	//======================================================================

	// �N�H�[�^�j�I�������]�s���
	MATRIX QuaternionToMatrix(const Quaternion& q) {

		MATRIX mat = MGetIdent();

		//X��
		mat.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
		mat.m[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
		mat.m[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;

		//Y��
		mat.m[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
		mat.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
		mat.m[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;

		//Z��
		mat.m[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
		mat.m[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		mat.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

		return mat;
	}

	// ��]�N�H�[�^�j�I��
	Quaternion CreateRotationQuaternion(float radian, VECTOR axis) {

		Quaternion ans;
		float norm;
		float ccc, sss;

		ans.w = ans.x = ans.y = ans.z = 0.0f;

		norm = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
		if (norm <= 0.0f) return ans;

		norm = 1.0f / sqrtf(norm);
		axis.x *= norm;
		axis.y *= norm;
		axis.z *= norm;

		ccc = cosf(0.5f * radian);
		sss = sinf(0.5f * radian);

		ans.w = ccc;
		ans.x = sss * axis.x;
		ans.y = sss * axis.y;
		ans.z = sss * axis.z;

		return ans;
	}

	// �ʒu�N�H�[�^�j�I��
	Quaternion CreateXYZToQuaternion(VECTOR pos) {

		Quaternion ans;

		ans.w = 0.0f;
		ans.x = pos.x;
		ans.y = pos.y;
		ans.z = pos.z;

		return ans;
	}

	// �ʒu�N�H�[�^�j�I��
	Quaternion CreateXYZToQuaternion(float posX, float posY, float posZ) {

		Quaternion ans;

		ans.w = 0.0f;
		ans.x = posX;
		ans.y = posY;
		ans.z = posZ;

		return ans;
	}

	// Transform����Quaternion�̕ϊ�
	Quaternion CreateTransformToQuaternion(const Quaternion& q, const Transform& trs) {

		Quaternion ans = q;

		ans.matScale = MGetScale(trs.scale);

		//���[��
		ans.zAxis = VGet(ans.matScale.m[2][0], ans.matScale.m[2][1], ans.matScale.m[2][2]);
		ans *= CreateRotationQuaternion(ans.roll, ans.zAxis);
		//�s�b�`
		ans.xAxis = VGet(ans.matScale.m[0][0], ans.matScale.m[0][1], ans.matScale.m[0][2]);
		ans *= CreateRotationQuaternion(ans.pitch, ans.xAxis);
		//���[
		ans.yAxis = VGet(ans.matScale.m[1][0], ans.matScale.m[1][1], ans.matScale.m[1][2]);
		ans *= CreateRotationQuaternion(ans.yaw, ans.yAxis);

		ans.matRot = QuaternionToMatrix(ans);
		ans.matTrans = MGetTranslate(trs.position);

		ans.mat = MMult(MMult(ans.matScale, ans.matRot), ans.matTrans);       //�ړ��~��]�~�X�P�[��

		return ans;
	}
}

using namespace Operations;