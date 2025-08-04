#include "ScreenManager.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/common/camera/CameraManager.h"
#include "framework/myDxLib.h"
#include "src/util/ptr/PtrUtil.h"

namespace {

	std::unordered_map<std::string, ScreenImageData*>* m_hTemporaryScreens;	// 仮スクリーン画像
	std::list<ScreenImageData*>* m_DrawingScreens;	// 描画中のスクリーン画像
	ScreenImageData* m_DefaultScreen;	// 基本のスクリーン画像
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

void ScreenManager::CreateScreen(const ScreenImageData& data) {

	if (m_hTemporaryScreens == nullptr)
		return;

	if (m_hTemporaryScreens->contains(data.tag))
		return;

	(*m_hTemporaryScreens)[data.tag] = new ScreenImageData(data);
}

void ScreenManager::CreateScreen(const std::string& tag, int drawX, int drawY, int drawW, int drawH) {

	if (m_hTemporaryScreens == nullptr)
		return;

	if (m_hTemporaryScreens->contains(tag))
		return;

	ScreenImageData* data = new ScreenImageData();
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

	ScreenImageData* data = (*m_hTemporaryScreens)[tag];

	if (data->handle == -1) {
		data->handle = MakeScreen(data->clipW, data->clipH, TRUE);
	}

	SetDrawScreen(data->handle);
	ClearDrawScreen();

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

	ScreenImageData* data = (*m_hTemporaryScreens)[tag];

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // アルファ合成

	//DrawExtendGraph(data->drawX, data->drawY, data->drawX + data->drawW, data->drawY + data->drawH, data->handle, TRUE);

	int drawCX = data->drawX + data->drawW / 2;
	int drawCY = data->drawY + data->drawH / 2;

	int clipCX = data->clipX + data->clipW / 2;
	int clipCY = data->clipY + data->clipH / 2;

	int clipSX = clipCX - data->drawW / 2;
	int clipSY = 0;

	DrawRectRotaGraph(drawCX, drawCY, clipSX, clipSY, data->drawW, data->drawH, 1.0f, 0.0f, data->handle, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // アルファ合成

	DeleteGraph(data->handle);
	data->handle = -1;
}

void ScreenManager::CleanUp() {

	if (m_DrawingScreens == nullptr)
		return;

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // アルファ合成

	for (auto it = m_DrawingScreens->begin(); it != m_DrawingScreens->end();) {
		ScreenImageData* data = *it;
		if (data->handle != -1) {

			int drawCX = data->drawX + data->drawW / 2;
			int drawCY = data->drawY + data->drawH / 2;

			int clipCX = data->clipX + data->clipW / 2;
			int clipCY = data->clipY + data->clipH / 2;

			int clipSX = clipCX - data->drawW / 2;
			int clipSY = 0;

			DrawRectRotaGraph(drawCX, drawCY, clipSX, clipSY, data->drawW, data->drawH, 1.0f, 0.0f, data->handle, TRUE);

			DeleteGraph(data->handle);
			data->handle = -1;
		}
		it = m_DrawingScreens->erase(it);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // アルファ合成
}

void ScreenManager::SetClipPos(const std::string& tag, int x, int y) {

	if (not IsScreenExist(tag))
		return;

	ScreenImageData* data = (*m_hTemporaryScreens)[tag];
	data->clipX = x;
	data->clipY = y;
}

void ScreenManager::SetClipSize(const std::string& tag, int w, int h) {

	if (not IsScreenExist(tag))
		return;

	ScreenImageData* data = (*m_hTemporaryScreens)[tag];
	data->clipW = w;
	data->clipH = h;
}

void ScreenManager::SetDrawPos(const std::string& tag, int x, int y) {

	if (not IsScreenExist(tag))
		return;

	ScreenImageData* data = (*m_hTemporaryScreens)[tag];
	data->drawX = x;
	data->drawY = y;
}

void ScreenManager::SetDrawSize(const std::string& tag, int w, int h) {

	if (not IsScreenExist(tag))
		return;

	ScreenImageData* data = (*m_hTemporaryScreens)[tag];
	data->drawW = w;
	data->drawH = h;
}

ScreenImageData* ScreenManager::GetScreenImageData(const std::string& tag) {

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
