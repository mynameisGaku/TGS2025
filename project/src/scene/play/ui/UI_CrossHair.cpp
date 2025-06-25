#include "UI_CrossHair.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/screen/ScreenManager.h"

UI_CrossHair::UI_CrossHair(const RectTransform& trs, int index)
{
	charaIndex = index;

	Vector2 beginPos = ScreenManager::GetScreenBeginPos(charaIndex);
	Vector2 endPos = ScreenManager::GetScreenEndPos(charaIndex);
	Vector2 size = endPos - beginPos;

	RectTransform rectTrs = trs;
	rectTrs.anchor.SetBegin(beginPos);
	rectTrs.anchor.SetEnd(endPos);

	SetTransform(rectTrs);

	SetScroll(nullptr, 0.0f, 1.0f, Gauge::ScrollType::eRight);

	hCrossHair = -1;
	hCrossHairFrame = -1;

	hCrossHairOutSide = -1;
	hCrossHairOutSideBack = -1;

	gauge = Gauge();
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
	//if (charaIndex >= 0) {

	//	if (CameraManager::IsScreenDivision()) {
	//		SetIsDraw(true);
	//		Vector2 cameraDiv = CameraManager::GetDivedByCameraNum();
	//		rectTransform->position = Vector2(cameraDiv.x * charaIndex + cameraDiv.x * 0.5f, WindowSetting::Inst().height_half);
	//	}
	//	else if (charaIndex == 0) {
	//		SetIsDraw(true);
	//		Vector2 drawPos = CameraManager::GetScreenDivisionPos_CameraIndex(charaIndex);
	//		Vector2 drawSize = CameraManager::GetScreenDivisionSize();
	//		rectTransform->position = (drawPos + drawSize) * 0.5f;
	//	}
	//	else {
	//		SetIsDraw(false);
	//	}
	//}
	UI_Canvas::Update();
}

void UI_CrossHair::Draw()
{
	UI_Canvas::Draw();

	const RectTransform globalTrs = rectTransform->Global();

	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, hCrossHairFrame, true);
	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, hCrossHair, true);

	if (value != nullptr)
	{
		float norm = *value / valueMax;

		if (isDispMode)
			gauge.DrawRectRotaGraphGauge(globalTrs.position, norm, 1.0f, 0.0f, hCrossHairOutSide, -1, -1, hCrossHairOutSideBack, 1.0f, Vector2::Ones, 0.0f, scrollType);
		else
			gauge.DrawRectRotaGraphGauge(globalTrs.position, 1.0f - norm, 1.0f, 0.0f, hCrossHairOutSide, -1, -1, hCrossHairOutSideBack, 1.0f, Vector2::Ones, 0.0f, scrollType);
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

void UI_CrossHair::SetScroll(float* value, float min, float max, Gauge::ScrollType scroll, bool dispMode) {

	this->value = value;
	valueMin = min;
	valueMax = max;

	isDispMode = dispMode;

	scrollType = scroll;
}
