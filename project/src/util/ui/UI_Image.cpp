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

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報

	Vector2 devi = DisplacementByAnchorPoint();	// アンカーポイントによるズレ値

	Vector2 aftimgPos = easingMoveAftImg.current;	// 残像処理用座標
	float aftimgRot = easingRotAftImg.current;		// 残像処理用回転
	Vector2 aftimgScale = easingScaleAftImg.current;// 残像処理用拡縮

	float x = globalTrs.position.x + devi.x + aftimgPos.x;	// 座標(X軸)
	float y = globalTrs.position.y + devi.y + aftimgPos.y;	// 座標(Y軸)

	int sx = static_cast<int>(clipStart.x);	// 切り抜き開始地点(X軸)
	int sy = static_cast<int>(clipStart.y);	// 切り抜き開始地点(Y軸)

	int dx = static_cast<int>(clipSize.x);	// 切り抜きの大きさ(X軸)
	int dy = static_cast<int>(clipSize.y);	// 切り抜きの大きさ(Y軸)

	float cx = imageSize.x * center.x;	// 描画を行う際の中心点(X軸)
	float cy = imageSize.y * center.y;	// 描画を行う際の中心点(Y軸)

	float w = globalTrs.scale.x * aftimgScale.x;	// 拡大率(X軸)
	float h = globalTrs.scale.y * aftimgScale.y;	// 拡大率(Y軸)

	float ang = globalTrs.rotation + aftimgRot;	// 回転角度

	DrawRectRotaGraphF(x, y, sx, sy, dx, dy, (w + h) * 0.5f, ang, hImage, true);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
