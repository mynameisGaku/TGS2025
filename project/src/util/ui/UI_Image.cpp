#include "src/util/ui/UI_Image.h"

UI_Image::UI_Image(const int& image, const RectTransform& trs) {

	Object2D::SetTransform(trs);
	Object2D::SetImage(image);
}

UI_Image::~UI_Image() {

}

void UI_Image::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	DrawAfterImage();
}

void UI_Image::DrawAfterImage() {

	if (needAfterImage == false)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, easingAlphaAftImg.current);

	RectTransform globalTrs = rectTransform->Global();	// �O���[�o���ȍ��W�E��]�E�g�k���

	Vector2 devi = DisplacementByAnchorPoint();	// �A���J�[�|�C���g�ɂ��Y���l

	Vector2 aftimgPos = easingMoveAftImg.current;	// �c�������p���W
	float aftimgRot = easingRotAftImg.current;		// �c�������p��]
	Vector2 aftimgScale = easingScaleAftImg.current;// �c�������p�g�k

	float x = globalTrs.position.x + devi.x + aftimgPos.x;	// ���W(X��)
	float y = globalTrs.position.y + devi.y + aftimgPos.y;	// ���W(Y��)

	int sx = static_cast<int>(clipStart.x);	// �؂蔲���J�n�n�_(X��)
	int sy = static_cast<int>(clipStart.y);	// �؂蔲���J�n�n�_(Y��)

	int dx = static_cast<int>(clipSize.x);	// �؂蔲���̑傫��(X��)
	int dy = static_cast<int>(clipSize.y);	// �؂蔲���̑傫��(Y��)

	float cx = imageSize.x * center.x;	// �`����s���ۂ̒��S�_(X��)
	float cy = imageSize.y * center.y;	// �`����s���ۂ̒��S�_(Y��)

	float w = globalTrs.scale.x * aftimgScale.x;	// �g�嗦(X��)
	float h = globalTrs.scale.y * aftimgScale.y;	// �g�嗦(Y��)

	float ang = globalTrs.rotation + aftimgRot;	// ��]�p�x

	DrawRectRotaGraphF(x, y, sx, sy, dx, dy, (w + h) * 0.5f, ang, hImage, true);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
