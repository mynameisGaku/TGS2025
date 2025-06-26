#include "src/common/camera/cameraManager.h"

// ◇汎用
#include <vendor/magic_enum/magic_enum.hpp>
#include <vector>
#include <assert.h>
#include <src/util/ptr/PtrUtil.h>

// ◇演出・機能
#include "src/util/debug/imgui/imGuiManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/util/string/StringUtil.h"
#include "src/util/input/InputManager.h"
#include "src/common/setting/window/WindowSetting.h"

using namespace CameraDefine;

namespace {

	std::vector<Camera*>* cameras;	// カメラ
	bool isScreenDivision;			// 画面分割の有無

	static int screenDivBeginX = 0;	// 画面分割の開始X座標
	static int screenDivBeginY = 0;	// 画面分割の開始Y座標
	static int screenDivWidth = (int)WindowSetting::Inst().width;	// 画面分割の幅
	static int screenDivHeight = (int)WindowSetting::Inst().height;	// 画面分割の高さ
}

void CameraManager::Init() {

	CAMERADEFINE_REF.Load();

	isScreenDivision = false;

	if (cameras == nullptr)
		cameras = new std::vector<Camera*>();

	// 視野角の設定
	SetupCamera_Perspective(CAMERADEFINE_REF.m_Perspective);

	// カメラの描画範囲
	SetCameraNearFar(CAMERADEFINE_REF.m_Near, CAMERADEFINE_REF.m_Far);

	Camera* mainCamera = CreateCamera();
	Camera* camera2P = CreateCamera();
	//Camera* camera3P = CreateCamera();

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

	if (InputManager::Push(KeyDefine::KeyCode::U)) {
		for (const auto& c : *cameras) {
			c->SetIsView(!c->IsView());
		}
	}
}

void CameraManager::Draw() {

	// 画面分割されていない場合は、メインカメラのみ描画する
	if (cameras == nullptr || isScreenDivision)
		return;

	MainCamera()->Draw();
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

	PtrUtil::SafeDelete(cameras);
}

Camera* CameraManager::CreateCamera() {

	if (cameras == nullptr)
		return nullptr;

	Camera* newCamera = new Camera();
	newCamera->SetHolderCharaIndex((int)cameras->size());
	newCamera->ChangeState(&Camera::ChaseState);
	cameras->push_back(newCamera);
	return newCamera;
}

bool CameraManager::CheckNumber(int number) {

	if (cameras == nullptr)
		return false;

	if (number < 0 || cameras->size() <= number)
		return false;

	return true;
}

void CameraManager::ResetCamera(int number) {

	if (CheckNumber(number) == false)
		return;

	(*cameras)[number]->Reset();
}

void CameraManager::ChangeStateCamera(int number, void(Camera::* state)(FSMSignal)) {

	if (CheckNumber(number) == false)
		return;

	(*cameras)[number]->ChangeState(state);
}

void CameraManager::CameraScreenDivision(int x, int y, int w, int h) {

	int x2 = x + w;
	int y2 = y + h;

	SetDrawArea(x, y, x2, y2);

	float centerX = (x + x2) * 0.5f;
	float centerY = y2 * 0.5f;
	SetCameraScreenCenter(centerX, centerY);

	screenDivBeginX = x;
	screenDivBeginY = y;
	screenDivWidth = w;
	screenDivHeight = h;
}

void CameraManager::CameraScreenDivisionDraw(int x, int y, int w, int h, int number) {

	if (not CheckNumber(number))
		return;

	(*cameras)[number]->Draw();
	CameraScreenDivision(x, y, w, h);
}

void CameraManager::ApplyScreenDivision() {

	CameraScreenDivision(screenDivBeginX, screenDivBeginY, screenDivWidth, screenDivHeight);
}

void CameraManager::DefaultScreenSize() {

	SetDrawArea(0, 0, (int)WindowSetting::Inst().width, (int)WindowSetting::Inst().height);
	SetCameraScreenCenter(WindowSetting::Inst().width * 0.5f, WindowSetting::Inst().height * 0.5f);
}

void CameraManager::SetCameraWork(int number, const std::string& type) {

	if (CameraManager::CheckNumber(number) == false)
		return;

	(*cameras)[number]->SetPerformance(type);
}

void CameraManager::SetIsScreenDivision(bool value) {

	isScreenDivision = value;
}

//void CameraManager::CameraChangeStateTheString(const std::string& state) {
//
//	for (int i = 0; i < State::sMax; i++) {
//		State stateValue = static_cast<State>(i);
//		std::string stateName = magic_enum::enum_name(stateValue).data();
//		if (state == stateName) {
//			//MainCamera()->ChangeState(stateValue);
//			break;
//		}
//	}
//}

Camera* CameraManager::MainCamera() {

	return GetCamera(0);
}

Camera* CameraManager::GetCamera(int index) {

	if (CheckNumber(index) == false)
		return nullptr;

	return (*cameras)[index];
}

std::vector<Camera*> CameraManager::AllCameras() {

	return *cameras;
}

bool CameraManager::IsScreenDivision()
{
	return isScreenDivision;
}

Vector2 CameraManager::GetScreenDivisionPos() {

	return Vector2(screenDivBeginX, screenDivBeginY);
}

Vector2 CameraManager::GetScreenDivisionSize() {

	return Vector2(screenDivWidth, screenDivHeight);
}

Vector2 CameraManager::GetScreenDivisionPos_CameraIndex(int index) {

	return Vector2((WindowSetting().width / cameras->size()) * index, 0.0f);
}

Vector2 CameraManager::GetScreenDivisionCenter() {

	return GetScreenDivisionPos() + (GetScreenDivisionSize() * 0.5f);
}

Vector2 CameraManager::GetDivedByCameraNum() {

	return Vector2(WindowSetting::Inst().width / AllCameras().size(), WindowSetting::Inst().height / AllCameras().size());
}

#ifdef IMGUI

void CameraManager::InitImGuiNode() {

	if (cameras == nullptr)
		return;

	ImGuiRoot* CameraTree = ImGuiManager::AddRoot(new ImGuiRoot("Camera"));	// カメラのツリー

	ImGuiRoot* CameraPreviewTree = CameraTree->AddChild(new ImGuiRoot("Preview"));	// カメラの情報を閲覧するツリー
	ImGuiRoot* CameraSettingTree = CameraTree->AddChild(new ImGuiRoot("Setting"));	// カメラの情報を変更するツリー

	for (int i = 0; i < cameras->size(); i++) {

		//==========================================================================================
		// ▼変数

		std::string cNumber = StringUtil::FormatToString("Camera %dP", i + 1);

		Vector3 cPos = (*cameras)[i]->transform->Global().position;	// カメラの座標
		Vector3 cRot = (*cameras)[i]->transform->Global().rotation;	// カメラの回転

		// 回転を弧度法に変換
		cRot = Vector3(MathUtil::ToDegrees(cRot.x), MathUtil::ToDegrees(cRot.y), MathUtil::ToDegrees(cRot.z));

		Vector3 cWorldPos = (*cameras)[i]->WorldPos();	// カメラの絶対座標
		Vector3 cOffset = (*cameras)[i]->Offset();		// カメラの相対座標
		Vector3 cTarget = (*cameras)[i]->Target();		// カメラの注視点

		//==========================================================================================
		// ▼情報閲覧

		// ◆CPTN = CameraPreviewTreeNumbering(カメラ一つ一つに対しての、情報閲覧するツリーを構築)
		ImGuiRoot* CPTN = CameraPreviewTree->AddChild(new ImGuiRoot(cNumber + " Preview"));

		CPTN->NodeBeginChild(250.0f, 100.0f);

		// ▽座標
		CPTN->Add(new ImGuiNode_Text(cNumber + " Position Preview",	StringUtil::FormatToString("Position X %.1f Y %.1f Z %.1f", cPos.x, cPos.y, cPos.z)));

		// ▽回転
		CPTN->Add(new ImGuiNode_Text(cNumber + " Rotation Preview",	StringUtil::FormatToString("Rotation X %.1f Y %.1f Z %.1f", cRot.x, cRot.y, cRot.z)));
		
		// ▽絶対座標
		CPTN->Add(new ImGuiNode_Text(cNumber + " WorldPos Preview", StringUtil::FormatToString("cWorldPos X %.1f Y %.1f Z %.1f", cWorldPos.x, cWorldPos.y, cWorldPos.z)));

		// ▽相対座標
		CPTN->Add(new ImGuiNode_Text(cNumber + " Offset Preview",	StringUtil::FormatToString("Offset X % .1f Y % .1f Z % .1f", cOffset.x, cOffset.y, cOffset.z)));
		
		// ▽注視点
		CPTN->Add(new ImGuiNode_Text(cNumber + " Target Preview",	StringUtil::FormatToString("Target X % .1f Y % .1f Z % .1f", cTarget.x, cTarget.y, cTarget.z)));
		
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
		CSTN->Add(new ImGuiNode_SliderFloat("Rotation X", &(*cameras)[i]->transform->rotation.x, MathUtil::ToDegrees(-360.0f), MathUtil::ToDegrees(360.0f)));
		CSTN->Add(new ImGuiNode_SliderFloat("Rotation Y", &(*cameras)[i]->transform->rotation.y, MathUtil::ToDegrees(-360.0f), MathUtil::ToDegrees(360.0f)));
		CSTN->Add(new ImGuiNode_SliderFloat("Rotation Z", &(*cameras)[i]->transform->rotation.z, MathUtil::ToDegrees(-360.0f), MathUtil::ToDegrees(360.0f)));
		CSTN->NodeEndChild();

		// ▽相対座標
		//CSTN->NodeBeginChild(250.0f, 85.0f);
		//CSTN->Add(new ImGuiNode_SliderFloat("Offset X", &(*cameras)[i]->OffsetPtr().x, -2000.0f, 2000.0f));
		//CSTN->Add(new ImGuiNode_SliderFloat("Offset Y", &(*cameras)[i]->OffsetPtr().y, -2000.0f, 2000.0f));
		//CSTN->Add(new ImGuiNode_SliderFloat("Offset Z", &(*cameras)[i]->OffsetPtr().z, -2000.0f, 2000.0f));
		//CSTN->NodeEndChild();

		// ▽注視点
		//CSTN->NodeBeginChild(250.0f, 85.0f);
		//CSTN->Add(new ImGuiNode_SliderFloat("Target X", &(*cameras)[i]->TargetPtr().x, -5000.0f, 5000.0f));
		//CSTN->Add(new ImGuiNode_SliderFloat("Target Y", &(*cameras)[i]->TargetPtr().y, -5000.0f, 5000.0f));
		//CSTN->Add(new ImGuiNode_SliderFloat("Target Z", &(*cameras)[i]->TargetPtr().z, -5000.0f, 5000.0f));
		//CSTN->NodeEndChild();

		//==========================================================================================
		// ▼ステート変更
		//{
		//	// ◆CSSTN = CameraStateSettingTreeNumbering(カメラ一つ一つに対しての、ステート変更するツリーを構築)
		//	ImGuiRoot* CSSTN = CameraSettingTree->AddChild(new ImGuiRoot(cNumber + " State"));

		//	std::vector<std::string> stateName;

		//	for (int i = 0; i < static_cast<int>(State::sMax); i++)
		//		stateName.push_back(static_cast<std::string>(magic_enum::enum_name(static_cast<State>(i))));

		//	CSSTN->Add(new ImGuiNode_DropDown("State Change", stateName, [](std::string s) { CameraManager::CameraChangeStateTheString(s); }));
		//}

		//==========================================================================================
		// ▼演出変更
		//{
		//	// ◆CPTN = CameraPerformanceTreeNumbering(カメラ一つ一つに対しての、演出変更するツリーを構築)
		//	ImGuiRoot* CPTN = CameraSettingTree->AddChild(new ImGuiRoot(cNumber + " Performance"));

		//	std::vector<std::string> cameraPerformanceName;

		//	for (int i = 0; i < static_cast<int>(CameraWorkType::Max); i++)
		//		cameraPerformanceName.push_back(static_cast<std::string>(magic_enum::enum_name(static_cast<CameraWorkType>(i))));

		//	CPTN->Add(new ImGuiNode_DropDown("Camera Performance", cameraPerformanceName, [](std::string s) { CameraManager::SetCameraWork(0, s); }));
		//}

		CSTN->Add(new ImGuiNode_Button("Reset", std::bind(ResetCamera, i)));
	}

	CameraTree->Add(new ImGuiNode_Button("Reset All", []() {
		if (cameras == nullptr)
			return;
		for (int i = 0; i < cameras->size(); i++) {
			(*cameras)[i]->Reset();
		}
	}
	)
	);

	CameraTree->Add(new ImGuiNode_CheckBox("ScreenDivision", &isScreenDivision));
}

void CameraManager::UpdateImGuiNode() {

	if (cameras == nullptr)
		return;


	for (int i = 0; i < cameras->size(); i++) {

		//==========================================================================================
		// ▼変数

		std::string cNumber = StringUtil::FormatToString("Camera %dP", i + 1);

		Vector3 cPos = (*cameras)[i]->transform->Global().position;	// カメラの座標
		Vector3 cRot = (*cameras)[i]->transform->Global().rotation;	// カメラの回転

		// 回転を弧度法に変換
		cRot = Vector3(MathUtil::ToDegrees(cRot.x), MathUtil::ToDegrees(cRot.y), MathUtil::ToDegrees(cRot.z));

		Vector3 cWorldPos = (*cameras)[i]->WorldPos();	// カメラの絶対座標
		Vector3 cOffset = (*cameras)[i]->Offset();		// カメラの相対座標
		Vector3 cTarget = (*cameras)[i]->Target();		// カメラの注視点

		//==========================================================================================
		// ▼情報閲覧

		// ◆CPTN = CameraPreviewTreeNumbering(カメラ一つ一つに対しての、情報閲覧するツリーを構築)
		ImGuiRoot* CPTN = ImGuiManager::FindRoot("Camera")->SearchChildren(cNumber + " Preview");
		if (CPTN != nullptr)
		{
			// ▽座標
			CPTN->Node<ImGuiNode_Text>(cNumber + " Position Preview")->SetText(StringUtil::FormatToString("Position X %.1f Y %.1f Z %.1f", cPos.x, cPos.y, cPos.z));

			// ▽回転
			CPTN->Node<ImGuiNode_Text>(cNumber + " Rotation Preview")->SetText(StringUtil::FormatToString("Rotation X %.1f Y %.1f Z %.1f", cRot.x, cRot.y, cRot.z));

			// ▽絶対座標
			CPTN->Node<ImGuiNode_Text>(cNumber + " WorldPos Preview")->SetText(StringUtil::FormatToString("WorldPos X %.1f Y %.1f Z %.1f", cWorldPos.x, cWorldPos.y, cWorldPos.z));

			// ▽相対座標
			CPTN->Node<ImGuiNode_Text>(cNumber + " Offset Preview")->SetText(StringUtil::FormatToString("Offset X %.1f Y %.1f Z %.1f", cOffset.x, cOffset.y, cOffset.z));

			// ▽注視点
			CPTN->Node<ImGuiNode_Text>(cNumber + " Target Preview")->SetText(StringUtil::FormatToString("Target X %.1f Y %.1f Z %.1f", cTarget.x, cTarget.y, cTarget.z));
		}
	}
}

#endif // IMGUI
