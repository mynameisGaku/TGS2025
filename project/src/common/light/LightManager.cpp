#include "lightManager.h"

// ◇汎用
#include <unordered_map>

// ◇演出・機能
#include "../Source/ImGuiTree/imGuiManager.h"

// ◇個別で必要な物
#include "Light.h"

namespace {

	std::unordered_map<LightType, Light*>* lights;	// ライトの情報
}

void LightManager::Init() {

	if (lights == nullptr)
		lights = new std::unordered_map<LightType, Light*>();

#ifdef IMGUI
	ImGuiRoot* LightTree = ImGuiManager::AddRoot(new ImGuiRoot("Light"));
#endif // IMGUI

	SetLightEnable(true);

	CreateLightDir();
	//CreateLightPoint();
	//CreateLightSpot();
}

void LightManager::Update() {

	if (lights == nullptr)
		return;

	for (const auto& light : *lights) {
		if (light.second != nullptr)
			light.second->Update();
	}
}

void LightManager::Draw() {

	if (lights == nullptr)
		return;

	for (const auto& light : *lights) {
		if (light.second != nullptr)
			light.second->Draw();
	}
}

void LightManager::Release() {

	if (lights != nullptr) {

		for (auto itr = lights->begin(); itr != lights->end(); itr++) {
			if (itr->second != nullptr) {
				delete itr->second;
				itr->second = nullptr;
			}
		}
		lights->clear();
		delete lights;
		lights = nullptr;

		DeleteLightHandleAll();
	}

#ifdef IMGUI
	ImGuiManager::Delete("Light");
#endif
}

void LightManager::CreateLightDir() {

	if (lights == nullptr)
		return;

	if (lights->contains(LightType::Directional))
		return;

	LightInfo lightInfo;
	lightInfo.type = LightType::Directional;
	lightInfo.colorDif = GetColorF(1.0f, 1.0f, 0.8f, 1.0f);
	lightInfo.colorSpc = GetColorF(1.0f, 1.0f, 0.8f, 1.0f);
	lightInfo.colorAmb = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);

	Light* dir = new Light(lightInfo);
	dir->SetDegRotation(Vector3(-180.0f, -180.0f, 0.0f));

	(*lights)[LightType::Directional] = dir;
}

void LightManager::CreateLightPoint() {

	if (lights == nullptr)
		return;

	if (lights->contains(LightType::Point))
		return;

	LightInfo lightInfo;
	lightInfo.type = LightType::Point;
	lightInfo.range = 1550.0f;
	lightInfo.atten0 = 0.001f;
	lightInfo.atten1 = 0.001f;
	lightInfo.atten2 = 0.0f;
	lightInfo.colorDif = GetColorF(1.0f, 0.5f, 0.0f, 1.0f);
	lightInfo.colorSpc = GetColorF(1.0f, 0.5f, 0.0f, 1.0f);
	lightInfo.colorAmb = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);

	Light* point = new Light(lightInfo);
	point->transform->position = Vector3::SetY(1550.0f);

	(*lights)[LightType::Point] = point;
}

void LightManager::CreateLightSpot() {

	if (lights == nullptr)
		return;

	if (lights->contains(LightType::Spot))
		return;

	LightInfo lightInfo;
	lightInfo.type = LightType::Spot;
	lightInfo.outAngle = 100.0f;
	lightInfo.inAngle = 0.0f;
	lightInfo.range = 1995.0f;
	lightInfo.atten0 = 0.0f;
	lightInfo.atten1 = 0.0005f;
	lightInfo.atten2 = 0.0f;
	lightInfo.colorDif = GetColorF(1.0f, 0.5f, 0.0f, 1.0f);
	lightInfo.colorSpc = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	lightInfo.colorAmb = GetColorF(1.0f, 0.5f, 0.0f, 1.0f);

	Light* spot = new Light(lightInfo);
	spot->transform->position = Vector3::SetY(2000.0f);
	spot->SetDegRotation(Vector3::SetY(-180.0f));

	(*lights)[LightType::Spot] = spot;
}

void LightManager::SetIsActive(const bool& value, const LightDefine::LightType& type) {

	if (lights == nullptr || type == LightType::None)
		return;

	if (type == LightType::Max) {
		for (const auto& light : *lights) {
			if (light.second != nullptr)
				light.second->SetIsActive(value);
		}
	}
	else {
		if ((*lights)[type] != nullptr)
			(*lights)[type]->SetIsActive(value);
	}
}

Light* LightManager::GetLight(const LightDefine::LightType& type) {

	if (lights == nullptr || type == LightType::None || type == LightType::Max)
		return nullptr;

	return (*lights)[type];
}
