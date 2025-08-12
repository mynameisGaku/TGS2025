#pragma once
#include "src/util/ui/UI_Image.h"

// 3ƒXƒ‰ƒCƒX‰æ‘œ‚Å”CˆÓ‚Ì’·‚³‚Ì–_‚ð•`‰æ‚·‚é
class SliceBar : public UI_Image
{
public:
	SliceBar();
	SliceBar(const RectTransform& trs);
	~SliceBar();

	void Update() override;
	void Draw() override;

private:
	int m_Image;
};
