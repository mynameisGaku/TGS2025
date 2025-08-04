#include "ScreenManager.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/common/camera/CameraManager.h"
#include "framework/myDxLib.h"
#include "src/util/ptr/PtrUtil.h"

namespace {

	std::unordered_map<std::string, ScreenData*>* m_hTemporaryScreens;
	std::list<ScreenData*>* m_DrawingScreens;
	ScreenData* m_DefaultScreen;
}

void ScreenManager::Init() {

	PtrUtil::CheckInstanceAndCreate(m_hTemporaryScreens);
	PtrUtil::CheckInstanceAndCreate(m_DrawingScreens);

	if (m_DefaultScreen == nullptr) {
		PtrUtil::CheckInstanceAndCreate(m_DefaultScreen);
		int w, h;
		GetWindowSize(&w, &h);

		m_DefaultScreen->tag = "Default";
		m_DefaultScreen->handle = -1;
		m_DefaultScreen->clipX = 0;
		m_DefaultScreen->clipY = 0;
		m_DefaultScreen->clipW = w;
		m_DefaultScreen->clipH = h;
		m_DefaultScreen->drawX = 0;
		m_DefaultScreen->drawY = 0;
		m_DefaultScreen->drawW = w;
		m_DefaultScreen->drawH = h;
		CreateScreen(*m_DefaultScreen);
	}
}

void ScreenManager::Update() {

}

void ScreenManager::Release() {

	if (m_DrawingScreens != nullptr) {
		for (const auto& clip : *m_DrawingScreens) {
			if (clip->handle != -1) {
				DeleteGraph(clip->handle);
				clip->handle = -1;
			}
		}
		m_DrawingScreens->clear();
		PtrUtil::SafeDelete(m_DrawingScreens);
	}

	if (m_hTemporaryScreens != nullptr) {
		for (auto& clip : *m_hTemporaryScreens) {
			if (clip.second->handle != -1) {
				DeleteGraph(clip.second->handle);
				clip.second->handle = -1;
			}

			PtrUtil::SafeDelete(clip.second);
		}
		m_hTemporaryScreens->clear();
		PtrUtil::SafeDelete(m_hTemporaryScreens);
	}

	if (m_DefaultScreen != nullptr) {
		if (m_DefaultScreen->handle != -1) {
			DeleteGraph(m_DefaultScreen->handle);
			m_DefaultScreen->handle = -1;
		}
		PtrUtil::SafeDelete(m_DefaultScreen);
	}
}

int ScreenManager::GetScreenDivCount()
{
	if (not CameraManager::IsScreenDivision())
		return 1;

	return (int)CameraManager::AllCameras().size();
}

int ScreenManager::GetScreenDivWidth()
{
	int width = WindowSetting::Inst().width;
	int divCount = GetScreenDivCount();

	return width / divCount;
}

int ScreenManager::GetScreenDivHeight()
{
	int height = WindowSetting::Inst().height;
	int divCount = GetScreenDivCount();

	return height / divCount;
}

Vector2 ScreenManager::GetScreenBeginPos(int divCount) {

	if (divCount < 0)
		return Vector2::Zero;

	return Vector2((float)GetScreenDivWidth() * divCount, 0.0f);
}

Vector2 ScreenManager::GetScreenEndPos(int divCount) {

	if (divCount < 0)
		return Vector2::Zero;

	return Vector2((float)GetScreenDivWidth() * (divCount + 1), WindowSetting::Inst().height);
}

void ScreenManager::CreateScreen(const ScreenData& data) {

	if (m_hTemporaryScreens == nullptr)
		return;

	if (m_hTemporaryScreens->contains(data.tag))
		return;

	(*m_hTemporaryScreens)[data.tag] = new ScreenData(data);
}

void ScreenManager::CreateScreen(const std::string& tag, int drawX, int drawY, int drawW, int drawH) {

	if (m_hTemporaryScreens == nullptr)
		return;

	if (m_hTemporaryScreens->contains(tag))
		return;

	ScreenData* data = new ScreenData();
	data->tag = tag;
	data->drawX = drawX;
	data->drawY = drawY;
	data->drawW = drawW;
	data->drawH = drawH;

	(*m_hTemporaryScreens)[tag] = data;
}

void ScreenManager::DrawBegin(const std::string& tag) {

	if (m_hTemporaryScreens == nullptr)
		return;

	if (not m_hTemporaryScreens->contains(tag))
		return;

	ScreenData* data = (*m_hTemporaryScreens)[tag];

	if (data->handle == -1) {
		data->handle = MakeScreen(data->clipW, data->clipH, TRUE);
	}

	SetDrawScreen(data->handle);
	ClearDrawScreen();
	//CameraManager::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawBox(data->clipX, data->clipY, data->clipX + data->clipW, data->clipY + data->clipH, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_DrawingScreens->push_back(data);
}

void ScreenManager::DrawEnd(const std::string& tag) {

	if (m_hTemporaryScreens == nullptr)
		return;

	if (not m_hTemporaryScreens->contains(tag))
		return;

	ScreenData* data = (*m_hTemporaryScreens)[tag];

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // アルファ合成

	DrawExtendGraph(data->drawX, data->drawY, data->drawX + data->drawW, data->drawY + data->drawH, data->handle, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // アルファ合成

	DeleteGraph(data->handle);
	data->handle = -1;
}

void ScreenManager::CleanUp() {

	if (m_DrawingScreens == nullptr)
		return;

	//ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // アルファ合成

	for (auto it = m_DrawingScreens->begin(); it != m_DrawingScreens->end();) {
		ScreenData* clip = *it;
		if (clip->handle != -1) {

			DrawExtendGraph(clip->drawX, clip->drawY, clip->drawX + clip->drawW, clip->drawY + clip->drawH, clip->handle, TRUE);

			DeleteGraph(clip->handle);
			clip->handle = -1;
		}
		it = m_DrawingScreens->erase(it);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // アルファ合成
}

void ScreenManager::SetClipPos(const std::string& tag, int x, int y) {

	if (not IsScreenExist(tag))
		return;

	ScreenData* data = (*m_hTemporaryScreens)[tag];
	data->clipX = x;
	data->clipY = y;
}

void ScreenManager::SetClipSize(const std::string& tag, int w, int h) {

	if (not IsScreenExist(tag))
		return;

	ScreenData* data = (*m_hTemporaryScreens)[tag];
	data->clipW = w;
	data->clipH = h;
}

void ScreenManager::SetDrawPos(const std::string& tag, int x, int y) {

	if (not IsScreenExist(tag))
		return;

	ScreenData* data = (*m_hTemporaryScreens)[tag];
	data->drawX = x;
	data->drawY = y;
}

void ScreenManager::SetDrawSize(const std::string& tag, int w, int h) {

	if (not IsScreenExist(tag))
		return;

	ScreenData* data = (*m_hTemporaryScreens)[tag];
	data->drawW = w;
	data->drawH = h;
}

ScreenData* ScreenManager::GetScreenData(const std::string& tag) {

	if (not IsScreenExist(tag))
		return nullptr;

	return (*m_hTemporaryScreens)[tag];
}

bool ScreenManager::IsScreenExist(const std::string& tag) {

	if (m_hTemporaryScreens == nullptr)
		return false;

	if (not m_hTemporaryScreens->contains(tag))
		return false;

	return true;
}
