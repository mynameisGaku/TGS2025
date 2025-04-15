#include "Vector2.h"

void Vector2::operator=(const Vector2& value) {

	x = value.x;
	y = value.y;
}

void Vector2::operator=(const VECTOR& value) {

	x = value.x;
	y = value.y;
}

const Vector2 Vector2::operator+(const Vector2& value) const {

	Vector2 vec;

	vec.x = x + value.x;
	vec.y = y + value.y;

	return vec;
}

const Vector2 Vector2::operator-(const Vector2& value) const {

	Vector2 vec;

	vec.x = x - value.x;
	vec.y = y - value.y;

	return vec;
}

const Vector2 Vector2::operator*(const Vector2& value) const {

	Vector2 vec;

	vec.x = x * value.x;
	vec.y = y * value.y;

	return vec;
}

const Vector2 Vector2::operator/(const Vector2& value) const {

	Vector2 vec;

	vec.x = x / value.x;
	vec.y = y / value.y;

	return vec;
}

const Vector2 Vector2::operator*(float value) const {

	Vector2 vec;

	vec.x = x * value;
	vec.y = y * value;

	return vec;
}

const Vector2 Vector2::operator*(int value) const {

	Vector2 vec;

	vec.x = x * value;
	vec.y = y * value;

	return vec;
}

const Vector2 Vector2::operator/(float value) const {

	Vector2 vec;

	vec.x = x / value;
	vec.y = y / value;

	return vec;
}

const Vector2 Vector2::operator/(int value) const {

	Vector2 vec;

	vec.x = x / value;
	vec.y = y / value;

	return vec;
}

void Vector2::operator+=(const Vector2& value) {

	x = x + value.x;
	y = y + value.y;
}

void Vector2::operator-=(const Vector2& value) {

	x = x - value.x;
	y = y - value.y;
}

void Vector2::operator*=(const Vector2& value) {

	x = x * value.x;
	y = y * value.y;
}

void Vector2::operator/=(const Vector2& value) {

	x = x / value.x;
	y = y / value.y;
}

void Vector2::operator*=(float value) {

	x = x * value;
	y = y * value;
}

void Vector2::operator*=(int value) {

	x = x * value;
	y = y * value;
}

void Vector2::operator/=(float value) {

	x = x / value;
	y = y / value;
}

void Vector2::operator/=(int value) {

	x = x / value;
	y = y / value;
}

void Vector2::operator=(float value) {

	x = value;
	y = value;
}

void Vector2::operator=(int value) {

	x = static_cast<float>(value);
	y = static_cast<float>(value);
}

bool Vector2::operator==(const Vector2& value) const {

	return x == value.x && y == value.y;
}

bool Vector2::operator==(const VECTOR& value) const {

	return x == value.x && y == value.y;
}

bool Vector2::operator!=(const Vector2& value) const {

	return !(*this == value);
}

bool Vector2::operator!=(const VECTOR& value) const {

	return !(*this == value);
}

float Vector2::Get(int i) const {

	float value = 0.0f;

	switch (i) {
	case 0:
		value = x;
		break;
	case 1:
		value = y;
		break;
	}

	return value;
}

void Vector2::Set(int i, float value) {

	switch (i) {
	case 0:
		x = value;
		break;
	case 1:
		y = value;
		break;
	}
}
