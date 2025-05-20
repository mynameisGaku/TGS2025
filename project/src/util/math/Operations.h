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
}

using namespace Operations;