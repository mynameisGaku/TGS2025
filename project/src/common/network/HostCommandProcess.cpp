#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

#include "NetworkManager.h"
#include <src/util/logger/Logger.h>
#include <vendor/nlohmann/json.hpp>
#include <src/util/transform/Transform.h>
#include <src/scene/play/chara/CharaManager.h>
#include <src/util/file/json/VectorJson.h>
#include <src/common/load_screen/LoadScreen.h>
#include <src/reference/network/NetworkRef.h>

using JSON = nlohmann::json;

void NetworkManager::HostCommandProcess(JSON& json, SOCKET sock)
{
	std::string command = json["Command"];

	// 無視リスト
	const std::string const ignores[] = {
		"",
	};

	for (auto& ignore : ignores)
	{
		if (command == ignore)
			return;
	}

	bool needReply = json.at("NeedReply").get<bool>();

	if (command == "AddUser")
	{
		std::string name = json.at("Name").get<std::string>();
		User user{};
		user.Name = name;
		user.UUID = json.at("UUID").get<std::string>();
		user.Socket = sock;
		user.IsHost = false;
		g_Users.push_back(user);

		if (needReply)
		{
			JSON reply;

			reply["Command"] = "SetUserData";
			// 返信する内容として、既存のキャラデータをリストアップ
			for (auto& u : g_Users)
			{
				// replyに現在存在するユーザーデータを配列として登録
				JSON userJson;
				userJson["Name"] = u.Name;
				userJson["UUID"] = u.UUID;
				userJson["Socket"] = u.Socket;
				userJson["IsHost"] = u.IsHost;

				reply["Users"].push_back(userJson);
			}

			// リストの最後尾に新たに追加する
			reply["Added_ID"] = user.UUID;

			// 追加したユーザーデータを配列に追加
			std::string str = reply.dump();


			// 追加後の配列をまとめて送り、クライアントにセットする
			PacketHeader header{};
			header.type = PACKET_JSON;
			header.size = static_cast<int>(str.size()) + 1;

			send(sock, (const char*)&header, sizeof(header), 0);
			send(sock, str.c_str(), header.size, 0);
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
		// ゲーム開始のコマンドを受け取ったら、全クライアントに通知
		JSON broadcast{};
		broadcast["Command"] = "TransitToPlay";
		std::string str = broadcast.dump();
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

        SceneManager::ChangeScene("PlayScene");
        SceneManager::CommonScene()->FindGameObject<LoadScreen>()->FadeOut(0.1f);
	}
	else if (command == "SetUserData")
	{
		Logger::FormatDebugLog("[受信] ユーザーデータ更新要求");
	}
	else if (command == "ChangeState")
	{
		std::string str = json.dump();

		// 送信主以外のクライアントに対しても、同じく変更を要求する
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// ホスト側にいる対象者のステートを変更する
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

		Logger::FormatDebugLog("[受信] ステート変更要求. UUID: %s. State: %s", uuid.c_str(), state.c_str());
	}
	else if (command == "ChangeSubState")
	{
		std::string str = json.dump();

		// 送信主以外のクライアントに対しても、同じく変更を要求する
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// ホスト側にいる対象者のステートを変更する
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

		Logger::FormatDebugLog("[受信] ステート変更要求. UUID: %s. State: %s", uuid.c_str(), state.c_str());
	}
	else
	{
		Logger::FormatDebugLog("未知のCommand: %s", command.c_str());
	}
}