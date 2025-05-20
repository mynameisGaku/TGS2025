#include "object2D.h"
#include "../Library/resourceLoader.h"

Object2D::Object2D() : Object2D(-1, RectTransform()) {}

Object2D::Object2D(const RectTransform& _rectTransform) : Object2D(-1, _rectTransform) {}

Object2D::Object2D(const int& image, const RectTransform& _rectTransform) {

	SetTransform(_rectTransform);
	SetImage(image);
}

Object2D::~Object2D() {

	if (hImage != -1) {
		ResourceLoader::DeleteGraph(hImage);
		hImage = -1;
	}

	Function::DeletePointer(rectTransform);
}

void Object2D::Draw() {

	if (IsActive() == false || hImage <= -1)
		return;

	RectTransform globalTrs = rectTransform->Global();	// �O���[�o���ȍ��W�E��]�E�g�k���

	Vector2 offset = DisplacementByAnchorPoint();	// �A���J�[�|�C���g�ɂ����W�̃Y���l

	float x = globalTrs.position.x + offset.x;	// ���W(X��)
	float y = globalTrs.position.y + offset.y;	// ���W(Y��)

	int sx = static_cast<int>(clipStart.x);	// �؂蔲���J�n�n�_(X��)
	int sy = static_cast<int>(clipStart.y);	// �؂蔲���J�n�n�_(Y��)

	int dx = static_cast<int>(clipSize.x);	// �؂蔲���̑傫��(X��)
	int dy = static_cast<int>(clipSize.y);	// �؂蔲���̑傫��(Y��)

	float cx = imageSize.x * center.x;	// �`����s���ۂ̒��S�_(X��)
	float cy = imageSize.y * center.y;	// �`����s���ۂ̒��S�_(Y��)

	float w = globalTrs.scale.x;	// �g�嗦(X��)
	float h = globalTrs.scale.y;	// �g�嗦(Y��)

	float ang = globalTrs.rotation;	// ��]�p�x

	DrawRectRotaGraphF(x, y, sx, sy, dx, dy, (w + h) * 0.5f, ang, hImage, true);
}

void Object2D::SetTransform(const RectTransform& trs) {

	Function::DeletePointer(rectTransform);
	rectTransform = new RectTransform(trs);
}

void Object2D::SetImage(const int& handle) {

	if (handle < 0) {
		if (hImage != -1) {
			ResourceLoader::DeleteGraph(hImage);
			hImage = -1;
		}

		return;
	}

	hImage = handle;

	int width;
	int height;
	GetGraphSize(hImage, &width, &height);
	imageSize.x = static_cast<float>(width);
	imageSize.y = static_cast<float>(height);

	SetClipSize(imageSize);
}


void Object2D::SetImage(const int& handle, const Vector2& _clipStart, const Vector2& _clipSize, const Vector2& _center) {

	SetImage(handle);
	SetClipStart(_clipStart);
	SetClipSize(_clipSize);
	SetCenter(_center);
}

Vector2 Object2D::DisplacementByAnchorPoint() {

	Vector2 offset = V2::ZERO;	// ��
	const float adjust = 2.0f;	// �����l

	// �������̃Y������
	switch (Anchor::PlacementHorizontal(rectTransform->preset)) {
	case Anchor::Placement::LEFT:	offset.x = imageSize.x * 0.5f;	break;
	case Anchor::Placement::RIGHT:	offset.x = -imageSize.x * 0.5f;	break;
	case Anchor::Placement::MIDDLE:	offset.x = 0.0f;				break;
	default:						offset.x = 0.0f;				break;
	}

	// �������̃Y������
	switch (Anchor::PlacementVertical(rectTransform->preset)) {
	case Anchor::Placement::UP:		offset.y = imageSize.y * 0.5f - adjust;	break;
	case Anchor::Placement::DOWN:	offset.y = -imageSize.y * 0.5f - adjust;	break;
	case Anchor::Placement::MIDDLE:	offset.y = 0.0f;							break;
	default:						offset.y = 0.0f;							break;
	}

	return offset;
}
