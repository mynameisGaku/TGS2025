// ŸŒvZ®ŠÖ˜AŸ
#pragma once
#include "Quaternion.h"
#include "Transform.h"
#include "RectTransform.h"

namespace Operations {

	//======================================================================
	// 
	// ¥ Vector2‚ÆVector3‚ÌŒvZ
	//
	//======================================================================

	// Vector2‚ÆVector3‚Ì‘«‚µZ
	inline Vector2& operator +(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;

		return result;
	}

	// Vector2‚ÆVector3‚Ì‘«‚µZ‘ã“ü
	inline Vector2& operator +=(Vector2& v1, const Vector3& v2) {

		v1.x += v2.x;
		v1.y += v2.y;

		return v1;
	}

	// Vector2‚ÆVector3‚Ìˆø‚«Z
	inline Vector2& operator -(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;

		return result;
	}

	// Vector2‚ÆVector3‚Ìˆø‚«Z‘ã“ü
	inline Vector2& operator -=(Vector2& v1, const Vector3& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;

		return v1;
	}

	// Vector2‚ÆVector3‚ÌŠ|‚¯Z
	inline Vector2& operator *(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;

		return result;
	}

	// Vector2‚ÆVector3‚ÌŠ|‚¯Z‘ã“ü
	inline Vector2& operator *=(Vector2& v1, const Vector3& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;

		return v1;
	}

	// Vector2‚ÆVector3‚ÌŠ„‚èZ
	inline Vector2& operator /(const Vector2& v1, const Vector3& v2) {

		Vector2 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;

		return result;
	}

	// Vector2‚ÆVector3‚ÌŠ„‚èZ‘ã“ü
	inline Vector2& operator /=(Vector2& v1, const Vector3& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;

		return v1;
	}

	//======================================================================
	// 
	// ¥ Vector3‚ÆVector2‚ÌŒvZ
	//
	//======================================================================

	// Vector3‚ÆVector2‚Ì‘«‚µZ
	inline Vector3& operator +(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3‚ÆVector2‚Ì‘«‚µZ‘ã“ü
	inline Vector3& operator +=(Vector3& v1, const Vector2& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z;

		return v1;
	}

	// Vector3‚ÆVector2‚Ìˆø‚«Z
	inline Vector3& operator -(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3‚ÆVector2‚Ìˆø‚«Z‘ã“ü
	inline Vector3& operator -=(Vector3& v1, const Vector2& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z;

		return v1;
	}

	// Vector3‚ÆVector2‚ÌŠ|‚¯Z
	inline Vector3& operator *(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3‚ÆVector2‚ÌŠ|‚¯Z‘ã“ü
	inline Vector3& operator *=(Vector3& v1, const Vector2& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z;

		return v1;
	}

	// Vector3‚ÆVector2‚ÌŠ„‚èZ
	inline Vector3& operator /(const Vector3& v1, const Vector2& v2) {

		Vector3 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z;

		return result;
	}

	// Vector3‚ÆVector2‚ÌŠ„‚èZ‘ã“ü
	inline Vector3& operator /=(Vector3& v1, const Vector2& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z;

		return v1;
	}

	//======================================================================
	// 
	// ¥ Vector3‚ÆQuaternion‚ÌŒvZ
	//
	//======================================================================

	// Vector3‚ÆQuaternion‚Ì‘«‚µZ
	inline Vector3& operator +(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;

		return result;
	}

	// Vector3‚ÆQuaternion‚Ì‘«‚µZ‘ã“ü
	inline Vector3& operator +=(Vector3& v1, const Quaternion& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;

		return v1;
	}

	// Vector3‚ÆQuaternion‚Ìˆø‚«Z
	inline Vector3& operator -(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;

		return result;
	}

	// Vector3‚ÆQuaternion‚Ìˆø‚«Z‘ã“ü
	inline Vector3& operator -=(Vector3& v1, const Quaternion& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;

		return v1;
	}

	// Vector3‚ÆQuaternion‚ÌŠ|‚¯Z
	inline Vector3& operator *(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;

		return result;
	}

	// Vector3‚ÆQuaternion‚ÌŠ|‚¯Z‘ã“ü
	inline Vector3& operator *=(Vector3& v1, const Quaternion& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;

		return v1;
	}

	// Vector3‚ÆQuaternion‚ÌŠ„‚èZ
	inline Vector3& operator /(const Vector3& v1, const Quaternion& v2) {

		Vector3 result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;

		return result;
	}

	// Vector3‚ÆQuaternion‚ÌŠ„‚èZ‘ã“ü
	inline Vector3& operator /=(Vector3& v1, const Quaternion& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;

		return v1;
	}

	//======================================================================
	// 
	// ¥ Quaternion‚ÆVector3‚ÌŒvZ
	//
	//======================================================================

	// Quaternion‚ÆVector3‚Ì‘«‚µZ
	inline Quaternion& operator +(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion‚ÆVector3‚Ì‘«‚µZ‘ã“ü
	inline Quaternion& operator +=(Quaternion& v1, const Vector3& v2) {

		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		v1.w;

		return v1;
	}

	// Quaternion‚ÆVector3‚Ìˆø‚«Z
	inline Quaternion& operator -(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion‚ÆVector3‚Ìˆø‚«Z‘ã“ü
	inline Quaternion& operator -=(Quaternion& v1, const Vector3& v2) {

		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		v1.w;

		return v1;
	}

	// Quaternion‚ÆVector3‚ÌŠ|‚¯Z
	inline Quaternion& operator *(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion‚ÆVector3‚ÌŠ|‚¯Z‘ã“ü
	inline Quaternion& operator *=(Quaternion& v1, const Vector3& v2) {

		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;
		v1.w;

		return v1;
	}

	// Quaternion‚ÆVector3‚ÌŠ„‚èZ
	inline Quaternion& operator /(const Quaternion& v1, const Vector3& v2) {

		Quaternion result;

		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		result.w = v1.w;

		return result;
	}

	// Quaternion‚ÆVector3‚ÌŠ„‚èZ‘ã“ü
	inline Quaternion& operator /=(Quaternion& v1, const Vector3& v2) {

		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;
		v1.w;

		return v1;
	}

	//======================================================================
	// 
	// ¥ Transform‚ÆRectTransform‚ÌŒvZ
	//
	//======================================================================

	// Transform‚ÆRectTransform‚Ì‘«‚µZ
	inline Transform& operator +(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position + rtrs.position;
		result.rotation = trs.rotation + V3::SetY(rtrs.rotation);
		result.scale = trs.scale + rtrs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚Ì‘«‚µZ‘ã“ü
	inline Transform& operator +=(Transform& trs, const RectTransform& rtrs) {

		trs.position += rtrs.position;
		trs.rotation += V3::SetY(rtrs.rotation);
		trs.scale += rtrs.scale;

		return trs;
	}

	// Transform‚ÆRectTransform‚Ìˆø‚«Z
	inline Transform& operator -(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position - rtrs.position;
		result.rotation = trs.rotation - V3::SetY(rtrs.rotation);
		result.scale = trs.scale - rtrs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚Ìˆø‚«Z‘ã“ü
	inline Transform& operator -=(Transform& trs, const RectTransform& rtrs) {

		trs.position -= rtrs.position;
		trs.rotation -= V3::SetY(rtrs.rotation);
		trs.scale -= rtrs.scale;

		return trs;
	}

	// Transform‚ÆRectTransform‚ÌŠ|‚¯Z
	inline Transform& operator *(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position * rtrs.position;
		result.rotation = trs.rotation * Vector3(1.0f, rtrs.rotation, 1.0f);
		result.scale = trs.scale * rtrs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚ÌŠ|‚¯Z‘ã“ü
	inline Transform& operator *=(Transform& trs, const RectTransform& rtrs) {

		trs.position *= rtrs.position;
		trs.rotation *= Vector3(1.0f, rtrs.rotation, 1.0f);
		trs.scale *= rtrs.scale;

		return trs;
	}

	// Transform‚ÆRectTransform‚ÌŠ„‚èZ
	inline Transform& operator /(const Transform& trs, const RectTransform& rtrs) {

		Transform result;

		result.position = trs.position / rtrs.position;
		result.rotation = trs.rotation / Vector3(1.0f, rtrs.rotation, 1.0f);
		result.scale = trs.scale / rtrs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚ÌŠ„‚èZ‘ã“ü
	inline Transform& operator /=(Transform& trs, const RectTransform& rtrs) {

		trs.position /= rtrs.position;
		trs.rotation /= Vector3(1.0f, rtrs.rotation, 1.0f);
		trs.scale /= rtrs.scale;

		return trs;
	}

	//======================================================================
	// 
	// ¥ RectTransform‚ÆTransform‚ÌŒvZ
	//
	//======================================================================

	// RectTransform‚ÆTransform‚Ì‘«‚µZ
	inline RectTransform& operator +(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position + trs.position;
		result.rotation = rtrs.rotation + trs.rotation.y;
		result.scale = rtrs.scale + trs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚Ì‘«‚µZ‘ã“ü
	inline RectTransform& operator +=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position += trs.position;
		rtrs.rotation += trs.rotation.y;
		rtrs.scale += trs.scale;

		return rtrs;
	}

	// RectTransform‚ÆTransform‚Ìˆø‚«Z
	inline RectTransform& operator -(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position - trs.position;
		result.rotation = rtrs.rotation - trs.rotation.y;
		result.scale = rtrs.scale - trs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚Ìˆø‚«Z‘ã“ü
	inline RectTransform& operator -=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position -= trs.position;
		rtrs.rotation -= trs.rotation.y;
		rtrs.scale -= trs.scale;

		return rtrs;
	}

	// RectTransform‚ÆTransform‚ÌŠ|‚¯Z
	inline RectTransform& operator *(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position * trs.position;
		result.rotation = rtrs.rotation * trs.rotation.y;
		result.scale = rtrs.scale * trs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚ÌŠ|‚¯Z‘ã“ü
	inline RectTransform& operator *=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position *= trs.position;
		rtrs.rotation *= trs.rotation.y;
		rtrs.scale *= trs.scale;

		return rtrs;
	}

	// RectTransform‚ÆTransform‚ÌŠ„‚èZ
	inline RectTransform& operator /(const RectTransform& rtrs, const Transform& trs) {

		RectTransform result;

		result.position = rtrs.position / trs.position;
		result.rotation = rtrs.rotation / trs.rotation.y;
		result.scale = rtrs.scale / trs.scale;

		return result;
	}

	// Transform‚ÆRectTransform‚ÌŠ„‚èZ‘ã“ü
	inline RectTransform& operator /=(RectTransform& rtrs, const Transform& trs) {

		rtrs.position /= trs.position;
		rtrs.rotation /= trs.rotation.y;
		rtrs.scale /= trs.scale;

		return rtrs;
	}

	//======================================================================
	// 
	// ¥ Quaternion‚ÌŒvZ
	//
	//======================================================================

	// ƒNƒH[ƒ^ƒjƒIƒ“‚©‚ç‰ñ“]s—ñ‚Ö
	MATRIX QuaternionToMatrix(const Quaternion& q) {

		MATRIX mat = MGetIdent();

		//X²
		mat.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
		mat.m[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
		mat.m[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;

		//Y²
		mat.m[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
		mat.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
		mat.m[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;

		//Z²
		mat.m[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
		mat.m[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		mat.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

		return mat;
	}

	// ‰ñ“]ƒNƒH[ƒ^ƒjƒIƒ“
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

	// ˆÊ’uƒNƒH[ƒ^ƒjƒIƒ“
	Quaternion CreateXYZToQuaternion(VECTOR pos) {

		Quaternion ans;

		ans.w = 0.0f;
		ans.x = pos.x;
		ans.y = pos.y;
		ans.z = pos.z;

		return ans;
	}

	// ˆÊ’uƒNƒH[ƒ^ƒjƒIƒ“
	Quaternion CreateXYZToQuaternion(float posX, float posY, float posZ) {

		Quaternion ans;

		ans.w = 0.0f;
		ans.x = posX;
		ans.y = posY;
		ans.z = posZ;

		return ans;
	}

	// Transform‚©‚çQuaternion‚Ì•ÏŠ·
	Quaternion CreateTransformToQuaternion(const Quaternion& q, const Transform& trs) {

		Quaternion ans = q;

		ans.matScale = MGetScale(trs.scale);

		//ƒ[ƒ‹
		ans.zAxis = VGet(ans.matScale.m[2][0], ans.matScale.m[2][1], ans.matScale.m[2][2]);
		ans *= CreateRotationQuaternion(ans.roll, ans.zAxis);
		//ƒsƒbƒ`
		ans.xAxis = VGet(ans.matScale.m[0][0], ans.matScale.m[0][1], ans.matScale.m[0][2]);
		ans *= CreateRotationQuaternion(ans.pitch, ans.xAxis);
		//ƒˆ[
		ans.yAxis = VGet(ans.matScale.m[1][0], ans.matScale.m[1][1], ans.matScale.m[1][2]);
		ans *= CreateRotationQuaternion(ans.yaw, ans.yAxis);

		ans.matRot = QuaternionToMatrix(ans);
		ans.matTrans = MGetTranslate(trs.position);

		ans.mat = MMult(MMult(ans.matScale, ans.matRot), ans.matTrans);       //ˆÚ“®~‰ñ“]~ƒXƒP[ƒ‹

		return ans;
	}
}

using namespace Operations;