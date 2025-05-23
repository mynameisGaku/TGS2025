#include "src/util/ui/UI_Subtone.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include <src/util/ptr/PtrUtil.h>

UI_Subtone::UI_Subtone() : UI_Subtone(-1, GetColor(0, 0, 0), 125, true, Vector2::Zero, Vector2::Zero) {}

UI_Subtone::UI_Subtone(const int& image, const int& _color, const int& _alpha, const bool& active, const Vector2& _begin, const Vector2& _end) {

	hImage = image;
	color = _color;
	alpha = _alpha;
	SetIsActive(active);

	PtrUtil::SafeDelete(rectTransform);

	SetPoint(_begin, _end);
}

UI_Subtone::~UI_Subtone() {

}

inline void UI_Subtone::Draw() {

	if (IsActive() == false)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// ◇サブトーンの画像が設定されている場合
	if (hImage != -1) {

		DrawModiGraphF(point[0].x, point[0].y, point[1].x, point[1].y, point[2].x, point[2].y, point[3].x, point[3].y, hImage, true);

#ifdef _DEBUG
		for (int i = 0; i < POINT_DIR_NUM; i++)
			DrawCircleAA(point[i].x, point[i].y, 8, 8, PointColor[i], true);
#endif

	}
	// ◇サブトーンの画像が設定されていない場合
	else {

		DrawBoxAA(begin.x, begin.y, end.x, end.y, GetColor(0, 0, 0), true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_Subtone::Draw(const Vector2& _begin, const Vector2& _end) {

	if (IsActive() == false)
		return;

	SetPoint(_begin, _end);

	Draw();
}

void UI_Subtone::SetPoint(const Vector2& _begin, const Vector2& _end) {

	// 始点座標
	begin = _begin;

	// 終点座標
	end = _end;

	// 左上
	point[0] = _begin;

	// 右上
	point[1].x = _end.x;
	point[1].y = _begin.y;

	// 左下
	point[2].x = _begin.x;
	point[2].y = _end.y;

	// 右下
	point[3] = _end;
}
