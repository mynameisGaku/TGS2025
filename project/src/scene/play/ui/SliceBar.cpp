#include "SliceBar.h"

SliceBar::SliceBar() : SliceBar(RectTransform())
{
}

SliceBar::SliceBar(const RectTransform& trs)
{
	SetTransform(trs);

	m_ImageWidth = 0;
	m_ImageHeight = 0;

	m_ImagePartWidth[pLEFT] = 0;
	m_ImagePartWidth[pCENTER] = 0;
	m_ImagePartWidth[pRIGHT] = 0;

	m_IsInitImage = false;
}

SliceBar::~SliceBar()
{
}

void SliceBar::Update()
{
}

void SliceBar::Draw()
{
	if (not m_IsInitImage) return;
	if (m_ImageWidth <= 0) return;
	if (m_ImageHeight <= 0) return;

	RectTransform global = rectTransform->Global();

	//=== スケール計算 ===
	std::array<float, SLICE_COUNT> partScaleX;

	float scaleY = global.scale.y / m_ImageHeight;

	// 縦横比を保つため、左右の横幅は縦幅に合わせる
	partScaleX[pLEFT] = scaleY;
	partScaleX[pRIGHT] = scaleY;

	// 横幅に合わせて中央を引き伸ばす
	float sideLength = (m_ImagePartWidth[pLEFT] * partScaleX[pLEFT]) + (m_ImagePartWidth[pRIGHT] * partScaleX[pRIGHT]);
	float centerLength = global.scale.x - sideLength;

	partScaleX[pCENTER] = centerLength / m_ImagePartWidth[pCENTER];

	//======================

	m_Color.SetDrawBright();

	float cx = center.x * global.scale.x;
	float cy = center.y * global.scale.y;

	int srcX = 0;
	float currentWidth = 0;
	for (size_t i = 0; i < SLICE_COUNT; i++)
	{
		DrawRectRotaGraph3F(global.position.x + currentWidth - cx, global.position.y - cy, srcX, 0, m_ImagePartWidth[i], m_ImageHeight, 0, 0, partScaleX[i], scaleY, 0.0, hImage, TRUE);

		srcX += m_ImagePartWidth[i];
		currentWidth += m_ImagePartWidth[i] * partScaleX[i];
	}

	SetDrawBright(255, 255, 255);
}

void SliceBar::InitImage(int _hImage)
{
	hImage = _hImage;
	if (hImage <= 0) return;

	GetGraphSize(hImage, &m_ImageWidth, &m_ImageHeight);

	// 各パーツの横幅は等しいとする
	const int partWidth = m_ImageWidth / 3;
	m_ImagePartWidth[pLEFT] = partWidth;
	m_ImagePartWidth[pCENTER] = partWidth;
	m_ImagePartWidth[pRIGHT] = partWidth;

	m_IsInitImage = true;
}
