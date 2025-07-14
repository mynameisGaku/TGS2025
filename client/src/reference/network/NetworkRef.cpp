#include <src/reference/network/NetworkRef.h>
#include <src/util/file/json/settings_json.h>
#include <src/util/math/Random.h>
#include <fstream>


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

	// uuid�������Ă��邩�H
	if (json["UUID"].is_null())
	{
        // uuid�𐶐����ăN���C�A���g�ɓo�^
		std::string uuid = "";

        // �ő包����3�P�^�Ƃ��A�P�P�^�A�Q�P�^�̏ꍇ��00X��0XX�̂悤�ɁA0������  
        auto formatToThreeDigits = [](int number) -> std::string {
            std::ostringstream oss;
            oss << std::setw(3) << std::setfill('0') << number;
            return oss.str();
            };

		for (int i = 0; i < 3; i++)
		{
			uuid += formatToThreeDigits(Random.GetIntRange(0, 999));
		}
		json["UUID"] = uuid;

        std::ofstream file(PATH);
        file << json;
    }
    UUID = json["UUID"].get<std::string>();

	m_WasLoad = true;
}