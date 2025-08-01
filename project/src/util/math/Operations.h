// ◆計算式関連◆
#pragma once
#include "Quaternion.h"
#include "src/util/transform/Transform.h"
#include "src/util/transform/RectTransform.h"

namespace Operations {

	//======================================================================
	// 
	// ▼ Vector2とVector3の計算
	//
	//======================================================================

	// Vector2とVector3の足し算
	inline Vector2& operator +(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;

		return result;
	}

	// Vector2とVector3の足し算代入
	inline Vector2& operator +=(Vector2& v1, const Vector3& v2) {

		v1.x += v2.x;
		v1.y += v2.y;

		return v1;
	}

	// Vector2とVector3の引き算
	inline Vector2& operator -(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;

		return result;
	}

	// Vector2とVector3の引き算代入
	inline Vector2& operator -=(Vector2& v1, const Vector3& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;

		return v1;
	}

	// Vector2とVector3の掛け算
	inline Vector2& operator *(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;

		return result;
	}

	// Vector2とVector3の掛け算代入
	inline Vector2& operator *=(Vector2& v1, const Vector3& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;

		return v1;
	}

	// Vector2とVector3の割り算
	inline Vector2& operator /(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;

		return result;
	}

	// Vector2とVector3の割り算代入
	inline Vector2& operator /=(Vector2& v1, const Vector3& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;

		return v1;
	}

	//======================================================================
	// 
	// ▼ Vector3とVector2の計算
	//
	//======================================================================

	// Vector3とVector2の足し算
	inline Vector3& operator +(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3とVector2の足し算代入
	inline Vector3& operator +=(Vector3& v1, const Vector2& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z;

		return v1;
	}

	// Vector3とVector2の引き算
	inline Vector3& operator -(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3とVector2の引き算代入
	inline Vector3& operator -=(Vector3& v1, const Vector2& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z;

		return v1;
	}

	// Vector3とVector2の掛け算
	inline Vector3& operator *(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3とVector2の掛け算代入
	inline Vector3& operator *=(Vector3& v1, const Vector2& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z;

		return v1;
	}

	// Vector3とVector2の割り算
	inline Vector3& operator /(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3とVector2の割り算代入
	inline Vector3& operator /=(Vector3& v1, const Vector2& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z;

		return v1;
	}

	//======================================================================
	// 
	// ▼ Vector3とQuaternionの計算
	//
	//======================================================================

	// Vector3とQuaternionの足し算
	inline Vector3& operator +(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;

		return result;
	}

	// Vector3とQuaternionの足し算代入
	inline Vector3& operator +=(Vector3& v1, const Quaternion& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;

		return v1;
	}

	// Vector3とQuaternionの引き算
	inline Vector3& operator -(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;

		return result;
	}

	// Vector3とQuaternionの引き算代入
	inline Vector3& operator -=(Vector3& v1, const Quaternion& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;

		return v1;
	}

	// Vector3とQuaternionの掛け算
	inline Vector3& operator *(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;

		return result;
	}

	// Vector3とQuaternionの掛け算代入
	inline Vector3& operator *=(Vector3& v1, const Quaternion& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;

		return v1;
	}

	// Vector3とQuaternionの割り算
	inline Vector3& operator /(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;

		return result;
	}

	// Vector3とQuaternionの割り算代入
	inline Vector3& operator /=(Vector3& v1, const Quaternion& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;

		return v1;
	}

	//======================================================================
	// 
	// ▼ QuaternionとVector3の計算
	//
	//======================================================================

	// QuaternionとVector3の足し算
	inline Quaternion& operator +(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		result.w = v1.w;

		return result;
	}

	// QuaternionとVector3の足し算代入
	inline Quaternion& operator +=(Quaternion& v1, const Vector3& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		v1.w;

		return v1;
	}

	// QuaternionとVector3の引き算
	inline Quaternion& operator -(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		result.w = v1.w;

		return result;
	}

	// QuaternionとVector3の引き算代入
	inline Quaternion& operator -=(Quaternion& v1, const Vector3& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		v1.w;

		return v1;
	}

	// QuaternionとVector3の掛け算
	inline Quaternion& operator *(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		result.w = v1.w;

		return result;
	}

	// QuaternionとVector3の掛け算代入
	inline Quaternion& operator *=(Quaternion& v1, const Vector3& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;
		v1.w;

		return v1;
	}

	// QuaternionとVector3の割り算
	inline Quaternion& operator /(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		result.w = v1.w;

		return result;
	}

	// QuaternionとVector3の割り算代入
	inline Quaternion& operator /=(Quaternion& v1, const Vector3& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;
		v1.w;

		return v1;
	}

	//======================================================================
	// 
	// ▼ TransformとRectTransformの計算
	//
	//======================================================================

	// TransformとRectTransformの足し算
	inline Transform& operator +(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position + rtrs.position;
		result.rotation = trs.rotation + Vector3::SetY(rtrs.rotation);
		result.scale = trs.scale + rtrs.scale;

		return result;
	}

	// TransformとRectTransformの足し算代入
	inline Transform& operator +=(Transform& trs, const RectTransform& rtrs) {

		trs.position += rtrs.position;
		trs.rotation += Vector3::SetY(rtrs.rotation);
		trs.scale += rtrs.scale;

		return trs;
	}

	// TransformとRectTransformの引き算
	inline Transform& operator -(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position - rtrs.position;
		result.rotation = trs.rotation - Vector3::SetY(rtrs.rotation);
		result.scale = trs.scale - rtrs.scale;

		return result;
	}

	// TransformとRectTransformの引き算代入
	inline Transform& operator -=(Transform& trs, const RectTransform& rtrs) {

		trs.position -= rtrs.position;
		trs.rotation -= Vector3::SetY(rtrs.rotation);
		trs.scale -= rtrs.scale;

		return trs;
	}

	// TransformとRectTransformの掛け算
	inline Transform& operator *(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position * rtrs.position;
		result.rotation = trs.rotation * Vector3(1.0f, rtrs.rotation, 1.0f);
		result.scale = trs.scale * rtrs.scale;

		return result;
	}

	// TransformとRectTransformの掛け算代入
	inline Transform& operator *=(Transform& trs, const RectTransform& rtrs) {

		trs.position *= rtrs.position;
		trs.rotation *= Vector3(1.0f, rtrs.rotation, 1.0f);
		trs.scale *= rtrs.scale;

		return trs;
	}

	// TransformとRectTransformの割り算
	inline Transform& operator /(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position / rtrs.position;
		result.rotation = trs.rotation / Vector3(1.0f, rtrs.rotation, 1.0f);
		result.scale = trs.scale / rtrs.scale;

		return result;
	}

	// TransformとRectTransformの割り算代入
	inline Transform& operator /=(Transform& trs, const RectTransform& rtrs) {

		trs.position /= rtrs.position;
		trs.rotation /= Vector3(1.0f, rtrs.rotation, 1.0f);
		trs.scale /= rtrs.scale;

		return trs;
	}

	//======================================================================
	// 
	// ▼ RectTransformとTransformの計算
	//
	//======================================================================

	// RectTransformとTransformの足し算
	inline RectTransform& operator +(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position + trs.position;
		result.rotation = rtrs.rotation + trs.rotation.y;
		result.scale = rtrs.scale + trs.scale;

		return result;
	}

	// TransformとRectTransformの足し算代入
	inline RectTransform& operator +=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position += trs.position;
		rtrs.rotation += trs.rotation.y;
		rtrs.scale += trs.scale;

		return rtrs;
	}

	// RectTransformとTransformの引き算
	inline RectTransform& operator -(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position - trs.position;
		result.rotation = rtrs.rotation - trs.rotation.y;
		result.scale = rtrs.scale - trs.scale;

		return result;
	}

	// TransformとRectTransformの引き算代入
	inline RectTransform& operator -=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position -= trs.position;
		rtrs.rotation -= trs.rotation.y;
		rtrs.scale -= trs.scale;

		return rtrs;
	}

	// RectTransformとTransformの掛け算
	inline RectTransform& operator *(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position * trs.position;
		result.rotation = rtrs.rotation * trs.rotation.y;
		result.scale = rtrs.scale * trs.scale;

		return result;
	}

	// TransformとRectTransformの掛け算代入
	inline RectTransform& operator *=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position *= trs.position;
		rtrs.rotation *= trs.rotation.y;
		rtrs.scale *= trs.scale;

		return rtrs;
	}

	// RectTransformとTransformの割り算
	inline RectTransform& operator /(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position / trs.position;
		result.rotation = rtrs.rotation / trs.rotation.y;
		result.scale = rtrs.scale / trs.scale;

		return result;
	}

	// TransformとRectTransformの割り算代入
	inline RectTransform& operator /=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position /= trs.position;
		rtrs.rotation /= trs.rotation.y;
		rtrs.scale /= trs.scale;

		return rtrs;
	}
}

using namespace Operations;