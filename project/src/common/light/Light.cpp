#include "Light.h"

// ◇演出・機能
#include "src/util/math/mathUtils.h"
#include "src/util/debug/imgui/imGuiManager.h"

Light::Light(const LightInfo& _info) {

	CreateLightHandle(_info);

	m_rotation = Vector3::Zero;
	m_outAngle = 0.0f;
	m_inAngle = 0.0f;

#ifdef IMGUI
	initImGuiNode();
#endif // IMGUI
}

Light::~Light() {

	if (info.handle > 0) {
		DeleteLightHandle(info.handle);
		info.handle = -1;
	}

#ifdef IMGUI
	ImGuiManager::Delete(imguiLabel);
#endif // IMGUI
}

void Light::Update() {

	LightUpdate();

	Object3D::Update();
}

void Light::LightUpdate() {

	if (info.handle < 0)
		return;

	SetLightEnableHandle(info.handle, IsActive());

	if (IsActive() == false)
		return;

	// 角度の設定。度数法を弧度法に変換
	info.outAngle = MathUtil::ToRadians(m_outAngle);
	info.inAngle = MathUtil::ToRadians(m_inAngle);
	transform->rotation = Vector3(MathUtil::ToRadians(m_rotation.x), MathUtil::ToRadians(m_rotation.y), MathUtil::ToRadians(m_rotation.z));

	// 拡散色を設定
	SetLightDifColorHandle(info.handle, info.colorDif);

	// 反射色を設定
	SetLightSpcColorHandle(info.handle, info.colorSpc);

	// 環境色を設定する
	SetLightAmbColorHandle(info.handle, info.colorAmb);

	// ライトの角度を適応
	SetLightDirectionHandle(info.handle, transform->rotation);
	
	// ライトの座標を適応
	SetLightPositionHandle(info.handle, transform->position);
	
	// ライトの有効距離を適応
	SetLightRangeAttenHandle(info.handle, info.range, info.atten0, info.atten1, info.atten2);
	
	// ライトの照射角度パラメーターを適応する
	SetLightAngleHandle(info.handle, info.outAngle, info.inAngle);
}

void Light::CreateLightHandle(const LightInfo& _info) {

	info = _info;

	switch (info.type) {
	case LightType::Directional:
		info.handle = CreateDirLightHandle(transform->rotation);
		break;

	case LightType::Point:
		info.handle = CreatePointLightHandle(transform->position, info.range, info.atten0, info.atten1, info.atten2);
		break;

	case LightType::Spot:
		info.handle = CreateSpotLightHandle(transform->position, transform->rotation, info.outAngle, info.inAngle, info.range, info.atten0, info.atten1, info.atten2);
		break;

	default:
		info.handle = -1;
		break;
	}

}

#ifdef IMGUI

void Light::initImGuiNode() {

	switch (info.type) {
	case LightType::Directional:	initImGuiNode_LightDir();		break;
	case LightType::Point:			initImGuiNode_LightPoint();		break;
	case LightType::Spot:			initImGuiNode_LightStop();		break;
	}
}

void Light::initImGuiNode_LightDir() {

	// ImGuiManagerからLightという名前のツリーを探す
	ImGuiRoot* lightTree = ImGuiManager::FindRoot("Light");
	if (lightTree == nullptr)
		return;

	imguiLabel = "Light Direction";

	// 平行光源に関するデータを持つツリーを探す
	ImGuiRoot* lightDirTree = lightTree->SearchChildren(imguiLabel);

	// 平行光源に関するデータを持つツリーが存在しない場合、構築する
	if (lightDirTree == nullptr)
		lightDirTree = lightTree->AddChild(new ImGuiRoot(imguiLabel));

	//====================================================================================================
	// ▼回転

	// ◇LDRT = Light-Direction-Rotation-Tree (平行光源の回転角を変更するツリーを構築する)
	ImGuiRoot* LDRT = lightDirTree->AddChild(new ImGuiRoot("Rotation"));

	LDRT->Add(new ImGuiNode_SliderFloat("Rotation X", &m_rotation.x, -180.0f, 180.0f));
	LDRT->Add(new ImGuiNode_SliderFloat("Rotation Y", &m_rotation.y, -180.0f, 180.0f));
	LDRT->Add(new ImGuiNode_SliderFloat("Rotation Z", &m_rotation.z, -180.0f, 180.0f));

	//====================================================================================================
	// ▼拡散色

	// ◇LDDCT = Light-Direction-Diffuse-Color-Tree (平行光源の拡散色を変更するツリーを構築する)
	ImGuiRoot* LDDCT = lightDirTree->AddChild(new ImGuiRoot("Dif Color"));

	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color R", &info.colorDif.r, 0.0f, 1.0f, "%.3f"));
	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color G", &info.colorDif.g, 0.0f, 1.0f, "%.3f"));
	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color B", &info.colorDif.b, 0.0f, 1.0f, "%.3f"));
	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color A", &info.colorDif.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// ▼反射色

	// ◇LDSCT = Light-Direction-Specular-Color-Tree (平行光源の反射色を変更するツリーを構築する)
	ImGuiRoot* LDSCT = lightDirTree->AddChild(new ImGuiRoot("Spc Color"));

	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color R", &info.colorSpc.r, 0.0f, 1.0f, "%.3f"));
	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color G", &info.colorSpc.g, 0.0f, 1.0f, "%.3f"));
	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color B", &info.colorSpc.b, 0.0f, 1.0f, "%.3f"));
	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color A", &info.colorSpc.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// ▼環境色

	// ◇LDACT = Light-Direction-Ambient-Color-Tree (平行光源の環境色を変更するツリーを構築する)
	ImGuiRoot* LDACT = lightDirTree->AddChild(new ImGuiRoot("Amb Color"));

	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color R", &info.colorAmb.r, 0.0f, 1.0f, "%.3f"));
	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color G", &info.colorAmb.g, 0.0f, 1.0f, "%.3f"));
	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color B", &info.colorAmb.b, 0.0f, 1.0f, "%.3f"));
	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color A", &info.colorAmb.a, 0.0f, 1.0f, "%.3f"));
}

void Light::initImGuiNode_LightPoint() {

	// ImGuiManagerからLightという名前のツリーを探す
	ImGuiRoot* lightTree = ImGuiManager::FindRoot("Light");
	if (lightTree == nullptr)
		return;

	imguiLabel = "Light Point";

	// 点光源に関するデータを持つツリーを探す
	ImGuiRoot* lightPointTree = lightTree->SearchChildren(imguiLabel);

	// 点光源に関するデータを持つツリーが存在しない場合、構築する
	if (lightPointTree == nullptr)
		lightPointTree = lightTree->AddChild(new ImGuiRoot(imguiLabel));

	//====================================================================================================
	// ▼座標

	// ◇LPPT = Light-Point-Position-Tree (点光源の座標を変更するツリーを構築する)
	ImGuiRoot* LPPT = lightPointTree->AddChild(new ImGuiRoot("Position"));

	LPPT->Add(new ImGuiNode_SliderFloat("Position X", &transform->position.x, -5000.0f, 5000.0f));
	LPPT->Add(new ImGuiNode_SliderFloat("Position Y", &transform->position.y, -5000.0f, 5000.0f));
	LPPT->Add(new ImGuiNode_SliderFloat("Position Z", &transform->position.z, -5000.0f, 5000.0f));

	//====================================================================================================
	// ▼有効距離と距離減衰パラメーター

	// ◇LPRAAT = Light-Point-Range-And-Atten-Tree (点光源の有効距離と距離減衰パラメーターを変更するツリーを構築する)
	ImGuiRoot* LPRAAT = lightPointTree->AddChild(new ImGuiRoot("Range & Atten"));

	LPRAAT->Add(new ImGuiNode_SliderFloat("Range", &info.range, -5000.0f, 5000.0f));
	LPRAAT->Add(new ImGuiNode_SliderFloat("Atten 0", &info.atten0, 0.0f, 1.0f, "%.5f"));
	LPRAAT->Add(new ImGuiNode_SliderFloat("Atten 1", &info.atten1, 0.0f, 1.0f, "%.5f"));
	LPRAAT->Add(new ImGuiNode_SliderFloat("Atten 2", &info.atten2, 0.0f, 1.0f, "%.5f"));

	//====================================================================================================
	// ▼拡散色

	// ◇LPDCT = Light-Point-Diffuse-Color-Tree (点光源の拡散色を変更するツリーを構築する)
	ImGuiRoot* LPDCT = lightPointTree->AddChild(new ImGuiRoot("Dif Color"));

	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color R", &info.colorDif.r, 0.0f, 1.0f, "%.3f"));
	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color G", &info.colorDif.g, 0.0f, 1.0f, "%.3f"));
	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color B", &info.colorDif.b, 0.0f, 1.0f, "%.3f"));
	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color A", &info.colorDif.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// ▼反射色

	// ◇LPSCT = Light-Point-Specular-Color-Tree (点光源の反射色を変更するツリーを構築する)
	ImGuiRoot* LPSCT = lightPointTree->AddChild(new ImGuiRoot("Spc Color"));

	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color R", &info.colorSpc.r, 0.0f, 1.0f, "%.3f"));
	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color G", &info.colorSpc.g, 0.0f, 1.0f, "%.3f"));
	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color B", &info.colorSpc.b, 0.0f, 1.0f, "%.3f"));
	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color A", &info.colorSpc.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// ▼環境色

	// ◇LPACT = Light-Point-Ambient-Color-Tree(点光源の環境色を変更するツリーを構築する)
	ImGuiRoot* LPACT = lightPointTree->AddChild(new ImGuiRoot("Amb Color"));

	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color R", &info.colorAmb.r, 0.0f, 1.0f, "%.3f"));
	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color G", &info.colorAmb.g, 0.0f, 1.0f, "%.3f"));
	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color B", &info.colorAmb.b, 0.0f, 1.0f, "%.3f"));
	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color A", &info.colorAmb.a, 0.0f, 1.0f, "%.3f"));
}

void Light::initImGuiNode_LightStop() {

	// ImGuiManagerからLightという名前のツリーを探す
	ImGuiRoot* lightTree = ImGuiManager::FindRoot("Light");
	if (lightTree == nullptr)
		return;

	imguiLabel = "Light Spot";

	// 照明光源に関するデータを持つツリーを探す
	ImGuiRoot* lightSpotTree = lightTree->SearchChildren(imguiLabel);

	// 照明光源に関するデータを持つツリーが存在しない場合、構築する
	if (lightSpotTree == nullptr)
		lightSpotTree = lightTree->AddChild(new ImGuiRoot(imguiLabel));

	//====================================================================================================
	// ▼座標

	// ◇LSPT = Light-Spot-Position-Tree (照明光源の座標を変更するツリーを構築する)
	ImGuiRoot* LSPT = lightSpotTree->AddChild(new ImGuiRoot("Position"));

	LSPT->Add(new ImGuiNode_SliderFloat("Position X", &transform->position.x, -5000.0f, 5000.0f));
	LSPT->Add(new ImGuiNode_SliderFloat("Position Y", &transform->position.y, -5000.0f, 5000.0f));
	LSPT->Add(new ImGuiNode_SliderFloat("Position Z", &transform->position.z, -5000.0f, 5000.0f));

	//====================================================================================================
	// ▼回転

	// ◇LSRT = Light-Spot-Rotation-Tree (照明光源の回転角を変更するツリーを構築する)
	ImGuiRoot* LSRT = lightSpotTree->AddChild(new ImGuiRoot("Rotation"));

	LSRT->Add(new ImGuiNode_SliderFloat("Rotation X", &m_rotation.x, -180.0f, 180.0f));
	LSRT->Add(new ImGuiNode_SliderFloat("Rotation Y", &m_rotation.y, -180.0f, 180.0f));
	LSRT->Add(new ImGuiNode_SliderFloat("Rotation Z", &m_rotation.z, -180.0f, 180.0f));

	//====================================================================================================
	// ▼有効距離と距離減衰パラメーター

	// ◇LSRAAT = Light-Spot-Range-And-Atten-Tree (照明光源の有効距離と距離減衰パラメーターを変更するツリーを構築する)
	ImGuiRoot* LSRAAT = lightSpotTree->AddChild(new ImGuiRoot("Range & Atten"));

	LSRAAT->Add(new ImGuiNode_SliderFloat("Range", &info.range, -5000.0f, 5000.0f));
	LSRAAT->Add(new ImGuiNode_SliderFloat("Atten 0", &info.atten0, 0.0f, 1.0f, "%.5f"));
	LSRAAT->Add(new ImGuiNode_SliderFloat("Atten 1", &info.atten1, 0.0f, 1.0f, "%.5f"));
	LSRAAT->Add(new ImGuiNode_SliderFloat("Atten 2", &info.atten2, 0.0f, 1.0f, "%.5f"));

	//====================================================================================================
	// ▼有効距離と距離減衰パラメーター

	// ◇LSAT = Light-Spot-Angle-Tree (照明光源の内外角度を変更するツリーを構築する)
	ImGuiRoot* LSAT = lightSpotTree->AddChild(new ImGuiRoot("Angle"));

	LSAT->Add(new ImGuiNode_SliderFloat("OutAngle", &m_outAngle, -180.0f, 180.0f));
	LSAT->Add(new ImGuiNode_SliderFloat("InAngle", &m_inAngle, -180.0f, 180.0f));

	//====================================================================================================
	// ▼拡散色

	// ◇LSDCT = Light-Spot-Diffuse-Color-Tree (点光源の拡散色を変更するツリーを構築する)
	ImGuiRoot* LSDCT = lightSpotTree->AddChild(new ImGuiRoot("Dif Color"));

	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color R", &info.colorDif.r, 0.0f, 1.0f, "%.3f"));
	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color G", &info.colorDif.g, 0.0f, 1.0f, "%.3f"));
	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color B", &info.colorDif.b, 0.0f, 1.0f, "%.3f"));
	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color A", &info.colorDif.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// ▼反射色

	// ◇LSSCT = Light-Spot-Specular-Color-Tree (点光源の反射色を変更するツリーを構築する)
	ImGuiRoot* LSSCT = lightSpotTree->AddChild(new ImGuiRoot("Spc Color"));

	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color R", &info.colorSpc.r, 0.0f, 1.0f, "%.3f"));
	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color G", &info.colorSpc.g, 0.0f, 1.0f, "%.3f"));
	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color B", &info.colorSpc.b, 0.0f, 1.0f, "%.3f"));
	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color A", &info.colorSpc.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// ▼環境色

	// ◇LSACT = Light-Spot-Ambient-Color-Tree (点光源の環境色を変更するツリーを構築する)
	ImGuiRoot* LSACT = lightSpotTree->AddChild(new ImGuiRoot("Amb Color"));

	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color R", &info.colorAmb.r, 0.0f, 1.0f, "%.3f"));
	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color G", &info.colorAmb.g, 0.0f, 1.0f, "%.3f"));
	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color B", &info.colorAmb.b, 0.0f, 1.0f, "%.3f"));
	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color A", &info.colorAmb.a, 0.0f, 1.0f, "%.3f"));
}

#endif // IMGUI