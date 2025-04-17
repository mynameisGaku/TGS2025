#include "SettingManager.h"

// ���ėp
#include "Util/Utils.h"

// �����o�E�@�\
#include "InputManager.h"
#include "MouseController.h"

// ���ʂŕK�v�ȕ�
#include "SensiSetting.h"
#include "VolumeSetting.h"

using namespace KeyDefine;

namespace {

	static const char* CSV_FILE_NAME = "data/csv/SettingData.csv";
}

SettingManager::SettingManager() {

	state = State::Close;
	curItem = SettingItem::Sensi;

	isDisplay = false;
	canDisplay = true;
}

SettingManager::~SettingManager() {

	std::ofstream file(CSV_FILE_NAME);
	file << "�ݒ荀��\n";
	file.close();	// �t�@�C�������

	for (auto& itr : setting) {
		itr.second->SaveCsv(CSV_FILE_NAME);
		itr.second->Apply();
		Function::DeletePointer(itr.second);
	}

	setting.clear();
}

void SettingManager::Start() {

	setting[SettingItem::Sensi] = new SensiSetting();
	setting[SettingItem::Volume] = new VolumeSetting();

	for (auto& itr : setting) {
		itr.second->LoadCsv(CSV_FILE_NAME);
		itr.second->SetIsAdjustable(false);
	}
}

void SettingManager::Update() {

	if (canDisplay == false)
		return;

	switch (state) {
	case SettingManager::State::Close:	closeStateUpdate();	break;
	case SettingManager::State::Open:	openStateUpdate();	break;
	default:
		break;
	}

	for (auto& itr : setting)
		itr.second->Update();
}

void SettingManager::Draw() {

	for (auto& itr : setting)
		itr.second->Draw();
}

void SettingManager::LoadCsv() {

	for (auto& itr : setting)
		itr.second->LoadCsv(CSV_FILE_NAME);
}

void SettingManager::SaveCsv() {

	for (auto& itr : setting)
		itr.second->SaveCsv(CSV_FILE_NAME);
}

void SettingManager::closeStateUpdate() {

	if (InputManager::Push(KeyCode::Escape)) {
		isDisplay = true;
		state = State::Open;
		setting[curItem]->FadeIn();
		MouseController::SetMouseMovement(MouseMovement::Free);
	}
}

void SettingManager::openStateUpdate() {

	if (InputManager::Push(KeyCode::Escape)) {
		isDisplay = false;
		state = State::Close;
		for (auto& itr : setting)
			itr.second->FadeOut();
		MouseController::SetMouseMovement(MouseMovement::Fixed);
	}

	bool push = false;	// ������Ă��邩
	int nextID = 0;		// ���̐ݒ荀��

	if (InputManager::Push(KeyCode::RightArrow)) {
		nextID = min(static_cast<int>(curItem) + 1, static_cast<int>(SettingItem::Max) - 1);
		push = true;
	}
	else if (InputManager::Push(KeyCode::LeftArrow)) {
		nextID = max(static_cast<int>(curItem) - 1, 0);
		push = true;
	}

	if (push) {
		SettingItem nextItem = static_cast<SettingItem>(nextID);

		if (isDisplay && curItem != nextItem) {
			setting[curItem]->FadeOut();
			setting[nextItem]->FadeIn();
		}

		curItem = nextItem;
	}
}
