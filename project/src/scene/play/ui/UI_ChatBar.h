#pragma once
#include "src/util/ui/UI_Image.h"
#include <unordered_map>
#include <string>

class SliceBar;

class UI_ChatBar : public UI_Image {
public:
	UI_ChatBar();
	UI_ChatBar(const RectTransform& trs, int index);
	~UI_ChatBar();

	void ReloadParam();
	void Update() override;
	void Draw() override;

private:
	int m_hBarImage;
	std::unordered_map<std::string, SliceBar*> m_BarList;
};
