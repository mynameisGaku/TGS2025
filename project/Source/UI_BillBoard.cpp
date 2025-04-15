#include "ui_BillBoard.h"

UI_BillBoard::UI_BillBoard(const int& image, const Transform& trs, const Vector2& _center, const float& _clipSize) {

	SetImage(image, V2::ZERO, Vector2(_clipSize), _center);

	transform = new Transform(trs);
}

UI_BillBoard::~UI_BillBoard() {

	Function::DeletePointer(transform);
}

void UI_BillBoard::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	UI_Canvas::Update();
}

void UI_BillBoard::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawBillboard3D(transform->position, center.x, center.y, clipSize * transform->scale.Average(), transform->rotation.y, hImage, true);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
