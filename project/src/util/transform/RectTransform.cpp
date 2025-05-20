#include "src/util/transform/RectTransform.h"

RectTransform::RectTransform() : RectTransform(Anchor::Preset::None, Vector2::Zero, 0.0f, Vector2::Ones, nullptr) {

}

RectTransform::RectTransform(const Vector2& pos, const float& rot, const Vector2& scl, RectTransform* p) :
	RectTransform(Anchor::Preset::None, pos, rot, scl, p) {

}

RectTransform::RectTransform(Anchor::Preset _preset, const Vector2& _position, const float& _rotation, const Vector2& _scale, RectTransform* _parent) {

	preset = _preset;
	position = _position;
	rotation = _rotation;
	scale = _scale;
	parent = nullptr;
	SetParent(_parent);
}

RectTransform::~RectTransform() {

	// 親と縁を切る
	if (parent != nullptr)
		parent->RemoveChild(this);

	// 子と縁を切る
	//for (RectTransform* c : children)
	//	c->RemoveParent(this, false);
}

const RectTransform RectTransform::Global() const {

	// 自分の情報(相対的)
	Vector2 globalPosition = position;
	float globalRotation = rotation;
	Vector2 globalScale = scale;

	globalPosition += Anchor::Point(preset);

	// 親を持つ場合
	if (parent != nullptr) {
		globalPosition += parent->Global().position;
		globalRotation += parent->Global().rotation;
		if (parent->Global().scale.GetLength() != 0.0f)
			globalScale *= parent->Global().scale;
	}

	return RectTransform(globalPosition, globalRotation, globalScale, nullptr);
}

RectTransform* RectTransform::Copy() const {

	RectTransform* newTrs = new RectTransform();

	newTrs->position = this->Global().position;
	newTrs->rotation = this->Global().rotation;
	newTrs->scale = this->Global().scale;

	return newTrs;
}

void RectTransform::RemoveChild(RectTransform* child) {

	for (auto itr = children.begin(); itr != children.end(); itr++) {
		if (*itr == child) {
			children.erase(itr);
			return;
		}
	}
}

void RectTransform::RemoveParent(RectTransform* _parent, bool removeFromChildren) {

	// 親がnullptrならスキップ
	if (parent == nullptr) return;
	// 親以外には操作できない
	if (parent != _parent) return;

	// 座標が移動しないように
	const RectTransform globalRectTransform = this->Global();
	this->position = globalRectTransform.position;
	this->rotation = globalRectTransform.rotation;
	this->scale = globalRectTransform.scale;

	// 繋がりを断つ
	if (removeFromChildren)
		parent->RemoveChild(this);

	parent = nullptr;
}

void RectTransform::SetParent(RectTransform* _parent) {

	if (parent != nullptr)
		RemoveParent(parent);

	parent = _parent;

	if (parent != nullptr)
		parent->AddChild(this);
}

void RectTransform::SetGlobal(const RectTransform& global) {

	this->position -= parent->Global().position;
	this->rotation -= parent->Global().rotation;
	this->scale /= parent->Global().scale;
}

const RectTransform RectTransform::operator+(const RectTransform& trs) const {

	RectTransform ans = *this;

	ans.position = position + trs.position;
	ans.rotation = rotation + trs.rotation;
	ans.scale = scale + trs.scale;

	return ans;
}

const RectTransform RectTransform::operator-(const RectTransform& trs) const {

	RectTransform ans;

	ans.position = position - trs.position;
	ans.rotation = rotation - trs.rotation;
	ans.scale = scale - trs.scale;

	return ans;
}

const RectTransform RectTransform::operator*(const float sca) const {

	RectTransform ans;

	ans.position = position * sca;
	ans.rotation = rotation * sca;
	ans.scale = scale * sca;

	return ans;
}

const RectTransform RectTransform::operator/(const float sca) const {

	RectTransform ans;

	ans.position = position / sca;
	ans.rotation = rotation / sca;
	ans.scale = scale / sca;

	return ans;
}

const RectTransform RectTransform::operator-() const {

	RectTransform ans = *this;

	ans *= -1.0f;

	return ans;
}

void RectTransform::operator+=(const RectTransform& trs) {

	position = position + trs.position;
	rotation = rotation + trs.rotation;
	scale = scale + trs.scale;
}

void RectTransform::operator-=(const RectTransform& trs) {

	position = position - trs.position;
	rotation = rotation - trs.rotation;
	scale = scale - trs.scale;
}

void RectTransform::operator*=(const float sca) {

	position = position * sca;
	rotation = rotation * sca;
	scale = scale * sca;
}

void RectTransform::DrawTest() {

#ifdef _DEBUG

	RectTransform rect0 = RectTransform(Anchor::Preset::LeftUp,		Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect1 = RectTransform(Anchor::Preset::LeftMiddle, Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect2 = RectTransform(Anchor::Preset::LeftDown,	Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect3 = RectTransform(Anchor::Preset::MiddleUp,	Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect4 = RectTransform(Anchor::Preset::Middle,		Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect5 = RectTransform(Anchor::Preset::MiddleDown, Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect6 = RectTransform(Anchor::Preset::RightUp,	Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect7 = RectTransform(Anchor::Preset::RightMiddle,Vector2::Zero, 0.0f, Vector2::Ones, nullptr);
	RectTransform rect8 = RectTransform(Anchor::Preset::RightDown,	Vector2::Zero, 0.0f, Vector2::Ones, nullptr);

	DrawCircleAA(rect0.Global().position.x, rect0.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect1.Global().position.x, rect1.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect2.Global().position.x, rect2.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect3.Global().position.x, rect3.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect4.Global().position.x, rect4.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect5.Global().position.x, rect5.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect6.Global().position.x, rect6.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect7.Global().position.x, rect7.Global().position.y, 32, 32, GetColor(255, 255, 255), true);
	DrawCircleAA(rect8.Global().position.x, rect8.Global().position.y, 32, 32, GetColor(255, 255, 255), true);

#else

	return;

#endif // _DEBUG
	
}
