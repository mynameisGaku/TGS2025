#pragma once
#include <Windows.h>
#include <string>

class NetworkRef
{
private:
    const std::string PATH = "network.json";
    const std::string FILEKEY = "NetworkConfig";
public:
    static NetworkRef& Inst();
    static void Destroy();
    void Load(bool forceLoad = false);

    std::string HostIP;
    std::string UUID;
    USHORT Port;
    USHORT ConnectionMax;
    BOOL IsHost;
    BOOL IsNetworkEnable;

private:
    static NetworkRef* instance;
    bool m_WasLoad = false;
};