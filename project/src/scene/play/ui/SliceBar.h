#pragma once
#include "src/util/ui/UI_Image.h"
#include "src/util/color/ColorUtil.h"
#include <array>

// 3ƒXƒ‰ƒCƒX‰æ‘œ‚Å”CˆÓ‚Ì’·‚³‚Ì–_‚ð•`‰æ‚·‚é
class SliceBar : public UI_Image
{
public:
	SliceBar();
	SliceBar(const RectTransform& trs);
	~SliceBar();

	void Update() override;
	void Draw() override;

	void InitImage(int _hImage);

private:
	enum SlicePart
	{
		pLEFT,
		pCENTER,
		pRIGHT,
		SLICE_COUNT
	};

	int m_ImageWidth;
	int m_ImageHeight;

	std::array<int, SLICE_COUNT> m_ImagePartWidth;

	bool m_IsInitImage;
};
