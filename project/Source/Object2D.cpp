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

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報

	Vector2 offset = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	float x = globalTrs.position.x + offset.x;	// 座標(X軸)
	float y = globalTrs.position.y + offset.y;	// 座標(Y軸)

	int sx = static_cast<int>(clipStart.x);	// 切り抜き開始地点(X軸)
	int sy = static_cast<int>(clipStart.y);	// 切り抜き開始地点(Y軸)

	int dx = static_cast<int>(clipSize.x);	// 切り抜きの大きさ(X軸)
	int dy = static_cast<int>(clipSize.y);	// 切り抜きの大きさ(Y軸)

	float cx = imageSize.x * center.x;	// 描画を行う際の中心点(X軸)
	float cy = imageSize.y * center.y;	// 描画を行う際の中心点(Y軸)

	float w = globalTrs.scale.x;	// 拡大率(X軸)
	float h = globalTrs.scale.y;	// 拡大率(Y軸)

	float ang = globalTrs.rotation;	// 回転角度

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

	Vector2 offset = V2::ZERO;	// 差
	const float adjust = 2.0f;	// 調整値

	// 水平軸のズレ調整
	switch (Anchor::PlacementHorizontal(rectTransform->preset)) {
	case Anchor::Placement::LEFT:	offset.x = imageSize.x * 0.5f;	break;
	case Anchor::Placement::RIGHT:	offset.x = -imageSize.x * 0.5f;	break;
	case Anchor::Placement::MIDDLE:	offset.x = 0.0f;				break;
	default:						offset.x = 0.0f;				break;
	}

	// 垂直軸のズレ調整
	switch (Anchor::PlacementVertical(rectTransform->preset)) {
	case Anchor::Placement::UP:		offset.y = imageSize.y * 0.5f - adjust;	break;
	case Anchor::Placement::DOWN:	offset.y = -imageSize.y * 0.5f - adjust;	break;
	case Anchor::Placement::MIDDLE:	offset.y = 0.0f;							break;
	default:						offset.y = 0.0f;							break;
	}

	return offset;
}
