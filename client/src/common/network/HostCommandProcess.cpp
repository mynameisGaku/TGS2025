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
#include <src/scene/play/ball/BallSpawner.h>
#include <src/util/enum/EnumUtil.h>

using JSON = nlohmann::json;

void NetworkManager::HostCommandProcess(JSON& json, SOCKET sock)
{
	std::string command = json["Command"];

	// �������X�g
	const std::string ignores[] = {
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
			reply["Added_ID"] = user.UUID;

			// �ǉ��������[�U�[�f�[�^��z��ɒǉ�
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
		from_json(json["Position"], trs.position);
		from_json(json["Rotation"], trs.rotation);
		from_json(json["Scale"], trs.scale);
		auto cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(json["UUID"].get<std::string>());
		if (not c)
		{
			Logger::FormatDebugLog("[��M] �w���UUID��������܂���ł����B. UUID: %s.", json["UUID"].get<std::string>().c_str());
			return;
		}
		if (not c->transform)
			return;
		*c->transform = trs;
	}
	else if (command == "TransitToPlay")
	{
		// �Q�[���J�n�̃R�}���h���󂯎������A�S�N���C�A���g�ɒʒm
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
		Logger::FormatDebugLog("[��M] ���[�U�[�f�[�^�X�V�v��");
	}
	else if (command == "ChangeState")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// �z�X�g���ɂ���Ώێ҂̃X�e�[�g��ύX����
		std::string uuid = json.at("UUID").get<std::string>();
		std::string state = json.at("State").get<std::string>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[��M] �w���UUID��������܂���ł����B. UUID: %s.", uuid.c_str());
			return;
		}
		if (not c->m_FSM)
			return;
		c->m_FSM->ChangeStateByName(state);

		Logger::FormatDebugLog("[��M] �X�e�[�g�ύX�v��. UUID: %s. State: %s", uuid.c_str(), state.c_str());
	}
	else if (command == "ChangeSubState")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// �z�X�g���ɂ���Ώێ҂̃X�e�[�g��ύX����
		std::string uuid = json.at("UUID").get<std::string>();
		std::string state = json.at("State").get<std::string>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[��M] �w���UUID��������܂���ł����B. UUID: %s.", uuid.c_str());
			return;
		}
		if (not c->m_SubFSM)
			return;
		c->m_SubFSM->ChangeStateByName(state);

		Logger::FormatDebugLog("[��M] �X�e�[�g�ύX�v��. UUID: %s. State: %s", uuid.c_str(), state.c_str());
	}
	else if (command == "ChangeRespawnState")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// �z�X�g���ɂ���Ώێ҂̃X�e�[�g��ύX����
		std::string uuid = json.at("UUID").get<std::string>();
		std::string state = json.at("State").get<std::string>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[��M] �w���UUID��������܂���ł����B. UUID: %s.", uuid.c_str());
			return;
		}
		if (not c->m_RespawnFSM)
			return;
		c->m_RespawnFSM->ChangeStateByName(state);

		Logger::FormatDebugLog("[��M] �X�e�[�g�ύX�v��. UUID: %s. State: %s", uuid.c_str(), state.c_str());
	}
	else if (command == "SetCharaMoveFlag")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// �z�X�g���ɂ���Ώێ҂̃X�e�[�g��ύX����
		std::string uuid = json.at("UUID").get<std::string>();
		bool moveFlag = json.at("Flag").get<bool>();

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[��M] �w���UUID��������܂���ł����B. UUID: %s.", uuid.c_str());
			return;
		}

		c->SetIsMove(moveFlag);
	}
	else if (command == "SetCharaAllFlag")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		// �z�X�g���ɂ���Ώێ҂̃X�e�[�g��ύX����
		std::string uuid = json.at("UUID").get<std::string>();
		JSON flags = json["Flags"];

		CharaManager* cm = FindGameObject<CharaManager>();
		if (not cm)
			return;
		auto c = cm->GetFromUUID(uuid);
		if (not c)
		{
			Logger::FormatDebugLog("[��M] �w���UUID��������܂���ł����B. UUID: %s.", uuid.c_str());
			return;
		}

		c->m_IsCharging          = flags["IsCharging"];
		c->m_IsLanding           = flags["IsLanding"];
		c->m_CanMove             = flags["CanMove"];
		c->m_CanRot              = flags["CanRot"];
		c->m_IsMove              = flags["IsMove"];
		c->m_IsJumping           = flags["IsJumping"];
		c->m_CanCatch            = flags["CanCatch"];
		c->m_CanHold             = flags["CanHold"];
		c->m_CanThrow            = flags["CanThrow"];
		c->m_IsCatching          = flags["IsCatching"];
		c->m_IsTargeting         = flags["IsTargeting"];
		c->m_IsTargeted          = flags["IsTargeted"];
		c->m_CanTackle           = flags["CanTackle"];
		c->m_IsTackling          = flags["IsTackling"];
		c->m_IsInvincible        = flags["IsInvincible"];
		c->m_IsDamage            = flags["IsDamage"];
		c->m_IsSliding           = flags["IsSliding"];
		c->m_IsInhibitionSpeed   = flags["IsInhibitionSpeed"];
		c->m_CanClimb            = flags["CanClimb"];
		c->m_IsClimb             = flags["IsClimb"];
		c->m_IsWall              = flags["IsWall"];
	}
	else if (command == "SetBallTransform")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		Transform trs{};
		from_json(json["Position"], trs.position);
		from_json(json["Rotation"], trs.rotation);
		from_json(json["Scale"], trs.scale);
		std::string id = json["ID"].get<std::string>();
		BallManager* ballManager = FindGameObject<BallManager>();
		if (not ballManager)
			return;
		Ball* ball = ballManager->GetBall(id);
		if (not ball)
			return;
		ball->SetTransform(trs);
	}
	else if (command == "SetBallState")
	{
		std::string str = json.dump();

		// ���M��ȊO�̃N���C�A���g�ɑ΂��Ă��A�������ύX��v������
		PacketHeader header{};
		header.type = PACKET_JSON;
		header.size = static_cast<int>(str.size()) + 1;
		Broadcast(header, str.c_str(), sock);

		Ball::State state{};
		state = EnumUtil::ToEnum(json["State"], Ball::State::S_LANDED);
		std::string id = json["ID"].get<std::string>();
		BallManager* ballManager = FindGameObject<BallManager>();
		if (not ballManager)
			return;
		Ball* ball = ballManager->GetBall(id);
		if (not ball)
			return;
		ball->SetState(state);
	}
	else
	{
		Logger::FormatDebugLog("���m��Command: %s", command.c_str());
	}
}