#pragma once
#include <Windows.h>
#include <string>

class NetworkRef
{
private:
    const std::string PATH = "data/Json/Network/NetwokConfig.json";
    const std::string FILEKEY = "NetwokConfig";
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
    IP IpAddress;
    USHORT Port;
    USHORT ConnectionMax;

private:
    static NetworkRef* instance;
    bool m_WasLoad = false;
};