#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "NetworkManager.h"
#include <src/util/logger/Logger.h>
#include <vendor/nlohmann/json.hpp>

using json = nlohmann::json;

void NetworkManager::HostCommandProcess(json& j, SOCKET sock)
{
    std::string command = j["Command"];

    // 無視リスト
    const std::string const ignores[] = {
        "",
    };

    for (auto& ignore : ignores)
    {
        if (command == ignore)
            return;
    }

    bool needReply = j["NeedReply"].get<bool>();

    if (command == "AddUser")
    {
        std::string name = j["Name"].get<std::string>();
        User user{};
        user.Name = name;
        user.UUID = ++g_UUIDGenerator;
        g_Users.push_back(user);

        if (needReply)
        {
            json reply;

            reply["Command"] = "SetUserData";
            for (auto& u : g_Users)
            {
                // replyに現在存在するユーザーデータを配列として登録
                json userJson;
                userJson["Name"] = u.Name;
                userJson["UUID"] = u.UUID;
                reply["Users"].push_back(userJson);
            }

            json add;
            add["Name"] = user.Name;
            add["UUID"] = user.UUID;
            reply["Users"].push_back(add);
            std::string str = reply.dump();

            PacketHeader header{};
            header.type = PACKET_JSON;
            header.size = static_cast<int>(str.size()) + 1;

            send(sock, (const char*)&header, sizeof(header), 0);
            send(sock, str.c_str(), header.size, 0);
        }
    }
    else
    {
        Logger::FormatDebugLog("未知のCommand: %s", command.c_str());
    }
}