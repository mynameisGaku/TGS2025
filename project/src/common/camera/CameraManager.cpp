#include "cameraManager.h"

// ◇汎用
#include "vendor/magic_enum/magic_enum.hpp"
#include "src/Util/Utils.h"
#include <vector>
#include <assert.h>

// ◇演出・機能
#include "../Source/ImGuiTree/imGuiManager.h"
#include "CameraDefineRef.h"

using namespace CameraDefine;

namespace {

	std::vector<Camera*>* cameras;
}

void CameraManager::Init() {

	CAMERADEFINE_REF.Load();

	if (cameras == nullptr)
		cameras = new std::vector<Camera*>();

	// 視野角の設定
	SetupCamera_Perspective(CAMERADEFINE_REF.m_Perspective);

	// カメラの描画範囲
	SetCameraNearFar(CAMERADEFINE_REF.m_Near, CAMERADEFINE_REF.m_Far);

	cameras->push_back(new Camera());

#ifdef IMGUI
	InitImGuiNode();
#endif

}

void CameraManager::Update() {

	if (cameras == nullptr)
		return;

	for (const auto& c : *cameras) {
		c->Update();
	}

#ifdef IMGUI
	UpdateImGuiNode();
#endif

}

void CameraManager::Draw() {

	if (cameras == nullptr)
		return;

	for (const auto& c : *cameras)
		c->Draw();
}

void CameraManager::Release() {

	if (cameras == nullptr)
		return;

#ifdef IMGUI
	ImGuiManager::Delete("Camera");
#endif

	for (auto itr = cameras->begin(); itr != cameras->end();) {
		delete* itr;
		itr = cameras->erase(itr);
	}

	Function::DeletePointer(cameras);
}

bool CameraManager::CheckNumber(const int& number) {

	if (cameras == nullptr)
		return false;

	if (number < 0 || cameras->size() < number)
		return false;

	return true;
}

void CameraManager::ResetCamera(const int& number) {

	if (CheckNumber(number) == false)
		return;

	(*cameras)[number]->Reset();
}

void CameraManager::ChangeStateCamera(const int& number, void(Camera::* state)(FSMSignal)) {

	if (CheckNumber(number) == false)
		return;

	(*cameras)[number]->ChangeState(state);
}

void CameraManager::SetCameraWork(const int& number, const std::string& type) {

	if (CheckNumber(number) == false)
		return;

	(*cameras)[number]->SetPerformance(type);
}

void CameraManager::CameraChangeStateTheString(const std::string& state) {

	for (int i = 0; i < State::sMax; i++) {
		State stateValue = static_cast<State>(i);
		std::string stateName = magic_enum::enum_name(stateValue).data();
		if (state == stateName) {
			//MainCamera()->ChangeState(stateValue);
			break;
		}
	}
}

Camera* CameraManager::MainCamera() {

	return GetCamera(0);
}

Camera* CameraManager::GetCamera(const int& index) {

	if (CheckNumber(index) == false)
		return nullptr;

	return (*cameras)[index];
}

std::vector<Camera*> CameraManager::AllCameras() {

	return *cameras;
}

#ifdef IMGUI

void CameraManager::InitImGuiNode() {

	if (cameras == nullptr)
		return;

	using namespace Math;
	using namespace Function;

	ImGuiRoot* CameraTree = ImGuiManager::AddRoot(new ImGuiRoot("Camera"));	// カメラのツリー

	ImGuiRoot* CameraPreviewTree = CameraTree->AddChild(new ImGuiRoot("Preview"));	// カメラの情報を閲覧するツリー
	ImGuiRoot* CameraSettingTree = CameraTree->AddChild(new ImGuiRoot("Setting"));	// カメラの情報を変更するツリー

	for (int i = 0; i < cameras->size(); i++) {

		//==========================================================================================
		// ▼変数

		std::string cNumber = FormatToString("Camera %dP", i + 1);

		Vector3 cPos = (*cameras)[i]->transform->Global().position;	// カメラの座標
		Vector3 cRot = (*cameras)[i]->transform->Global().rotation;	// カメラの回転

		// 回転を弧度法に変換
		cRot = Vector3(RadToDeg(cRot.x), RadToDeg(cRot.y), RadToDeg(cRot.z));

		Vector3 cWorldPos = (*cameras)[i]->WorldPos();	// カメラの絶対座標
		Vector3 cOffset = (*cameras)[i]->Offset();		// カメラの相対座標
		Vector3 cTarget = (*cameras)[i]->Target();		// カメラの注視点

		//==========================================================================================
		// ▼情報閲覧

		// ◆CPTN = CameraPreviewTreeNumbering(カメラ一つ一つに対しての、情報閲覧するツリーを構築)
		ImGuiRoot* CPTN = CameraPreviewTree->AddChild(new ImGuiRoot(cNumber + " Preview"));

		CPTN->NodeBeginChild(250.0f, 100.0f);

		// ▽座標
		CPTN->Add(new ImGuiNode_Text(cNumber + " Position Preview",	FormatToString("Position X %.1f Y %.1f Z %.1f", cPos.x, cPos.y, cPos.z)));

		// ▽回転
		CPTN->Add(new ImGuiNode_Text(cNumber + " Rotation Preview",	FormatToString("Rotation X %.1f Y %.1f Z %.1f", cRot.x, cRot.y, cRot.z)));
		
		// ▽絶対座標
		CPTN->Add(new ImGuiNode_Text(cNumber + " WorldPos Preview", FormatToString("cWorldPos X %.1f Y %.1f Z %.1f", cWorldPos.x, cWorldPos.y, cWorldPos.z)));

		// ▽相対座標
		CPTN->Add(new ImGuiNode_Text(cNumber + " Offset Preview",	FormatToString("Offset X % .1f Y % .1f Z % .1f", cOffset.x, cOffset.y, cOffset.z)));
		
		// ▽注視点
		CPTN->Add(new ImGuiNode_Text(cNumber + " Target Preview",	FormatToString("Target X % .1f Y % .1f Z % .1f", cTarget.x, cTarget.y, cTarget.z)));
		
		CPTN->NodeEndChild();

		//==========================================================================================
		// ▼情報変更

		// ◆CSTN = CameraSettingTreeNumbering(カメラ一つ一つに対しての、情報変更するツリーを構築)
		ImGuiRoot* CSTN = CameraSettingTree->AddChild(new ImGuiRoot(cNumber + " Settting"));

		// ▽座標
		CSTN->NodeBeginChild(250.0f, 85.0f);
		CSTN->Add(new ImGuiNode_SliderFloat("Position X", &(*cameras)[i]->transform->position.x, -5000.0f, 5000.0f));
		CSTN->Add(new ImGuiNode_SliderFloat("Position Y", &(*cameras)[i]->transform->position.y, -5000.0f, 5000.0f));
		CSTN->Add(new ImGuiNode_SliderFloat("Position Z", &(*cameras)[i]->transform->position.z, -5000.0f, 5000.0f));
		CSTN->NodeEndChild();

		// ▽回転
		CSTN->NodeBeginChild(250.0f, 85.0f);
		CSTN->Add(new ImGuiNode_SliderFloat("Rotation X", &(*cameras)[i]->transform->rotation.x, RadToDeg(-360.0f), RadToDeg(360.0f)));
		CSTN->Add(new ImGuiNode_SliderFloat("Rotation Y", &(*cameras)[i]->transform->rotation.y, RadToDeg(-360.0f), RadToDeg(360.0f)));
		CSTN->Add(new ImGuiNode_SliderFloat("Rotation Z", &(*cameras)[i]->transform->rotation.z, RadToDeg(-360.0f), RadToDeg(360.0f)));
		CSTN->NodeEndChild();

		// ▽相対座標
		CSTN->NodeBeginChild(250.0f, 85.0f);
		CSTN->Add(new ImGuiNode_SliderFloat("Offset X", &(*cameras)[i]->OffsetPtr().x, -2000.0f, 2000.0f));
		CSTN->Add(new ImGuiNode_SliderFloat("Offset Y", &(*cameras)[i]->OffsetPtr().y, -2000.0f, 2000.0f));
		CSTN->Add(new ImGuiNode_SliderFloat("Offset Z", &(*cameras)[i]->OffsetPtr().z, -2000.0f, 2000.0f));
		CSTN->NodeEndChild();

		// ▽注視点
		CSTN->NodeBeginChild(250.0f, 85.0f);
		CSTN->Add(new ImGuiNode_SliderFloat("Target X", &(*cameras)[i]->TargetPtr().x, -5000.0f, 5000.0f));
		CSTN->Add(new ImGuiNode_SliderFloat("Target Y", &(*cameras)[i]->TargetPtr().y, -5000.0f, 5000.0f));
		CSTN->Add(new ImGuiNode_SliderFloat("Target Z", &(*cameras)[i]->TargetPtr().z, -5000.0f, 5000.0f));
		CSTN->NodeEndChild();

		//==========================================================================================
		// ▼ステート変更
		{
			// ◆CSSTN = CameraStateSettingTreeNumbering(カメラ一つ一つに対しての、ステート変更するツリーを構築)
			ImGuiRoot* CSSTN = CameraSettingTree->AddChild(new ImGuiRoot(cNumber + " State"));

			std::vector<std::string> stateName;

			for (int i = 0; i < static_cast<int>(State::sMax); i++)
				stateName.push_back(static_cast<std::string>(magic_enum::enum_name(static_cast<State>(i))));

			CSSTN->Add(new ImGuiNode_DropDown("State Change", stateName, [](std::string s) { CameraManager::CameraChangeStateTheString(s); }));
		}

		//==========================================================================================
		// ▼演出変更
		{
			// ◆CPTN = CameraPerformanceTreeNumbering(カメラ一つ一つに対しての、演出変更するツリーを構築)
			ImGuiRoot* CPTN = CameraSettingTree->AddChild(new ImGuiRoot(cNumber + " Performance"));

			std::vector<std::string> cameraPerformanceName;

			for (int i = 0; i < static_cast<int>(CameraWorkType::Max); i++)
				cameraPerformanceName.push_back(static_cast<std::string>(magic_enum::enum_name(static_cast<CameraWorkType>(i))));

			CPTN->Add(new ImGuiNode_DropDown("Camera Performance", cameraPerformanceName, [](std::string s) { CameraManager::SetCameraWork(0, s); }));
		}

		CSTN->Add(new ImGuiNode_Button("Reset", std::bind(ResetCamera, i)));
	}
}

void CameraManager::UpdateImGuiNode() {

	if (cameras == nullptr)
		return;

	using namespace Math;
	using namespace Function;

	for (int i = 0; i < cameras->size(); i++) {

		//==========================================================================================
		// ▼変数

		std::string cNumber = FormatToString("Camera %dP", i + 1);

		Vector3 cPos = (*cameras)[i]->transform->Global().position;	// カメラの座標
		Vector3 cRot = (*cameras)[i]->transform->Global().rotation;	// カメラの回転

		// 回転を弧度法に変換
		cRot = Vector3(RadToDeg(cRot.x), RadToDeg(cRot.y), RadToDeg(cRot.z));

		Vector3 cWorldPos = (*cameras)[i]->WorldPos();	// カメラの絶対座標
		Vector3 cOffset = (*cameras)[i]->Offset();		// カメラの相対座標
		Vector3 cTarget = (*cameras)[i]->Target();		// カメラの注視点

		//==========================================================================================
		// ▼情報閲覧

		// ◆CPTN = CameraPreviewTreeNumbering(カメラ一つ一つに対しての、情報閲覧するツリーを構築)
		ImGuiRoot* CPTN = ImGuiManager::FindRoot("Camera")->SearchChildren(cNumber + " Preview");

		// ▽座標
		CPTN->Node<ImGuiNode_Text>(cNumber + " Position Preview")->SetText(FormatToString("Position X %.1f Y %.1f Z %.1f", cPos.x, cPos.y, cPos.z));
		
		// ▽回転
		CPTN->Node<ImGuiNode_Text>(cNumber + " Rotation Preview")->SetText(FormatToString("Rotation X %.1f Y %.1f Z %.1f", cRot.x, cRot.y, cRot.z));
		
		// ▽絶対座標
		CPTN->Node<ImGuiNode_Text>(cNumber + " WorldPos Preview")->SetText(FormatToString("WorldPos X %.1f Y %.1f Z %.1f", cWorldPos.x, cWorldPos.y, cWorldPos.z));
		
		// ▽相対座標
		CPTN->Node<ImGuiNode_Text>(cNumber + " Offset Preview")->SetText(FormatToString("Offset X %.1f Y %.1f Z %.1f", cOffset.x, cOffset.y, cOffset.z));
		
		// ▽注視点
		CPTN->Node<ImGuiNode_Text>(cNumber + " Target Preview")->SetText(FormatToString("Target X %.1f Y %.1f Z %.1f", cTarget.x, cTarget.y, cTarget.z));
	}
}

#endif // IMGUI
