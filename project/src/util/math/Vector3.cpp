#include "Vector3.h"
#include "Utils.h"

void Vector3::operator=(const Vector3& value) {

	x = value.x;
	y = value.y;
	z = value.z;
}

void Vector3::operator=(const VECTOR& value) {

	x = value.x;
	y = value.y;
	z = value.z;
}

const Vector3 Vector3::operator+(const Vector3& value) const {

	Vector3 vec;

	vec.x = x + value.x;
	vec.y = y + value.y;
	vec.z = z + value.z;

	return vec;
}

const Vector3 Vector3::operator+(const VECTOR& value) const {

	Vector3 vec;

	vec.x = x + value.x;
	vec.y = y + value.y;
	vec.z = z + value.z;

	return vec;
}

const Vector3 Vector3::operator-(const Vector3& value) const {

	Vector3 vec;

	vec.x = x - value.x;
	vec.y = y - value.y;
	vec.z = z - value.z;

	return vec;
}

const Vector3 Vector3::operator-(const VECTOR& value) const {

	Vector3 vec;

	vec.x = x - value.x;
	vec.y = y - value.y;
	vec.z = z - value.z;

	return vec;
}

const Vector3 Vector3::operator*(const Vector3& value) const {

	Vector3 vec;

	vec.x = x * value.x;
	vec.y = y * value.y;
	vec.z = z * value.z;

	return vec;
}

const Vector3 Vector3::operator*(const VECTOR& value) const {

	Vector3 vec;

	vec.x = x * value.x;
	vec.y = y * value.y;
	vec.z = z * value.z;

	return vec;
}

const Vector3 Vector3::operator/(const Vector3& value) const {

	Vector3 vec;

	vec.x = x / value.x;
	vec.y = y / value.y;
	vec.z = z / value.z;

	return vec;
}

const Vector3 Vector3::operator/(const VECTOR& value) const {

	Vector3 vec;

	vec.x = x / value.x;
	vec.y = y / value.y;
	vec.z = z / value.z;

	return vec;
}

const Vector3 Vector3::operator*(const MATRIX& mat) const {

	return VTransform(*this, mat);
}

const Vector3 Vector3::operator*(float value) const {

	Vector3 vec;

	vec.x = x * value;
	vec.y = y * value;
	vec.z = z * value;

	return vec;
}

const Vector3 Vector3::operator*(int value) const {

	Vector3 vec;

	vec.x = x * value;
	vec.y = y * value;
	vec.z = z * value;

	return vec;
}

const Vector3 Vector3::operator/(float value) const {

	Vector3 vec;

	vec.x = x / value;
	vec.y = y / value;
	vec.z = z / value;

	return vec;
}

const Vector3 Vector3::operator/(int value) const {

	Vector3 vec;

	vec.x = x / value;
	vec.y = y / value;
	vec.z = z / value;

	return vec;
}

void Vector3::operator+=(const Vector3& value) {

	x = x + value.x;
	y = y + value.y;
	z = z + value.z;
}

void Vector3::operator+=(const VECTOR& value) {

	x = x + value.x;
	y = y + value.y;
	z = z + value.z;
}

void Vector3::operator-=(const Vector3& value) {

	x = x - value.x;
	y = y - value.y;
	z = z - value.z;
}

void Vector3::operator-=(const VECTOR& value) {

	x = x - value.x;
	y = y - value.y;
	z = z - value.z;
}

void Vector3::operator*=(const Vector3& value) {

	x = x * value.x;
	y = y * value.y;
	z = z * value.z;
}

void Vector3::operator*=(const VECTOR& value) {

	x = x / value.x;
	y = y / value.y;
	z = z / value.z;
}

void Vector3::operator/=(const Vector3& value) {

	x = x / value.x;
	y = y / value.y;
	z = z / value.z;
}

void Vector3::operator/=(const VECTOR& value) {

	x = x / value.x;
	y = y / value.y;
	z = z / value.z;
}

void Vector3::operator*=(float value) {

	x = x * value;
	y = y * value;
	z = z * value;
}

void Vector3::operator*=(int value) {

	x = x * value;
	y = y * value;
	z = z * value;
}

void Vector3::operator/=(float value) {

	x = x / value;
	y = y / value;
	z = z / value;
}

void Vector3::operator/=(int value) {

	x = x / value;
	y = y / value;
	z = z / value;
}

void Vector3::operator*=(const MATRIX& mat) {

	*this = VTransform(*this, mat);
}

void Vector3::operator=(float value) {

	x = value;
	y = value;
	z = value;
}

void Vector3::operator=(int value) {

	x = static_cast<float>(value);
	y = static_cast<float>(value);
	z = static_cast<float>(value);
}

bool Vector3::operator==(const Vector3& value) const {

	return x == value.x && y == value.y && z == value.z;
}

bool Vector3::operator==(const VECTOR& value) const {

	return x == value.x && y == value.y && z == value.z;
}

bool Vector3::operator!=(const Vector3& value) const {

	return !(*this == value);
}

bool Vector3::operator!=(const VECTOR& value) const {

	return !(*this == value);
}

float Vector3::Get(int i) const {

	float value = 0.0f;

	switch (i) {
	case 0:
		value = x;
		break;
	case 1:
		value = y;
		break;
	case 2:
		value = z;
		break;
	}

	return value;
}

void Vector3::Set(int i, float value) {

	switch (i) {
	case 0:
		x = value;
		break;
	case 1:
		y = value;
		break;
	case 2:
		z = value;
		break;
	}
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	return Vector3(
		Function::Lerp(start.x, end.x, t),
		Function::Lerp(start.y, end.y, t),
		Function::Lerp(start.z, end.z, t));
}
