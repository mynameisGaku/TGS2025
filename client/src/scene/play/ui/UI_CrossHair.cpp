#include "UI_CrossHair.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/ui/UI_Manager.h"

UI_CrossHair::UI_CrossHair(const RectTransform& trs, int index)
{
	m_CharaIndex = index;
	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	SetScroll(nullptr, 0.0f, 1.0f, Gauge::ScrollType::eRight);

	hCrossHair = -1;
	hCrossHairFrame = -1;

	hCrossHairOutSide = -1;
	hCrossHairOutSideBack = -1;

	m_Gauge = Gauge();
}

UI_CrossHair::~UI_CrossHair()
{
	if (hCrossHair > 0)
		ResourceLoader::DeleteGraph(hCrossHair);

	if (hCrossHairFrame > 0)
		ResourceLoader::DeleteGraph(hCrossHairFrame);

	if (hCrossHairOutSide > 0)
		ResourceLoader::DeleteGraph(hCrossHairOutSide);

	if (hCrossHairOutSideBack > 0)
		ResourceLoader::DeleteGraph(hCrossHairOutSideBack);
}

void UI_CrossHair::Update()
{
	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();
}

void UI_CrossHair::Draw()
{
	UI_Canvas::Draw();

	const RectTransform globalTrs = rectTransform->Global();

	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, hCrossHairFrame, true);
	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, hCrossHair, true);

	if (m_pValue != nullptr)
	{
		float norm = *m_pValue / m_ValueMax;

		if (m_IsDispMode)
			m_Gauge.DrawRectRotaGraphGauge(globalTrs.position, norm, 1.0f, 0.0f, hCrossHairOutSide, -1, -1, hCrossHairOutSideBack, 1.0f, Vector2::Ones, 0.0f, m_ScrollType);
		else
			m_Gauge.DrawRectRotaGraphGauge(globalTrs.position, 1.0f - norm, 1.0f, 0.0f, hCrossHairOutSide, -1, -1, hCrossHairOutSideBack, 1.0f, Vector2::Ones, 0.0f, m_ScrollType);
	}
}

void UI_CrossHair::SetHandle_CrossHair(const std::string& path)
{
	if (hCrossHair != -1)
		ResourceLoader::DeleteGraph(hCrossHair);

	hCrossHair = ResourceLoader::LoadGraph(path);
}

void UI_CrossHair::SetHandle_CrossHairFrame(const std::string& path)
{
	if (hCrossHairFrame != -1)
		ResourceLoader::DeleteGraph(hCrossHairFrame);

	hCrossHairFrame = ResourceLoader::LoadGraph(path);
}

void UI_CrossHair::SetHandle_CrossHairOutSide(const std::string& path)
{
	if (hCrossHairOutSide != -1)
		ResourceLoader::DeleteGraph(hCrossHairOutSide);

	hCrossHairOutSide = ResourceLoader::LoadGraph(path);
}

void UI_CrossHair::SetHandle_CrossHairOutSideBack(const std::string& path)
{
	if (hCrossHairOutSideBack != -1)
		ResourceLoader::DeleteGraph(hCrossHairOutSideBack);

	hCrossHairOutSideBack = ResourceLoader::LoadGraph(path);
}

void UI_CrossHair::SetScroll(float* m_Value, float min, float max, Gauge::ScrollType scroll, bool dispMode) {

	this->m_pValue = m_Value;
	m_ValueMin = min;
	m_ValueMax = max;

	m_IsDispMode = dispMode;

	m_ScrollType = scroll;
}
