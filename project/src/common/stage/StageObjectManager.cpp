#include "src/common/stage/StageObjectManager.h"

// ◇汎用
#include <fstream>
#include <assert.h>

#include "src/util/ptr/PtrUtil.h"
#include "src/util/math/MathUtil.h"
#include "src/util/debug/imgui/imGuiManager.h"
#include "src/util/file/csv/CsvReader.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include <vendor/magic_enum/magic_enum.hpp>

// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/file/json/settings_json.h"

// スポナー
#include "src/scene/play/chara/CharaSpawnPointManager.h"
#include "src/scene/play/chara/CharaSpawnPoint.h"
#include "src/scene/play/ball/BallSpawner.h"

// 移動可能範囲
#include "src/scene/play/movable/MovableArea.h"
#include "src/common/component/collider/CollisionFunc.h"

using namespace StageDefine;

namespace {

	// Csvに書き出す際の定型文
	const char* CSV_TITLE_TEMPLATE = "//種類,座標(X軸),座標(Y軸),座標(Z軸),回転(X軸),回転(Y軸),回転(Z軸),拡縮(X軸),拡縮(Y軸),拡縮(Z軸),当たり判定の可否";

	std::string* csvFilePath_StageObjData;	// ステージオブジェクトの配置データを持つファイルパス
	std::string* csvFilePath_StageObjModel;	// ステージオブジェクトのモデルデータを持つファイルパス

	std::vector<StageObject*>* stageObjects;	// ステージオブジェクト
	std::vector<MovableArea*>* movableAreas;

	static unsigned int productionID;	// 製造番号
	bool canSaveCsv;			// CSVデータを保存できるか
	bool initialize = false;	// 初期化処理が行われたか
}

void StageObjectManager::Init() {

	if (stageObjects == nullptr)
		stageObjects = new std::vector<StageObject*>();

	if (movableAreas == nullptr)
		movableAreas = new std::vector<MovableArea*>();

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

	if (movableAreas != nullptr)
	{
		for (auto area : *movableAreas)
			area->Update();
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

	if (movableAreas != nullptr)
	{
		for (auto area : *movableAreas)
			area->Draw();
	}

#else

	if(stageObjects)
	{
		for (auto itr : *stageObjects)
			itr->Draw();
	}

#endif

	SetFogEnable(false);
}

void StageObjectManager::Release() {

	//SaveToCsv();
	//SaveToJson();
	EraseAll();
	EraseAll();

	PtrUtil::SafeDelete(stageObjects);
	PtrUtil::SafeDelete(movableAreas);
	PtrUtil::SafeDelete(csvFilePath_StageObjData);
	PtrUtil::SafeDelete(csvFilePath_StageObjModel);
}

bool StageObjectManager::CollCheckLine(const Vector3& begin, const Vector3& end, Vector3* hitPos) {

	if (stageObjects == nullptr)
		return false;

	bool hitFlag = false;
	//VECTOR prevPush = VGet(0, 0, 0);
	float minDist = VSquareSize(begin - end);
	VECTOR hitPosition = begin;

	for (auto obj : *stageObjects) {

		obj->SetOpacity(1.0f);
		MV1SetupCollInfo(obj->Info().hHitModel, -1, 8, 8, 8);

		MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(obj->Info().hHitModel, -1, end, begin);

		if (ret.HitFlag) {
			float distX = ret.HitPosition.x - end.x;
			float distY = ret.HitPosition.y - end.y;
			float distZ = ret.HitPosition.z - end.z;

			float dist = VSquareSize(Vector3(distX, distY, distZ));
			if (dist < minDist) {
				minDist = dist;
				hitPosition = ret.HitPosition;
				hitFlag = true;
			}
		}
	}

	// 当たっていたかを判断して、余計な代入を行わない
	if (hitFlag && hitPos != nullptr)
		*hitPos = hitPosition;

	return hitFlag;
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

bool StageObjectManager::CollCheckCapsule_Hitpos(const Vector3& p1, const Vector3& p2, float r, Vector3* hitPos)
{
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
			if (hitPos)
				*hitPos = dim.Dim->HitPosition;
		}
		MV1CollResultPolyDimTerminate(dim);
	}

	return hitFlag;
}

bool StageObjectManager::CollCheckRay(const Vector3& begin, const Vector3& end, Vector3* hitPos, Vector3* normal)
{
	Vector3 nearestPos = Vector3::Zero;
	Vector3 nearestNormal = Vector3::Zero;
	float nearestDist = 0.0f;
	bool isHit = false;

	for (const auto& obj : *stageObjects) {

		MV1_COLL_RESULT_POLY hit;
		hit = MV1CollCheck_Line(obj->Model(), 0, begin, end);
		if (hit.HitFlag > 0) {

			float dist = ((Vector3)hit.HitPosition - begin).GetLengthSquared();
			if ((not isHit) || nearestDist > dist)
			{
				nearestPos = hit.HitPosition;
				nearestNormal = hit.Normal;
				nearestDist = dist;
			}
			isHit = true;
		}
	}

	if (isHit)
	{
		if (hitPos != nullptr)	// 引数にポインタが入っていたら、代入をする
			*hitPos = nearestPos;
		if (normal != nullptr)	// 引数にポインタが入っていたら、代入をする
			*normal = nearestNormal;

		return true;
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

bool StageObjectManager::CollCheck_MovableArea(const Vector3& point)
{
	Vector3 nearestPos = Vector3::Zero;
	float nearestDist = 0.0f;
	bool isHit = false;

	for (const auto& obj : *movableAreas) {

		if (not obj->IsTag("MovableArea"))
			continue;

		if (ColFunction::CollCheck_PointToAABB(point, obj->GetAABB()) > 0.0f)
			isHit = true;
	}

	return isHit;
}

void StageObjectManager::LoadToCsv(const std::string& filename) {

	if (stageObjects == nullptr)
		return;

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
		tr.rotation = Vector3(MathUtil::ToRadians(csv->GetFloat(i, 4)), MathUtil::ToRadians(csv->GetFloat(i, 5)), MathUtil::ToRadians(csv->GetFloat(i, 6)));
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

	EraseAll();

	if (*csvFilePath_StageObjData != filename)
		*csvFilePath_StageObjData = filename;

	CharaSpawnPointManager* pCharaSpawnPointManager = FindGameObject<CharaSpawnPointManager>();
	if (pCharaSpawnPointManager == nullptr)
		pCharaSpawnPointManager = Instantiate<CharaSpawnPointManager>();

	auto settings_json = Settings_json::Inst();
	settings_json->LoadSettingJson(filename, filename, true);
	auto objects = settings_json->Get<std::vector<nlohmann::json>>("Objects", filename);

	for (const auto& obj : objects)
	{
		StageObjInfo info;
		Transform tr;
		bool isCollision = false;

		info.objname = obj.at("Name").get<std::string>();
		info.type = obj.at("Type").get<std::string>();

		auto pos = obj.at("Position");
		tr.position = Vector3(pos.at("x").get<float>(), pos.at("y").get<float>(), pos.at("z").get<float>());
		auto rot = obj.at("Rotation");
		tr.rotation = Vector3(rot.at("x").get<float>(), rot.at("y").get<float>(), rot.at("z").get<float>());
		auto sca = obj.at("Scale");
		tr.scale = Vector3(sca.at("x").get<float>(), sca.at("y").get<float>(), sca.at("z").get<float>());

		// CharaSpawnPoint
		if (obj.contains("CharaSpawnPointDesc") && !obj.at("CharaSpawnPointDesc").is_null())
		{
			info.hModel = ResourceLoader::MV1LoadModel("data/model/spawner/" + info.type + ".mv1");
			
			auto desc = obj.at("CharaSpawnPointDesc");
			CHARA_SPAWN_POINT_DESC cdesc;
			auto v = desc.at("SPAWN_INITIAL_VELOCITY");
			cdesc.SPAWN_INITIAL_VELOCITY = Vector3(v.at("x").get<float>(), v.at("y").get<float>(), v.at("z").get<float>());
			pCharaSpawnPointManager->AddSpawnPoint(info.hModel, tr, cdesc);
		}
		else if (obj.contains("BallSpawnerDesc") && !obj.at("BallSpawnerDesc").is_null())
		{
			// BallSpawner
			info.hModel = ResourceLoader::MV1LoadModel("data/model/spawner/" + info.type + ".mv1");

			auto desc = obj.at("BallSpawnerDesc");
			BALL_SPAWNER_DESC bdesc;
			bdesc.INTERVAL_SEC = desc.at("INTERVAL_SEC");
			bdesc.INTERVAL_SEC_RANDOM_RANGE = desc.at("INTERVAL_SEC_RANDOM_RANGE");
			bdesc.SPAWN_AMOUNT_ONCE_MAX = desc.at("SPAWN_AMOUNT_ONCE_MAX");
			bdesc.SPAWN_AMOUNT_ONCE_MIN = desc.at("SPAWN_AMOUNT_ONCE_MIN");
			bdesc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE = desc.at("SPAWN_AMOUNT_ONCE_RANDOM_RANGE");
			bdesc.SPAWN_AMOUNT_INITIAL = desc.at("SPAWN_AMOUNT_INITIAL");
			auto range = desc.at("SPAWN_RANGE");
			bdesc.SPAWN_RANGE = Vector3(range.at("x").get<float>(), range.at("y").get<float>(), range.at("z").get<float>());
			auto vel = desc.at("SPAWN_INITIAL_VELOCITY");
			bdesc.SPAWN_INITIAL_VELOCITY = Vector3(vel.at("x").get<float>(), vel.at("y").get<float>(), vel.at("z").get<float>());

			// 登録
			AddBallSpawner(info.hModel, tr, bdesc);
		}
		else if (obj.contains("MovableArea") && !obj.at("MovableArea").is_null())
		{
			// MovableArea
			info.hModel = ResourceLoader::MV1LoadModel("data/model/stage/Cube.mv1");
			ColDefine::AABB aabb = ColDefine::AABB(tr.position, (tr.scale * 100.0f) * 0.5f);
			MovableArea* movableArea = new MovableArea(aabb);
			movableArea->SetModel(info.hModel);
			movableArea->SetTransform(tr);
			movableArea->SetTag("MovableArea");

			movableAreas->push_back(movableArea);
		}
		else
		{
			info.hModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + info.type + ".mv1");
			info.hHitModel = ResourceLoader::MV1LoadModel(*csvFilePath_StageObjModel + info.type + "_col.mv1");

			isCollision = obj.at("IsCollision").get<bool>();

			Create(info, tr, isCollision);
		}
	}
}


void StageObjectManager::SaveToCsv() {
#if FALSE
	if (csvFilePath_StageObjData == nullptr || stageObjects == nullptr)
		return;

	OutPutToCsv(*csvFilePath_StageObjData);
#endif // FALSE
}

void StageObjectManager::SaveToJson()
{
	if (stageObjects == nullptr)
		return;

	nlohmann::json exportJson;
	std::vector<nlohmann::json> objectsJson;

	for (auto obj : *stageObjects)
	{
		nlohmann::json jsonObj;
		jsonObj["Name"] = obj->Info().objname;
		jsonObj["Type"] = obj->Info().type;
		jsonObj["Tag"] = obj->Info().type;

		jsonObj["Position"] = {
			{"x", obj->transform->position.x},
			{"y", obj->transform->position.y},
			{"z", obj->transform->position.z}
		};
		jsonObj["Rotation"] = {
			{"x", obj->transform->rotation.x},
			{"y", obj->transform->rotation.y},
			{"z", obj->transform->rotation.z}
		};
		jsonObj["Scale"] = {
			{"x", obj->transform->scale.x},
			{"y", obj->transform->scale.y},
			{"z", obj->transform->scale.z}
		};
		jsonObj["IsCollision"] = obj->IsCollider();

		// CharaSpawnPointDesc用: 仮の保存用。普段は必要ならCharaSpawnPointManagerから取る
		if (obj->Info().type == "CharaSpawnPoint")
		{
			CHARA_SPAWN_POINT_DESC cdesc{};
			cdesc.SPAWN_INITIAL_VELOCITY = Vector3(0, 0, 0);  // 仮データ

			jsonObj["CharaSpawnPointDesc"] = {
				{"SPAWN_INITIAL_VELOCITY", {
					{"x", cdesc.SPAWN_INITIAL_VELOCITY.x},
					{"y", cdesc.SPAWN_INITIAL_VELOCITY.y},
					{"z", cdesc.SPAWN_INITIAL_VELOCITY.z}
				}}
			};
		}

		// BallSpawnerDesc用: 仮の保存用。普段は必要ならBallSpawnerから取る
		if (obj->Info().type == "BallSpawner")
		{
			BALL_SPAWNER_DESC bdesc{};
			bdesc.SPAWN_RANGE = Vector3(0, 0, 0);
			bdesc.SPAWN_INITIAL_VELOCITY = Vector3(0, 0, 0);

			jsonObj["BallSpawnerDesc"] = {
				{"INTERVAL_SEC", bdesc.INTERVAL_SEC},
				{"INTERVAL_SEC_RANDOM_RANGE", bdesc.INTERVAL_SEC_RANDOM_RANGE},
				{"SPAWN_AMOUNT_ONCE_MAX", bdesc.SPAWN_AMOUNT_ONCE_MAX},
				{"SPAWN_AMOUNT_ONCE_MIN", bdesc.SPAWN_AMOUNT_ONCE_MIN},
				{"SPAWN_AMOUNT_ONCE_RANDOM_RANGE", bdesc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE},
				{"SPAWN_AMOUNT_INITIAL", bdesc.SPAWN_AMOUNT_INITIAL},
				{"SPAWN_RANGE", {
					{"x", bdesc.SPAWN_RANGE.x},
					{"y", bdesc.SPAWN_RANGE.y},
					{"z", bdesc.SPAWN_RANGE.z}
				}},
				{"SPAWN_INITIAL_VELOCITY", {
					{"x", bdesc.SPAWN_INITIAL_VELOCITY.x},
					{"y", bdesc.SPAWN_INITIAL_VELOCITY.y},
					{"z", bdesc.SPAWN_INITIAL_VELOCITY.z}
				}}
			};
		}

		objectsJson.push_back(jsonObj);
	}

	exportJson["Objects"] = objectsJson;

	std::ofstream file(*csvFilePath_StageObjData);
	file << exportJson.dump(4);
	file.close();
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
		data.push_back(std::to_string(MathUtil::ToDegrees(itr->transform->rotation.x)));
		data.push_back(std::to_string(MathUtil::ToDegrees(itr->transform->rotation.y)));
		data.push_back(std::to_string(MathUtil::ToDegrees(itr->transform->rotation.z)));
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
#ifdef IMGUI
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
#endif
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
		editTree->Add(new ImGuiNode_SliderFloat("Rotation X StageObject Edit", &(*itr)->transform->rotation.x, MathUtil::ToRadians(-360), MathUtil::ToRadians(360)));
		editTree->Add(new ImGuiNode_SliderFloat("Rotation Y StageObject Edit", &(*itr)->transform->rotation.y, MathUtil::ToRadians(-360), MathUtil::ToRadians(360)));
		editTree->Add(new ImGuiNode_SliderFloat("Rotation Z StageObject Edit", &(*itr)->transform->rotation.z, MathUtil::ToRadians(-360), MathUtil::ToRadians(360)));
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