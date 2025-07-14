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
			Logger::FormatDebugLog("[受信] ユーザー追加: %s (UUID: %d)", user.Name.c_str(), user.UUID);
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
	else
	{
		Logger::FormatDebugLog("[受信] JSON Command: %s", command.c_str());
	}
}