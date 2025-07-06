#include <src/reference/network/NetworkRef.h>
#include <src/util/file/json/settings_json.h>

NetworkRef* NetworkRef::instance = nullptr;

NetworkRef& NetworkRef::Inst()
{
    if (not instance)
    {
        instance = new NetworkRef();
    }
    return *instance;
}

void NetworkRef::Destroy()
{
    if (instance)
    {
        delete instance;
    }
    instance = nullptr;
}

void NetworkRef::Load(bool forceLoad = false)
{
    if( m_WasLoad && !forceLoad)
    {
        return;
    }

    Settings_json* jsonLoader = Settings_json::Inst();

    jsonLoader->LoadSettingJson(PATH, FILEKEY, true);
    auto json = jsonLoader->GetJson(FILEKEY);

    IpAddress.d1 = json["IP"]["d1"].get<UCHAR>();
    IpAddress.d2 = json["IP"]["d2"].get<UCHAR>();
    IpAddress.d3 = json["IP"]["d3"].get<UCHAR>();
    IpAddress.d4 = json["IP"]["d4"].get<UCHAR>();

    Port = json["port"].get<USHORT>();
    ConnectionMax = json["connection_max"].get<USHORT>();

    m_WasLoad = true;
}