#include "RawJson.h"
#include <cassert>
#include <typeinfo>
#include <src/util/math/Vector3.h>
#include <src/util/math/Vector2.h>

json RawJson::Create(const std::vector<std::any>& values)
{
	json result = json::array();

	for (const auto& item : values)
	{
		if (item.type() == typeid(int))
		{
			result.push_back(std::any_cast<int>(item));
		}
		else if (item.type() == typeid(float))
		{
			result.push_back(std::any_cast<float>(item));
		}
		else if (item.type() == typeid(double))
		{
			result.push_back(std::any_cast<double>(item));
		}
		else if (item.type() == typeid(bool))
		{
			result.push_back(std::any_cast<bool>(item));
		}
		else if (item.type() == typeid(std::string))
		{
			result.push_back(std::any_cast<std::string>(item));
		}
		else if (item.type() == typeid(const char*))
		{
			result.push_back(std::string(std::any_cast<const char*>(item)));
		}
		else if (item.type() == typeid(Vector2))
		{
			const Vector2& v = std::any_cast<Vector2>(item);
			result.push_back({ v.x, v.y });
		}
		else if (item.type() == typeid(Vector3))
		{
			const Vector3& v = std::any_cast<Vector3>(item);
			result.push_back({ v.x, v.y, v.z });
		}
		else if (item.type() == typeid(json))
		{
			result.push_back(std::any_cast<json>(item));
		}
		else
		{
			assert(!"–¢‘Î‰ž‚ÌŒ^‚Å‚·");
		}
	}

	return result;
}

json RawJson::CreateObject(const std::vector<std::pair<std::string, std::any>>& keyValues)
{
	json result = json::object();

	for (const auto& [key, value] : keyValues)
	{
		if (value.type() == typeid(int))
		{
			result[key] = std::any_cast<int>(value);
		}
		else if (value.type() == typeid(float))
		{
			result[key] = std::any_cast<float>(value);
		}
		else if (value.type() == typeid(double))
		{
			result[key] = std::any_cast<double>(value);
		}
		else if (value.type() == typeid(bool))
		{
			result[key] = std::any_cast<bool>(value);
		}
		else if (value.type() == typeid(std::string))
		{
			result[key] = std::any_cast<std::string>(value);
		}
		else if (value.type() == typeid(const char*))
		{
			result[key] = std::string(std::any_cast<const char*>(value));
		}
		else if (value.type() == typeid(Vector2))
		{
			const Vector2& v = std::any_cast<Vector2>(value);
			result[key] = { v.x, v.y };
		}
		else if (value.type() == typeid(Vector3))
		{
			const Vector3& v = std::any_cast<Vector3>(value);
			result[key] = { v.x, v.y, v.z };
		}
		else if(value.type() == typeid(json))
		{
			result[key] = std::any_cast<json>(value);
		}
		else
		{
			assert(!"–¢‘Î‰ž‚ÌŒ^‚Å‚·");
		}
	}

	return result;
}

bool RawJson::CheckIsJson(const std::string& str)
{
	try
	{
		json::parse(str);
		return true;
	}
	catch (const json::parse_error&)
	{
		return false;
	}
}
