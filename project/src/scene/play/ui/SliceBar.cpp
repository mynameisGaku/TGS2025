#include "SliceBar.h"
#include "src/util/file/resource_loader/ResourceLoader.h"

SliceBar::SliceBar() : SliceBar(RectTransform())
{
}

SliceBar::SliceBar(const RectTransform& trs)
{
	m_Image = -1;

	SetTransform(trs);

	hImage = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar.png");
	assert(hImage > 0);
}

SliceBar::~SliceBar()
{
	ResourceLoader::DeleteGraph(hImage);
}

void SliceBar::Update()
{
}

void SliceBar::Draw()
{
	RectTransform global = rectTransform->Global();

	DrawGraphF(global.position.x, global.position.y, hImage, TRUE);
}
