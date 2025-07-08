#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "NetworkManager.h"
#include <src/util/logger/Logger.h>
#include <vendor/nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void NetworkManager::ClientCommandProcess(json& j, SOCKET sock)
{
    std::string command = j["Command"];

    // ñ≥éãÉäÉXÉg
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
        for (auto& user : j["Users"])
        {
            User user{};
        }
    }
    else
    {
        Logger::FormatDebugLog("[éÛêM] JSON Command: %s", command.c_str());
    }
}