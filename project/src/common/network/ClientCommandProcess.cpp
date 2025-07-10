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
		for (auto& data : json["Users"])
		{
			User user{};
			user.Name = data["Name"].get<std::string>();
			user.UUID = data["UUID"].get<UINT>();
			user.Socket = data["Socket"].get<UINT>();
			user.IsHost = data["IsHost"].get<bool>();
			g_Users.push_back(user);
			Logger::FormatDebugLog("[受信] ユーザー追加: %s (UUID: %d)", user.Name.c_str(), user.UUID);
		}
		if (json.contains("Added_ID"))
		{
			// 追加されたユーザー = 自分なので、自身のMyUUIDにセット
			g_MyUUID = json.at("Added_ID").get<UINT>();
		}
	}
    else if (command == "SetTransform")
    {
        Transform trs;
        trs.position.x = json["Position"]["x"].get<float>();
        trs.position.y = json["Position"]["y"].get<float>();
        trs.position.z = json["Position"]["z"].get<float>();
        trs.rotation.x = json["Rotation"]["x"].get<float>();
        trs.rotation.y = json["Rotation"]["y"].get<float>();
        trs.rotation.z = json["Rotation"]["z"].get<float>();
        trs.scale.x = json["Scale"]["x"].get<float>();
        trs.scale.y = json["Scale"]["y"].get<float>();
        trs.scale.z = json["Scale"]["z"].get<float>();
        auto cm = FindGameObject<CharaManager>();
        if (not cm)
            return;
        auto c = cm->GetFromUUID(json["UUID"].get<UINT>());
        if (not c)
            return;
        *c->transform = trs;
    }
    else if (command == "TransitToPlay")
    {
        SceneManager::ChangeScene("PlayScene");
    }
	else
	{
		Logger::FormatDebugLog("[受信] JSON Command: %s", command.c_str());
	}
}