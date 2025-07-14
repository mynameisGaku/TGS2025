#pragma once
#include <Windows.h>
#include <string>

typedef struct
{
	UINT			UUID;
	std::string		Name;
	SOCKET			Socket;
	bool			IsHost;
}User;