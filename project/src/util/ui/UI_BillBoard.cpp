#include "src/util/ui/UI_BillBoard.h"
#include "src/util/ptr/PtrUtil.h"

UI_BillBoard::UI_BillBoard(const int& image, const Transform& trs, const Vector2& _center, const float& _clipSize) {

	SetImage(image, Vector2::Zero, Vector2(_clipSize), _center);

	transform = new Transform(trs);
}

UI_BillBoard::~UI_BillBoard() {

	PtrUtil::SafeDelete(transform);
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
