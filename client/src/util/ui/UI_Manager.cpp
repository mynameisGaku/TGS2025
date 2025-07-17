#include "src/util/ui/UI_Manager.h"

// ◇汎用
#include "src/util/ptr/PtrUtil.h"
#include "src/util/screen/ScreenManager.h"

namespace {

	std::vector<UI_Canvas*>* backCanvases;	// 管理するUI(背面表示)
	std::vector<UI_Canvas*>* frontCanvases;	// 管理するUI(正面表示)

	bool isUpdate;	// 更新するか
	bool isDraw;	// 描画するか
}

void UI_Manager::Init() {

	if (backCanvases == nullptr)
		backCanvases = new std::vector<UI_Canvas*>;

	if (frontCanvases == nullptr)
		frontCanvases = new std::vector<UI_Canvas*>;

	isUpdate = true;
	isDraw = true;
}

void UI_Manager::Update() {

	if (isUpdate == false)
		return;

	if (backCanvases != nullptr) {
		for (const auto& itr : *backCanvases) {
			if (itr->IsUpdate())
				itr->Update();
		}
	}

	if (frontCanvases != nullptr) {
		for (const auto& itr : *frontCanvases) {
			if (itr->IsUpdate())
				itr->Update();
		}
	}
}

void UI_Manager::DrawBack() {

	if (isDraw == false)
		return;

	if (backCanvases != nullptr) {
		for (const auto& itr : *backCanvases) {
			if (itr->IsDraw())
				itr->Draw();
		}
	}
}

void UI_Manager::DrawFront() {

	if (isDraw == false)
		return;

	if (frontCanvases != nullptr) {
		for (const auto& itr : *frontCanvases) {
			if (itr->IsDraw())
				itr->Draw();
		}
	}
}

void UI_Manager::Release() {

	if (backCanvases != nullptr) {
		for (auto itr = backCanvases->begin(); itr != backCanvases->end();) {
			if ((*itr) != nullptr) {
				delete* itr;
				itr = backCanvases->erase(itr);
				if (itr == backCanvases->end())
					break;
			}
			else
				itr++;
		}

		backCanvases->clear();
		PtrUtil::SafeDelete(backCanvases);
	}

	if (frontCanvases != nullptr) {
		for (auto itr = frontCanvases->begin(); itr != frontCanvases->end();) {
			if ((*itr) != nullptr) {
				delete* itr;
				itr = frontCanvases->erase(itr);
				if (itr == frontCanvases->end())
					break;
			}
			else {
				itr++;
			}
		}

		frontCanvases->clear();
		PtrUtil::SafeDelete(frontCanvases);
	}
}

UI_Canvas* UI_Manager::Add(UI_Canvas* uiData, const bool& front) {

	if (front)
		frontCanvases->push_back(uiData);
	else
		backCanvases->push_back(uiData);

	return uiData;
}

bool UI_Manager::Detach(UI_Canvas* uiData) {

	if (uiData == nullptr)
		return true;

	bool result = false;

	for (auto itr = backCanvases->begin(); itr != backCanvases->end();) {
		if ((*itr) == uiData) {
			itr = backCanvases->erase(itr);
			result = true;
			break;
		}
		else
			itr++;
	}

	for (auto itr = frontCanvases->begin(); itr != frontCanvases->end();) {
		if ((*itr) == uiData) {
			itr = frontCanvases->erase(itr);
			result = true;
			break;
		}
		else
			itr++;
	}

	return result;
}

UI_Canvas* UI_Manager::Find(const std::string& tag) {

	if (backCanvases != nullptr) {
		for (const auto& itr : *backCanvases) {
			if (itr->IsTag(tag))
				return itr;
		}
	}

	if (frontCanvases != nullptr) {
		for (const auto& itr : *frontCanvases) {
			if (itr->IsTag(tag))
				return itr;
		}
	}

	return nullptr;
}

void UI_Manager::SetIsActive(const bool& value) { isUpdate = isDraw = value; }
void UI_Manager::SetIsUpdate(const bool& value) { isUpdate = value; }
void UI_Manager::SetIsDraw(const bool& value) { isDraw = value; }

bool UI_Manager::IsActive() { return isUpdate && isDraw; }
bool UI_Manager::IsUpdate() {	return isUpdate; }
bool UI_Manager::IsDraw() { return isDraw; }

void UI_Manager::Sort() {

	if (backCanvases != nullptr && backCanvases->size() > 1)
		UI_Manager::UI_Canvas_CombSort(*backCanvases);

	if (frontCanvases != nullptr && frontCanvases->size() > 1)
		UI_Manager::UI_Canvas_CombSort(*frontCanvases);
}

void UI_Manager::UI_Canvas_CombSort(std::vector<UI_Canvas*>& ui) {

	size_t h = (ui.size() * 10) / 13;
	bool is_sorted = false;
	while (!is_sorted) {
		if (h == 1)is_sorted = true;
		for (size_t i = 0; i < ui.size() - h; ++i) {
			if (ui[i]->Priority() > ui[i + h]->Priority()) {
				std::swap(ui[i], ui[i + h]);
				if (is_sorted)is_sorted = false;
			}
		}
		if (h > 1) h = (h * 10) / 13;
		if (h == 0) h = 1;
	}
}

void UI_Manager::SetAnchorPositionByScreenSplit(UI_Canvas* ui, int cameraIndex) {

	if (ui == nullptr)
		return;

	Vector2 beginPos = ScreenManager::GetScreenBeginPos(cameraIndex);
	Vector2 endPos = ScreenManager::GetScreenEndPos(cameraIndex);

	ui->rectTransform->anchor.SetBegin(beginPos);
	ui->rectTransform->anchor.SetEnd(endPos);
}
