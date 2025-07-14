#pragma once
#include <Windows.h>
#include <string>

typedef struct
{
	std::string		UUID;
	std::string		Name;
	SOCKET			Socket;
	bool			IsHost;
}User;