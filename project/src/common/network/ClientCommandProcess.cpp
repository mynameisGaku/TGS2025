#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "NetworkManager.h"
#include <src/common/network/user/User.h>
#include <src/util/logger/Logger.h>
#include <vendor/nlohmann/json.hpp>
#include <string>
#include <src/util/transform/Transform.h>
#include <src/scene/play/chara/CharaManager.h>
#include <src/util/file/json/VectorJson.h>
#include <src/common/load_screen/LoadScreen.h>
#include <src/scene/play/ball/BallSpawner.h>

using JSON = nlohmann::json;

void NetworkManager::ClientCommandProcess(JSON& json, SOCKET sock)
{
	std::string command = json["Command"];

	// 無視リスト
	const std::string const ignores[] = {
		"AddUser",
	};

	for (auto& ignore : ignores)
	{
		if (command == ignore)
			return;
	}

	if (command == "SetUserData")
	{
		g_Users.clear();
		for (auto& data : json["Users"])
		{
			User user{};
			user.Name = data["Name"].get<std::string>();
			user.UUID = data["UUID"].get<std::string>();
			user.Socket = data["Socket"].get<UINT>();
			user.IsHost = data["IsHost"].get<bool>();
			g_Users.push_back(user);
			Logger::FormatDebugLog("[受信] ユーザー追加: %s (UUID: %s)", user.Name.c_str(), user.UUID.c_str());
		}
		if (json.contains("Added_ID"))
		{
			// 追加されたユーザー = 自分なので、自身のMyUUIDにセット
			g_MyUUID = json.at("Added_ID").get<std::string>();
		}
	}
	else if (command == "SetTransform")
	{
		Transform trs;
		from_json(json["Position"], trs.position);
		from_json(json["Rotation"], trs.rotation);
		from_json(json["Scale"], trs.scale);
		auto cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(json["UUID"].get<std::string>());
		if (not c)
			return;
		if (not c->transform)
			return;
		*c->transform = trs;
	}
	else if (command == "TransitToPlay")
	{
		SceneManager::ChangeScene("PlayScene");
		SceneManager::CommonScene()->FindGameObject<LoadScreen>()->FadeOut(0.1f);
	}
	else if (command == "ChangeState")
	{
		std::string uuid = json.at("UUID").get<std::string>();
		std::string state = json.at("State").get<std::string>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
			return;
		if (not c->m_FSM)
			return;
		c->m_FSM->ChangeStateByName(state);
	}
	else if (command == "ChangeSubState")
	{
		std::string uuid = json.at("UUID").get<std::string>();
		std::string state = json.at("State").get<std::string>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
			return;
		if (not c->m_SubFSM)
			return;
		c->m_SubFSM->ChangeStateByName(state);
	}
	else if (command == "ChangeRespawnState")
	{
		// ホスト側にいる対象者のステートを変更する
		std::string uuid = json.at("UUID").get<std::string>();
		std::string state = json.at("State").get<std::string>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[受信] 指定のUUIDが見つかりませんでした。. UUID: %s.", uuid.c_str());
			return;
		}
		if (not c->m_RespawnFSM)
			return;
		c->m_RespawnFSM->ChangeStateByName(state);
	}
	else if (command == "SetCharaMoveFlag")
	{
		std::string uuid = json.at("UUID").get<std::string>();
		bool moveFlag = json.at("Flag").get<bool>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[受信] 指定のUUIDが見つかりませんでした。. UUID: %s.", uuid.c_str());
			return;
		}

		c->SetIsMove(moveFlag);
	}
	else if (command == "SetCharaAllFlag")
	{
		std::string uuid = json.at("UUID").get<std::string>();
		JSON flags = json["Flags"];

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[受信] 指定のUUIDが見つかりませんでした。. UUID: %s.", uuid.c_str());
			return;
		}

		c->m_IsCharging			= flags["IsCharging"];
		c->m_IsLanding			= flags["IsLanding"];
		c->m_CanMove			= flags["CanMove"];
		c->m_CanRot				= flags["CanRot"];
		c->m_IsMove				= flags["IsMove"];
		c->m_IsJumping			= flags["IsJumping"];
		c->m_CanCatch			= flags["CanCatch"];
		c->m_CanHold			= flags["CanHold"];
		c->m_CanThrow			= flags["CanThrow"];
		c->m_IsCatching			= flags["IsCatching"];
		c->m_IsTargeting		= flags["IsTargeting"];
		c->m_IsTargeted			= flags["IsTargeted"];
		c->m_CanTackle			= flags["CanTackle"];
		c->m_IsTackling			= flags["IsTackling"];
		c->m_IsInvincible		= flags["IsInvincible"];
		c->m_IsDamage			= flags["IsDamage"];
		c->m_IsSliding			= flags["IsSliding"];
		c->m_IsInhibitionSpeed	= flags["IsInhibitionSpeed"];
		c->m_CanClimb			= flags["CanClimb"];
		c->m_IsClimb			= flags["IsClimb"];
		c->m_IsWall				= flags["IsWall"];
	}
	else if (command == "CreateBallSpawner")
	{
		std::string id = json["ID"];
		JSON bsdesc = json["Desc"];
		BALL_SPAWNER_DESC desc{};

		desc.INTERVAL_SEC = bsdesc["INTERVAL_SEC"].get<float>();
		desc.INTERVAL_SEC_RANDOM_RANGE = bsdesc["INTERVAL_SEC_RANDOM_RANGE"].get<float>();
		desc.SPAWN_AMOUNT_INITIAL = bsdesc["SPAWN_AMOUNT_INITIAL"].get<int>();
		desc.SPAWN_AMOUNT_ONCE_MAX = bsdesc["SPAWN_AMOUNT_ONCE_MAX"].get<int>();
		desc.SPAWN_AMOUNT_ONCE_MIN = bsdesc["SPAWN_AMOUNT_ONCE_MIN"].get<int>();
		desc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE = bsdesc["SPAWN_AMOUNT_ONCE_RANDOM_RANGE"].get<int>();
		from_json(bsdesc["SPAWN_INITIAL_VELOCITY"], desc.SPAWN_INITIAL_VELOCITY);
		from_json(bsdesc["SPAWN_RANGE"], desc.SPAWN_RANGE);
		int hModel = json["Model"];
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;
		from_json(json["Position"], Position);
		from_json(json["Rotation"], Rotation);
		from_json(json["Scale"], Scale);
		Transform trs(Position, Rotation, Scale);

		AddBallSpawner(hModel, trs, desc, id);
	}
	else
	{
		Logger::FormatDebugLog("[受信] JSON Command: %s", command.c_str());
	}
}