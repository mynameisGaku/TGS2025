#include "Transform.h"

// ���ėp
#include "Utils.h"

Transform::Transform(const Vector3& pos, const Vector3& rot, const Vector3& scl, Transform* p) {

	position = pos;
	rotation = rot;
	scale = scl;
	parent = nullptr;

	SetParent(p);
}

Transform::~Transform() {

	// �e�Ɖ���؂�
	if (parent != nullptr)
		parent->RemoveChild(this);

	// �q�Ɖ���؂�
	for (Transform* c : children)
		c->RemoveParent(this, false);
}

const Transform Transform::Global() const {

	// �����̏��(���ΓI)
	Vector3 globalPosition = position;
	Vector3 globalRotation = rotation;
	Vector3 globalScale = scale;

	// �e�����ꍇ
	if (parent != nullptr) {
		const Transform parentGlobal = parent->Global();
		globalPosition = VTransform(position, parentGlobal.Matrix());
		globalRotation += parentGlobal.rotation;
		globalScale *= parentGlobal.scale;
	}

	return Transform(globalPosition, globalRotation, globalScale);
}

const Transform Transform::Parent() const {

	if (parent == nullptr)
		return Transform();

	return parent->Global();
}

const MATRIX Transform::Matrix() const {

	MATRIX matrix = MGetIdent();
	matrix = MMult(matrix, MGetScale(scale));
	matrix = MMult(matrix, RotationMatrix());
	matrix = MMult(matrix, MGetTranslate(position));
	return matrix;
}

const MATRIX Transform::RotationMatrix() const {

	MATRIX matrix = MGetIdent();
	matrix = MMult(matrix, MGetRotZ(rotation.z));
	matrix = MMult(matrix, MGetRotX(rotation.x));
	matrix = MMult(matrix, MGetRotY(rotation.y));
	return matrix;
}

const Vector3 Transform::Right() const {

	Vector3 vec = Vector3(1, 0, 0);
	vec = VTransform(vec, RotationMatrix());
	return vec;
}

const Vector3 Transform::Up() const {

	Vector3 vec = Vector3(0, 1, 0);
	vec = VTransform(vec, RotationMatrix());
	return vec;
}

const Vector3 Transform::Forward() const {

	Vector3 vec = Vector3(0, 0, 1);
	vec = VTransform(vec, RotationMatrix());
	return vec;
}

const Vector3 Transform::LimitedRotation() const {

	using namespace Math;

	Vector3 limitedRot = V3::ZERO;

	// x,y,z���ꂼ��v�Z
	for (int i = 0; i < 3; i++) {
		float angle = rotation.Get(i);

		// 0��菬�����ꍇ�A2��(���)����������
		while (angle < 0)
			angle += PI_TW;

		// 2�Έȏ�̏ꍇ�A2��(���)����������
		while (angle >= PI_TW)
			angle -= PI_TW;

		limitedRot.Set(i, angle);
	}

	return limitedRot;
}

void Transform::SetParent(Transform* _parent) {

	if (parent != nullptr)
		RemoveParent(parent);

	parent = _parent;

	if (parent != nullptr)
		parent->AddChild(this);
}

void Transform::SetGlobal(const Transform& global) {

	this->position -= parent->Global().position;

	MATRIX mRot = MGetIdent();
	mRot = MMult(mRot, MGetRotZ(-parent->Global().rotation.z));
	mRot = MMult(mRot, MGetRotX(-parent->Global().rotation.x));
	mRot = MMult(mRot, MGetRotY(-parent->Global().rotation.y));

	this->position = this->position * mRot;

	this->position.x /= parent->Global().scale.x;
	this->position.y /= parent->Global().scale.y;
	this->position.z /= parent->Global().scale.z;

	this->rotation -= parent->Global().rotation;

	this->scale /= parent->Global().scale;
}

Transform* Transform::Copy() const {

	return new Transform(*this);
}

const Transform Transform::operator+(const Transform& trs) const {

	Transform ans = *this;

	ans.position = position + trs.position;
	ans.rotation = rotation + trs.rotation;
	ans.scale = scale + trs.scale;

	return ans;
}

const Transform Transform::operator-(const Transform& trs) const {

	Transform ans;

	ans.position = position - trs.position;
	ans.rotation = rotation - trs.rotation;
	ans.scale = scale - trs.scale;

	return ans;
}

const Transform Transform::operator*(const float sca) const {

	Transform ans;

	ans.position = position * sca;
	ans.rotation = rotation * sca;
	ans.scale = scale * sca;

	return ans;
}

const Transform Transform::operator/(const float sca) const {

	Transform ans;

	ans.position = position / sca;
	ans.rotation = rotation / sca;
	ans.scale = scale / sca;

	return ans;
}

const Transform Transform::operator-() const {

	Transform ans = *this;

	ans *= -1.0f;

	return ans;
}

void Transform::operator+=(const Transform& trs) {

	position = position + trs.position;
	rotation = rotation + trs.rotation;
	scale = scale + trs.scale;
}

void Transform::operator-=(const Transform& trs) {

	position = position - trs.position;
	rotation = rotation - trs.rotation;
	scale = scale - trs.scale;
}

void Transform::operator*=(const float sca) {

	position = position * sca;
	rotation = rotation * sca;
	scale = scale * sca;
}

void Transform::RemoveChild(Transform* child) {

	for (auto itr = children.begin(); itr != children.end(); itr++) {
		if (*itr == child) {
			children.erase(itr);
			return;
		}
	}
}

void Transform::RemoveParent(Transform* _parent, bool removeFromChildren) {

	// �e��nullptr�Ȃ�X�L�b�v
	if (parent == nullptr) return;

	// �e�ȊO�ɂ͑���ł��Ȃ�
	if (parent != _parent) return;

	// ���W���ړ����Ȃ��悤��
	const Transform globalTransform = this->Global();
	this->position = globalTransform.position;
	this->rotation = globalTransform.rotation;
	this->scale = globalTransform.scale;

	// �q�����f��
	if (removeFromChildren)
		parent->RemoveChild(this);

	parent = nullptr;
}
