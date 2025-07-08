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

	HostIP = json["HostIP"].get<std::string>();
	Port = json["Port"].get<USHORT>();
	ConnectionMax = json["ConnectionMax"].get<USHORT>();

	IsHost = json["IsHost"].get<bool>();
    IsNetworkEnable = json["IsNetworkEnable"].get<bool>();

	m_WasLoad = true;
}