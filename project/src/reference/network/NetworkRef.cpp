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

void NetworkRef::Load(bool forceLoad)
{
	if( m_WasLoad && !forceLoad)
	{
		return;
	}

	Settings_json* jsonLoader = Settings_json::Inst();

	jsonLoader->LoadSettingJson(PATH, FILEKEY, true);
	auto json = jsonLoader->GetJson(FILEKEY);

	Host_IPAddress.d1 = json["HostIP"]["d1"].get<UCHAR>();
	Host_IPAddress.d2 = json["HostIP"]["d2"].get<UCHAR>();
	Host_IPAddress.d3 = json["HostIP"]["d3"].get<UCHAR>();
	Host_IPAddress.d4 = json["HostIP"]["d4"].get<UCHAR>();

	Port = json["Port"].get<USHORT>();
	ConnectionMax = json["ConnectionMax"].get<USHORT>();

	IsHost = json["IsHost"].get<bool>();

	m_WasLoad = true;
}