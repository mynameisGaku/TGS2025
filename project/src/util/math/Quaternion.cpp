#include "Quaternion.h"
#include "Transform.h"

Quaternion::Quaternion() : Quaternion(0.0f, 0.0f, 0.0f, 1.0f) {}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) : Quaternion(VGet(_x, _y, _z), _w) {}

Quaternion::Quaternion(VECTOR vec, float _w) {

	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = _w;

	yaw = pitch = roll = 0.0f;
	xAxis = yAxis = zAxis = VGet(0.0f, 0.0f, 0.0f);
	mat = matTrans = matRot = matScale = MGetIdent();
}

Quaternion::~Quaternion() {

}

Quaternion::operator const VECTOR() {

	return VTransform(VGet(x, y, z), ToMatrix());
}

Quaternion::operator bool() const {

	return (VSize(VGet(x, y, z)) != 0);
}

void Quaternion::operator=(const VECTOR& value) {

	x = value.x;
	y = value.y;
	z = value.z;
}

Quaternion Quaternion::operator+(const VECTOR& value) {

	Quaternion ans;

	ans.x = x + value.x;
	ans.y = y + value.y;
	ans.z = z + value.z;
	ans.w = w;

	return ans;
}

Quaternion Quaternion::operator-(const VECTOR& value) {

	Quaternion ans;

	ans.x = x - value.x;
	ans.y = y - value.y;
	ans.z = z - value.z;
	ans.w = w;

	return ans;
}

Quaternion Quaternion::operator*(const VECTOR& value) {

	Quaternion ans;

	ans.x = x * value.x;
	ans.y = y * value.y;
	ans.z = z * value.z;
	ans.w = w;

	return ans;
}

Quaternion Quaternion::operator/(const VECTOR& value) {

	Quaternion ans;

	ans.x = x / value.x;
	ans.y = y / value.y;
	ans.z = z / value.z;
	ans.w = w;

	return ans;
}

void Quaternion::operator+=(const VECTOR& value) {

	x += value.x;
	y += value.y;
	z += value.z;
	w;
}

void Quaternion::operator-=(const VECTOR& value) {

	x -= value.x;
	y -= value.y;
	z -= value.z;
	w;
}

void Quaternion::operator*=(const VECTOR& value) {

	x *= value.x;
	y *= value.y;
	z *= value.z;
	w;
}

void Quaternion::operator/=(const VECTOR& value) {

	x /= value.x;
	y /= value.y;
	z /= value.z;
	w;
}

Quaternion Quaternion::operator*(const Quaternion& q) {

	Quaternion ans;
	float d1, d2, d3, d4;

	//tパラメータの計算 
	d1 = this->w * q.w;
	d2 = -this->x * q.x;
	d3 = -this->y * q.y;
	d4 = -this->z * q.z;
	ans.w = d1 + d2 + d3 + d4;

	//xパラメータの計算 
	d1 = this->w * q.x;
	d2 = q.w * this->x;
	d3 = this->y * q.z;
	d4 = -this->z * q.y;
	ans.x = d1 + d2 + d3 + d4;

	//yパラメータの計算
	d1 = this->w * q.y;
	d2 = q.w * this->y;
	d3 = this->z * q.x;
	d4 = -this->x * q.z;
	ans.y = d1 + d2 + d3 + d4;

	//zパラメータの計算
	d1 = this->w * q.z;
	d2 = q.w * this->z;
	d3 = this->x * q.y;
	d4 = -this->y * q.x;
	ans.z = d1 + d2 + d3 + d4;

	return ans;
}

void Quaternion::operator*=(const Quaternion& q) {

	Quaternion ans;
	float d1, d2, d3, d4;

	//tパラメータの計算 
	d1 = this->w * q.w;
	d2 = -this->x * q.x;
	d3 = -this->y * q.y;
	d4 = -this->z * q.z;
	ans.w = d1 + d2 + d3 + d4;

	//xパラメータの計算 
	d1 = this->w * q.x;
	d2 = q.w * this->x;
	d3 = this->y * q.z;
	d4 = -this->z * q.y;
	ans.x = d1 + d2 + d3 + d4;

	//yパラメータの計算
	d1 = this->w * q.y;
	d2 = q.w * this->y;
	d3 = this->z * q.x;
	d4 = -this->x * q.z;
	ans.y = d1 + d2 + d3 + d4;

	//zパラメータの計算
	d1 = this->w * q.z;
	d2 = q.w * this->z;
	d3 = this->x * q.y;
	d4 = -this->y * q.x;
	ans.z = d1 + d2 + d3 + d4;

	*this = ans;
}

MATRIX Quaternion::ToMatrix() {

	MATRIX mat = MGetIdent();

	//X軸
	mat.m[0][0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	mat.m[0][1] = 2.0f * x * y + 2.0f * w * z;
	mat.m[0][2] = 2.0f * x * z - 2.0f * w * y;

	//Y軸
	mat.m[1][0] = 2.0f * x * y - 2.0f * w * z;
	mat.m[1][1] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	mat.m[1][2] = 2.0f * y * z + 2.0f * w * x;

	//Z軸
	mat.m[2][0] = 2.0f * x * z + 2.0f * w * y;
	mat.m[2][1] = 2.0f * y * z - 2.0f * w * x;
	mat.m[2][2] = 1.0f - 2.0f * x * x - 2.0f * y * y;

	return mat;
}
