#pragma once
#include <Windows.h>
#include <string>

class NetworkRef
{
private:
    const std::string PATH = "data/Json/Network/NetworkConfig.json";
    const std::string FILEKEY = "NetworkConfig";
public:
    static NetworkRef& Inst();
    static void Destroy();
    void Load(bool forceLoad = false);

    typedef struct
    {
        UCHAR d1;
        UCHAR d2;
        UCHAR d3;
        UCHAR d4;
    }IP;
    IP Host_IPAddress;
    USHORT Port;
    USHORT ConnectionMax;
    BOOL IsHost;

private:
    static NetworkRef* instance;
    bool m_WasLoad = false;
};