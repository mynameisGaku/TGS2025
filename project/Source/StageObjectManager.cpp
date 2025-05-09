#include "stageObjectManager.h"

// ◇汎用
#include <fstream>
#include <assert.h>

#include "../Source/Util/Utils.h"
#include "../Source/ImGuiTree/imGuiManager.h"
#include "../Library/csvReader.h"
#include "../Library/resourceLoader.h"
#include "../Library/magic_enum.hpp"

// ◇演出・機能
#include "InputManager.h"
#include "settings_json.h"

using namespace StageDefine;

namespace {

	// Csvに書き出す際の定型文
	const char* CSV_TITLE_TEMPLATE = "//種類,座標(X軸),座標(Y軸),座標(Z軸),回転(X軸),回転(Y軸),回転(Z軸),拡縮(X軸),拡縮(Y軸),拡縮(Z軸),当たり判定の可否";

	std::string* csvFilePath_StageObjData;	// ステージオブジェクトの配置データを持つファイルパス
	std::string* csvFilePath_StageObjModel;	// ステージオブジェクトのモデルデータを持つファイルパス

	std::vector<StageObject*>* stageObjects;	// ステージオブジェクト

	static unsigned int productionID;	// 製造番号
	bool canSaveCsv;			// CSVデータを保存できるか
	bool initialize = false;	// 初期化処理が行われたか
}

void StageObjectManager::Init() {

	if (stageObjects == nullptr)
		stageObjects = new std::vector<StageObject*>();

	if (csvFilePath_StageObjData == nullptr)
		csvFilePath_StageObjData = new std::string;

	if (csvFilePath_StageObjModel == nullptr)
		csvFilePath_StageObjModel = new std::string;

	productionID = 0;
	canSaveCsv = true;

	initialize = false;
}

void StageObjectManager::Start() {

	if (initialize)
		return;

#ifdef IMGUI
	InitImGui();
#endif // IMGUI

	initialize = true;
}

void StageObjectManager::Update() {

	if (initialize == false)
		Start();

	if (InputManager::Simultaneously({ KeyDefine::KeyCode::LeftControl, KeyDefine::KeyCode::S })) {
		if (canSaveCsv)
			SaveToCsv();
		canSaveCsv = false;
	}
	else
		canSaveCsv = true;

	if (stageObjects != nullptr) {
		for (auto itr : *stageObjects)
			itr->Update();
	}

#ifdef IMGUI
	UpdateImGui();
#endif // IMGUI

}

void StageObjectManager::Draw() {

	SetFogColor(50, 50, 150);
	SetFogStartEnd(150.0f, STAGE_RANGE * 5.0f);
	SetFogEnable(true);

#ifdef _DEBUG

	DrawEditMode();

	if (stageObjects != nullptr) {
		for (auto itr : *stageObjects)
			itr->Draw();
	}

#else

	for (auto itr : *stageObjects)
		itr->Draw();

#endif

	SetFogEnable(false);
}

void StageObjectManager::Release() {

	//SaveToCsv();
	//SaveToJson();
	EraseAll();

	Function::DeletePointer(stageObjects);
	Function::DeletePointer(csvFilePath_StageObjData);
	Function::DeletePointer(csvFilePath_StageObjModel);
}

bool StageObjectManager::CollCheckCapsule(const Vector3& p1, const Vector3& p2, float r, Vector3* push) {

	if (stageObjects == nullptr)
		return false;

	bool hitFlag = false;
	Vector3 pushVec = VGet(0, 0, 0);

	for (const auto& obj : *stageObjects) {

		if (!obj->IsCollider())
			continue;

		MV1SetupCollInfo(obj->Info().hHitModel, -1, 8, 8, 8);
		MV1_COLL_RESULT_POLY_DIM dim = MV1CollCheck_Capsule(obj->Info().hHitModel, -1, p1, p2, r);
		for (int i = 0; i < dim.HitNum; i++) {
			hitFlag = true;
			// 当たり判定をしてpushVecを作る
			// ポリゴンと線分の距離を求める
			float len = Segment_Triangle_MinLength(p1, p2, dim.Dim[i].Position[0], dim.Dim[i].Position[1], dim.Dim[i].Position[2]);
			if (len > r)
				continue;
			// 押し返す量は、(r - len)になる
			// ポリゴンの法線から押し返す量を乗算する
			Vector3 newPush = dim.Dim[i].Normal * (r - len);
			pushVec += newPush;
			Vector3 pushVecNorm = VNorm(pushVec);
			float pushIn = VDot(pushVecNorm, newPush);
			// これが新しく押す量
			if (pushIn < VSize(pushVec)) {
				pushVec += newPush - pushVecNorm * pushIn;
			}
			else {
				pushVec = newPush;
			}
		}
		MV1CollResultPolyDimTerminate(dim);
	}

	if (push != nullptr)
		*push = pushVec;
	return hitFlag;
}

bool StageObjectManager::CollCheckCapsule_Under(const Vector3& begin, const Vector3& end, Vector3* hitPos)
{
	for (const auto& obj : *stageObjects) {

		MV1_COLL_RESULT_POLY hit;
		hit = MV1CollCheck_Line(obj->Model(), 0, begin, end);
		if (hit.HitFlag > 0) {
			if (hitPos != nullptr)	// 引数にポインタが入っていたら、代入をする
				*hitPos = hit.HitPosition;
			return true;
		}
	}
	return false;
}

bool StageObjectManager::CollCheckCapsule_Horizon(const Vector3& begin, const Vector3& end, float r, Vector3* push)
{
	bool hitFlag = false;
	VECTOR pushVec = VGet(0, 0, 0);
	for (const auto& obj : *stageObjects) {

		MV1SetupCollInfo(obj->Model(), -1, 8, 8, 8);
		MV1_COLL_RESULT_POLY_DIM dim = MV1CollCheck_Capsule(obj->Model(), -1, begin, end, r);
		for (int i = 0; i < dim.HitNum; i++) {
			hitFlag = true;
			// 当たり判定をしてpushVecを作る
			// ポリゴンと線分の距離を求める
			float len = Segment_Triangle_MinLength(begin, end, dim.Dim[i].Position[0], dim.Dim[i].Position[1], dim.Dim[i].Position[2]);
			if (len > r)
				continue;
			// 押し返す量は、(r - len)になる
			// ポリゴンの法線から押し返す量を乗算する
			VECTOR newPush = dim.Dim[i].Normal * (r - len);
			pushVec += newPush;
			VECTOR pushVecNorm = VNorm(pushVec);
			float pushIn = VDot(pushVecNorm, newPush);
			// これが新しく押す量
			if (pushIn < VSize(pushVec)) {
				pushVec += newPush - pushIn * pushVecNorm;
			}
			else {
				pushVec = newPush;
			}
		}
		MV1CollResultPolyDimTerminate(dim);

	}

	if (push != nullptr)
		*push = pushVec * -1.0f;
	return hitFlag;
}

void StageObjectManager::LoadToCsv(const std::string& filename) {

	if (stageObjects == nullptr)
		return;

	using namespace Math;

	EraseAll();

	if (*csvFilePath_StageObjData != filename)
		*csvFilePath_StageObjData = filename;

	CsvReader* csv = new CsvReader(*csvFilePath_StageObjData);
	if (csv == nullptr)
		return;

	for (int i = 1; i < csv->Column(); i++) {

		std::string objName = csv->GetString(i, 0);	// オブジェクトの名前

		if (objName.substr(0, 2) == "//")
			continue;

		StageObjInfo info;		// ステージオブジェクトの情報
		Transform tr;			// 座標・回転・拡縮の情報
		bool collider = false;	// 当たり判定を付けるか

		info.type = objName;
		info.hModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + objName + ".mv1");
		info.hHitModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + objName + "_col.mv1");

		tr.position = Vector3(csv->GetFloat(i, 1), csv->GetFloat(i, 2), csv->GetFloat(i, 3));
		tr.rotation = Vector3(DegToRad(csv->GetFloat(i, 4)), DegToRad(csv->GetFloat(i, 5)), DegToRad(csv->GetFloat(i, 6)));
		tr.scale	= Vector3(csv->GetFloat(i, 7), csv->GetFloat(i, 8), csv->GetFloat(i, 9));

		collider = csv->GetBool(i, 10);

		Create(info, tr, collider);
	}

	delete csv;
}

void StageObjectManager::LoadFromJson(const std::string& filename)
{
	if (stageObjects == nullptr)
		return;

	using namespace Math;

	EraseAll();

	if (*csvFilePath_StageObjData != filename)
		*csvFilePath_StageObjData = filename;

	// 読み込み
	{
		auto settings_json = Settings_json::Inst();
		settings_json->LoadSettingJson(filename, filename, true);
		std::vector<nlohmann::json> objects = Settings_json::Inst()->Get<std::vector<nlohmann::json>>("Objects", filename);

		for (const auto& obj : objects) {

			StageObjInfo info;		// ステージオブジェクトの情報
			Transform tr;			// 座標・回転・拡縮の情報
			bool isCollision = false;

			std::string type = obj.at("Type").get<std::string>();

			info.type = type;
			info.hModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + type + ".mv1");
			info.hHitModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + type + "_col.mv1");

			auto pos = obj.at("Position");
			float px = pos.at("x").get<float>();
			float py = pos.at("y").get<float>();
			float pz = pos.at("z").get<float>();
			tr.position = Vector3(px, py, pz);

			auto rot = obj.at("Rotation");
			float rx = rot.at("x").get<float>();
			float ry = rot.at("y").get<float>();
			float rz = rot.at("z").get<float>();
			tr.rotation = Vector3(rx, ry, rz);

			auto sca = obj.at("Scale");
			float sx = sca.at("x").get<float>();
			float sy = sca.at("y").get<float>();
			float sz = sca.at("z").get<float>();
			tr.scale = Vector3(sx, sy, sz);

			isCollision = obj.at("IsCollision").get<bool>();

			Create(info, tr, isCollision);
		}
	}
}

void StageObjectManager::SaveToCsv() {

	if (csvFilePath_StageObjData == nullptr || stageObjects == nullptr)
		return;

	OutPutToCsv(*csvFilePath_StageObjData);
}

void StageObjectManager::SaveToJson()
{
	if (csvFilePath_StageObjData == nullptr || stageObjects == nullptr)
		return;

    // 書き込み
    {
        auto settings_json = Settings_json::Inst();
		settings_json->Save(*csvFilePath_StageObjData);
    }
}

void StageObjectManager::OutPutToCsv(const std::string& filename) {

	if (stageObjects == nullptr)
		return;

	std::ofstream file(filename);

	file << (std::string)CSV_TITLE_TEMPLATE + '\n';	// Csvに定型文を書き込む
	std::vector<std::string> data;	// ファイルに書き出す情報を行ごとにまとめる

	for (const auto& itr : *stageObjects) {

		//==========================================================================================
		// ▼データをstring型に変換

		data.push_back(itr->Info().type);
		data.push_back(std::to_string(itr->transform->position.x));
		data.push_back(std::to_string(itr->transform->position.y));
		data.push_back(std::to_string(itr->transform->position.z));
		data.push_back(std::to_string(Math::RadToDeg(itr->transform->rotation.x)));
		data.push_back(std::to_string(Math::RadToDeg(itr->transform->rotation.y)));
		data.push_back(std::to_string(Math::RadToDeg(itr->transform->rotation.z)));
		data.push_back(std::to_string(itr->transform->scale.x));
		data.push_back(std::to_string(itr->transform->scale.y));
		data.push_back(std::to_string(itr->transform->scale.z));
		data.push_back(std::to_string(itr->IsCollider()));

		//==========================================================================================

		bool newLine = false;	// 行の最後の文字に改行が付いているか

		for (auto dat = data.begin(); dat != data.end(); dat++) {
			// 改行が無く、カンマも無い場合、カンマを追記する
			if ((*dat).find('\n') == std::string::npos) {
				if ((*dat).find(',') == std::string::npos)
					(*dat) += ',';
			}
			// 改行がある場合、newLineをTrueにする
			else
				newLine = true;

			file << (*dat).c_str();	// ファイルに書き出し
		}

		// 改行が無ければ改行する
		if (newLine == false)
			file << '\n';

		data.clear();	// データを解放する
	}

	file.close();	// ファイルを閉じる

	OutputDebugString((filename + "にデータを保存しました。\n").c_str());
}

void StageObjectManager::Create(StageObjInfo info, const Transform& _transform, bool _collider) {

	if (stageObjects == nullptr)
		return;

	StageObjInfo objInfo;

	objInfo = info;

	if (objInfo.hModel < 0)
		objInfo.hModel = ResourceLoader::MV1LoadModel(objInfo.fileName + objInfo.type + ".mv1");
	
	if (objInfo.hHitModel < 0)
		objInfo.hHitModel = ResourceLoader::MV1LoadModel(objInfo.fileName + objInfo.type + "_col.mv1");

	stageObjects->push_back(new StageObject(objInfo, _transform, ++productionID, _collider));
}

void StageObjectManager::CreateTheString(const std::string& objName) {

	StageObjInfo info;
	bool isCollder = false;

	info.type = objName;
	info.hModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + objName + ".mv1");
	info.hHitModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + objName + "_col.mv1");

	if (info.hHitModel > 0)
		isCollder = true;

	Create(info, Transform(), isCollder);
}

bool StageObjectManager::Erase(int id) {

	bool isErase = false;	// 削除したか

	for (auto itr = stageObjects->begin(); itr != stageObjects->end(); itr++) {
		if ((*itr)->ID() == id) {
			stageObjects->erase(itr);
			isErase = true;
			break;
		}
	}

	// 削除した際に生じた隙間を埋める
	if (isErase)
		ShiftID(id);

	return isErase;
}

bool StageObjectManager::EraseTheString(const std::string& id) {

	std::string sID;

	for (const auto& itr : id) {
		if (itr == ' ')
			break;
		sID.push_back(itr);
	}

	int iID = stoi(sID);

	return Erase(iID);
}

void StageObjectManager::EraseAll() {

	if (stageObjects == nullptr)
		return;

	for (auto stObj = stageObjects->begin(); stObj != stageObjects->end();) {
		delete* stObj;
		stObj = stageObjects->erase(stObj);
		if (stObj == stageObjects->end()) break;
	}

	//std::erase_if(*stageObjects, [](auto itr) { return itr != nullptr; });

	productionID = 0;
}

void StageObjectManager::ShiftID(int id) {

	productionID--;
	for (const auto& itr : *stageObjects) {
		if (itr->ID() > id)
			itr->AddingID(-1);
	}
}

void StageObjectManager::DrawEditMode() {

	ImGuiRoot* stageObjTree = ImGuiManager::FindRoot("StageObject");
	if (stageObjTree == nullptr)
		return;

	ImGuiRoot* stageObjEditTree = stageObjTree->Children("Edit Mode");
	ImGuiRoot* stageObjDeleteTree = stageObjTree->Children("Delete Mode");

	if (stageObjEditTree == nullptr || stageObjDeleteTree == nullptr)
		return;

	int id = -1;
	bool isOpen = (stageObjEditTree->IsOpen() || stageObjDeleteTree->IsOpen());

	for (StageObject* itr : *stageObjects) {
		if (stageObjEditTree->IsOpen())
			id = stageObjEditTree->Node<ImGuiNode_DropDown>("StageObject Edit")->CurrentIndex() + 1;

		if (stageObjDeleteTree->IsOpen())
			id = stageObjDeleteTree->Node<ImGuiNode_DropDown>("StageObject Delete")->CurrentIndex() + 1;

		if (!isOpen || (isOpen && itr->ID() == id))
			MV1SetOpacityRate(itr->Model(), 1.0f);

		if (id != -1 && itr->ID() != id)
			MV1SetOpacityRate(itr->Model(), 0.3f);

		itr->Draw();
	}
}

void StageObjectManager::SaveAndLoad() {

	SaveToCsv();
	
	if (csvFilePath_StageObjData != nullptr)
		LoadToCsv(*csvFilePath_StageObjData);
}

void StageObjectManager::ReloadStageData() {

	// 読み込み用Csvファイルのパスが存在しない場合
	if (csvFilePath_StageObjData == nullptr)
		return;

	// 読み込んだ全てのデータを解放する
	EraseAll();

	// Csvからエフェクト情報を読み込む
	LoadToCsv(*csvFilePath_StageObjData);
}

void StageObjectManager::HitModelDrawSwitch() {

	for (const auto& itr : *stageObjects)
		itr->SetIsHitModelDraw(!itr->IsHitModelDraw());
}

void StageObjectManager::SetModelFilePath(const std::string& filename) {

	if (csvFilePath_StageObjModel == nullptr)
		return;

	if (*csvFilePath_StageObjModel != filename)
		*csvFilePath_StageObjModel = filename;
}

std::vector<StageObject*> StageObjectManager::StageObjects() {

	return *stageObjects;
}

StageObject* StageObjectManager::GetStageObject(int id) {

	for (const auto& itr : *stageObjects) {
		if (itr->ID() == id)
			return itr;
	}

	return nullptr;
}

std::vector<std::string> StageObjectManager::StageObjectsTheString() {

	std::vector<std::string> names;

	for (const auto& itr : *stageObjects) {
		std::string ID = std::to_string(itr->ID());

		names.push_back(ID + " " + itr->Info().type);
	}

	return names;
}

#ifdef IMGUI

void StageObjectManager::InitImGui() {

	// 全ステージオブジェクトの種類名
	std::vector<std::string> objTypeList;

	// ステージオブジェクトの最大種類数
	int objTypeMax = static_cast<int>(ObjType::Max);

	for (int i = 0; i < objTypeMax; i++) {

		// 要素数をObjTypeへ変換
		ObjType eObjType = static_cast<ObjType>(i);

		// ステージオブジェクトの種類名を取得
		std::string objName = static_cast<std::string>(magic_enum::enum_name(eObjType));

		objTypeList.push_back(objName);
	}

	// ステージオブジェクトに関するImGui機能
	ImGuiRoot* stageObjectTree = ImGuiManager::AddRoot(new ImGuiRoot("StageObject"));

	// ステージオブジェクトの生成
	{
		ImGuiRoot* createTree = stageObjectTree->AddChild(new ImGuiRoot("Create Mode"));

		createTree->Add(new ImGuiNode_DropDown("StageObject Create", objTypeList, [](std::string s) { StageObjectManager::CreateTheString(s); }, "Create"));
	}

	// ステージオブジェクトの実体が存在しないもしくは、ステージオブジェクトに何も入っていない場合
	if (stageObjects == nullptr)
		stageObjects = new std::vector<StageObject*>();

	if (stageObjects->empty()) {
		StageObjInfo info;
		info.fileName = *csvFilePath_StageObjModel;
		info.type = objTypeList[0];
		Create(info, Transform(Vector3(0.0f, -1000.0f, 0.0f)));
	}

	// ステージオブジェクトの編集
	{
		ImGuiRoot* editTree = stageObjectTree->AddChild(new ImGuiRoot("Edit Mode"));

		auto itr = stageObjects->begin();

		editTree->NodeBeginChild(250.0f, 85.0f);
		editTree->Add(new ImGuiNode_SliderFloat("Position X StageObject Edit", &(*itr)->transform->position.x, -5000.0f, 5000.0f));
		editTree->Add(new ImGuiNode_SliderFloat("Position Y StageObject Edit", &(*itr)->transform->position.y, -5000.0f, 5000.0f));
		editTree->Add(new ImGuiNode_SliderFloat("Position Z StageObject Edit", &(*itr)->transform->position.z, -5000.0f, 5000.0f));
		editTree->NodeEndChild();

		editTree->NodeBeginChild(250.0f, 85.0f);
		editTree->Add(new ImGuiNode_SliderFloat("Rotation X StageObject Edit", &(*itr)->transform->rotation.x, Math::DegToRad(-360), Math::DegToRad(360)));
		editTree->Add(new ImGuiNode_SliderFloat("Rotation Y StageObject Edit", &(*itr)->transform->rotation.y, Math::DegToRad(-360), Math::DegToRad(360)));
		editTree->Add(new ImGuiNode_SliderFloat("Rotation Z StageObject Edit", &(*itr)->transform->rotation.z, Math::DegToRad(-360), Math::DegToRad(360)));
		editTree->NodeEndChild();

		editTree->NodeBeginChild(250.0f, 85.0f);
		editTree->Add(new ImGuiNode_SliderFloat("Scale X StageObject Edit", &(*itr)->transform->scale.x, 0.0f, 100.0f));
		editTree->Add(new ImGuiNode_SliderFloat("Scale Y StageObject Edit", &(*itr)->transform->scale.y, 0.0f, 100.0f));
		editTree->Add(new ImGuiNode_SliderFloat("Scale Z StageObject Edit", &(*itr)->transform->scale.z, 0.0f, 100.0f));
		editTree->NodeEndChild();

		editTree->Add(new ImGuiNode_DropDown("StageObject Edit", StageObjectsTheString()));

		editTree->Add(new ImGuiNode_Button("HitModel Draw", &HitModelDrawSwitch));

		editTree->Add(new ImGuiNode_CheckBox("IsCollider", &(*stageObjects)[0]->IsColliderPtr()));
	}

	// ステージオブジェクトの削除
	{
		ImGuiRoot* deleteTree = stageObjectTree->AddChild(new ImGuiRoot("Delete Mode"));

		deleteTree->Add(new ImGuiNode_DropDown("StageObject Delete", StageObjectsTheString(), [](std::string s) { StageObjectManager::EraseTheString(s); }, "Delete"));
	}

	// Csvデータの再読み込み
	stageObjectTree->Add(new ImGuiNode_Button("Reload Stage", std::bind(ReloadStageData)));

	// Csvデータへの書き込み
	stageObjectTree->Add(new ImGuiNode_Button("Save Stage", SaveToCsv));
}

void StageObjectManager::UpdateImGui() {

	// ステージオブジェクトの実体が存在しないもしくは、ステージオブジェクトに何も入っていない場合
	if ((stageObjects != nullptr && stageObjects->empty() == false) == false)
		return;

	// ステージオブジェクトに関するImGui機能
	ImGuiRoot* stageObjTree = ImGuiManager::FindRoot("StageObject");
	if (stageObjTree == nullptr)
		return;

	// ステージオブジェクトの編集
	{
		ImGuiRoot* stageObjEditTree = stageObjTree->Children("Edit Mode");
		if (stageObjEditTree == nullptr)
			return;

		stageObjEditTree->Node<ImGuiNode_DropDown>("StageObject Edit")->SetList(StageObjectsTheString());

		int curIndex = stageObjEditTree->Node<ImGuiNode_DropDown>("StageObject Edit")->CurrentIndex();
		int index = min(max(curIndex, 0), static_cast<int>(stageObjects->size()) - 1);

		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Position X StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->position.x);
		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Position Y StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->position.y);
		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Position Z StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->position.z);

		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Rotation X StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->rotation.x);
		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Rotation Y StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->rotation.y);
		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Rotation Z StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->rotation.z);

		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Scale X StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->scale.x);
		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Scale Y StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->scale.y);
		stageObjEditTree->Node<ImGuiNode_SliderFloat>("Scale Z StageObject Edit")->SetValue(&(*stageObjects)[index]->transform->scale.z);

		stageObjEditTree->Node<ImGuiNode_CheckBox>("IsCollider")->SetValue(&(*stageObjects)[index]->IsColliderPtr());
	}

	// ステージオブジェクトの削除
	{
		ImGuiRoot* stageObjDeleteTree = stageObjTree->Children("Delete Mode");
		if (stageObjDeleteTree == nullptr)
			return;

		stageObjDeleteTree->Node<ImGuiNode_DropDown>("StageObject Delete")->SetList(StageObjectsTheString());
	}
}

#endif