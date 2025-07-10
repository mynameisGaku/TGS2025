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

using JSON = nlohmann::json;

void NetworkManager::HostCommandProcess(JSON& json, SOCKET sock)
{
	std::string command = json["Command"];

	// �������X�g
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
		user.UUID = ++g_UUIDGenerator;
		user.Socket = sock;
		user.IsHost = false;
		g_Users.push_back(user);

		if (needReply)
		{
			JSON reply;

			reply["Command"] = "SetUserData";
			// �ԐM������e�Ƃ��āA�����̃L�����f�[�^�����X�g�A�b�v
			for (auto& u : g_Users)
			{
				// reply�Ɍ��ݑ��݂��郆�[�U�[�f�[�^��z��Ƃ��ēo�^
				JSON userJson;
				userJson["Name"] = u.Name;
				userJson["UUID"] = u.UUID;
				userJson["Socket"] = u.Socket;
				userJson["IsHost"] = u.IsHost;

				reply["Users"].push_back(userJson);
			}

			// ���X�g�̍Ō���ɐV���ɒǉ�����
			JSON add;
			add["Name"] = user.Name;
			add["UUID"] = user.UUID;
			add["Socket"] = user.Socket;
			add["IsHost"] = user.IsHost;
			reply["Added_ID"] = user.UUID;

			// �ǉ��������[�U�[�f�[�^��z��ɒǉ�
			reply["Users"].push_back(add);
			std::string str = reply.dump();


			// �ǉ���̔z����܂Ƃ߂đ���A�N���C�A���g�ɃZ�b�g����
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
		trs.position.x = json["Position"]["x"].get<float>();
		trs.position.y = json["Position"]["y"].get<float>();
		trs.position.z = json["Position"]["z"].get<float>();
		trs.rotation.x = json["Rotation"]["x"].get<float>();
		trs.rotation.y = json["Rotation"]["y"].get<float>();
		trs.rotation.z = json["Rotation"]["z"].get<float>();
		trs.scale.x = json["Scale"]["x"].get<float>();
		trs.scale.y = json["Scale"]["y"].get<float>();
		trs.scale.z = json["Scale"]["z"].get<float>();
        CharaManager* cm = FindGameObject<CharaManager>();
        if (not cm)
            return;
        auto c = cm->GetFromUUID(json["UUID"].get<UINT>());
        if (not c)
            return;
        *c->transform = trs;
	}
	else if (command == "TransitToPlay")
	{
		// �Q�[���J�n�̃R�}���h���󂯎������A�S�N���C�A���g�ɒʒm
		JSON broadcast;
		broadcast["Command"] = "TransitToPlay";
		std::string str = broadcast.dump();
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		SceneManager::ChangeScene("PlayScene");
	}
	else if (command == "SetUserData")
	{
		Logger::FormatDebugLog("[��M] ���[�U�[�f�[�^�X�V�v��");
	}
	else
	{
		Logger::FormatDebugLog("���m��Command: %s", command.c_str());
	}
}