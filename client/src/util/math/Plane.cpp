
#include <math.h>
#include "vector3.h"
#include "vector4.h"
#include "matrix.h"
#include "plane.h"
#include "Asm.h"

//==============================================================================
// Plane
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool Plane::CheckInside(const Vector3& point) const
{
	return (Plane::DotCoord(*this, point) < 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool Plane::CheckInsideLower(const Vector3& point) const
{
	return (Plane::DotCoord(*this, point) <= 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool Plane::CheckInside(const Vector3& point, float radius) const
{
	return (Plane::DotCoord(*this, point) - radius < 0.0f);
}

//------------------------------------------------------------------------------
Plane::Plane()
	: Normal(0.0f, 0.0f, 0.0f)
	, D(0.0f)
{
}

//------------------------------------------------------------------------------
Plane::Plane(float a, float b, float c, float d)
	: Normal(a, b, c)
	, D(d)
{
}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& normal, float d)
	: Normal(normal)
	, D(d)
{
}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& p1, const Vector3& normal)
	: Normal(normal)
	, D(-Vector3::Dot(normal, p1))
{
}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
{
	Normal = Vector3::Cross((point2 - point1), (point3 - point1));
	Normal.Normalize();

	D = -Vector3::Dot(point1, Normal);
}

//------------------------------------------------------------------------------
void Plane::Normalize()
{
	float t = 1.0f / Asm::sqrt((Normal.x * Normal.x) + (Normal.y * Normal.y) + (Normal.z * Normal.z));
	Normal.x *= t;
	Normal.y *= t;
	Normal.z *= t;
	D *= t;
}

//------------------------------------------------------------------------------
bool Plane::Intersects(const Vector3& start, const Vector3& end, Vector3* point) const
{
	Vector3 p = Vector3(Normal.x * D, Normal.y * D, Normal.z * D);
	Vector3 pa = p - start;
	Vector3 pb = p - end;
	float dot_pa = Vector3::Dot(pa, Normal);
	float dot_pb = Vector3::Dot(pb, Normal);

	// ��������
	if (dot_pa == 0.0 && dot_pb == 0.0)
	{
		// ���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
		return false;
	}
	else if (
		(dot_pa >= 0.0 && dot_pb <= 0.0) ||
		(dot_pa <= 0.0 && dot_pb >= 0.0))
	{
		// ���ς̕Е����v���X�ŕЕ����}�C�i�X�Ȃ̂ŁA�������Ă���
	}
	else
	{
		// �������Ă��Ȃ�
		return false;
	}

	if (point != nullptr)
	{
		Vector3 direction = end - start;
		float dot = Vector3::Dot(Normal, direction);
		float t = (D + Vector3::Dot(Normal, start)) / dot;	// ��_��A�̋��� : ��_��B�̋���
		(*point) = start - (t * direction);
	}
	return true;
}

//------------------------------------------------------------------------------
void Plane::Transform(const Matrix& mat)
{
	float x = Normal.x;
	float y = Normal.y;
	float z = Normal.z;
	float d = D;
	Matrix t = Matrix::MakeInverse(mat);

	Normal.x = (((x * t.m11) + (y * t.m12)) + (z * t.m13)) + (d * t.m14);
	Normal.y = (((x * t.m21) + (y * t.m22)) + (z * t.m23)) + (d * t.m24);
	Normal.z = (((x * t.m31) + (y * t.m32)) + (z * t.m33)) + (d * t.m34);
	D = (((x * t.m41) + (y * t.m42)) + (z * t.m43)) + (d * t.m44);
}

//------------------------------------------------------------------------------
void Plane::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, Normal.x, Normal.y, Normal.z, D);
}

//------------------------------------------------------------------------------
Plane Plane::Normalize(const Plane& plane)
{
	float t = 1.0f / Asm::sqrt((plane.Normal.x * plane.Normal.x) + (plane.Normal.y * plane.Normal.y) + (plane.Normal.z * plane.Normal.z));
	return Plane(
		plane.Normal.x * t,
		plane.Normal.y * t,
		plane.Normal.z * t,
		plane.D * t);
}

//------------------------------------------------------------------------------
float Plane::Dot(const Plane& plane, const Vector4& vec)
{
	return (plane.Normal.x * vec.x) + (plane.Normal.y * vec.y) + (plane.Normal.z * vec.z) + (plane.D * vec.w);
}

//------------------------------------------------------------------------------
float Plane::DotCoord(const Plane& plane, const Vector3& vec)
{
	return (plane.Normal.x * vec.x) + (plane.Normal.y * vec.y) + (plane.Normal.z * vec.z) + plane.D;
}

//------------------------------------------------------------------------------
float Plane::DotNormal(const Plane& plane, const Vector3& vec)
{
	return (plane.Normal.x * vec.x) + (plane.Normal.y * vec.y) + (plane.Normal.z * vec.z);
}

//------------------------------------------------------------------------------
Plane Plane::Transform(const Plane& plane, const Matrix& mat)
{
	float x = plane.Normal.x;
	float y = plane.Normal.y;
	float z = plane.Normal.z;
	float d = plane.D;
	Matrix t = Matrix::MakeInverse(mat);

	return Plane(
		(((x * t.m11) + (y * t.m12)) + (z * t.m13)) + (d * t.m14),
		(((x * t.m21) + (y * t.m22)) + (z * t.m23)) + (d * t.m24),
		(((x * t.m31) + (y * t.m32)) + (z * t.m33)) + (d * t.m34),
		(((x * t.m41) + (y * t.m42)) + (z * t.m43)) + (d * t.m44));
}
