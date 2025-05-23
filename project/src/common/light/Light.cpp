#include "Light.h"

// �����o�E�@�\
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

	// �p�x�̐ݒ�B�x���@���ʓx�@�ɕϊ�
	info.outAngle = MathUtil::ToRadians(m_outAngle);
	info.inAngle = MathUtil::ToRadians(m_inAngle);
	transform->rotation = Vector3(MathUtil::ToRadians(m_rotation.x), MathUtil::ToRadians(m_rotation.y), MathUtil::ToRadians(m_rotation.z));

	// �g�U�F��ݒ�
	SetLightDifColorHandle(info.handle, info.colorDif);

	// ���ːF��ݒ�
	SetLightSpcColorHandle(info.handle, info.colorSpc);

	// ���F��ݒ肷��
	SetLightAmbColorHandle(info.handle, info.colorAmb);

	// ���C�g�̊p�x��K��
	SetLightDirectionHandle(info.handle, transform->rotation);
	
	// ���C�g�̍��W��K��
	SetLightPositionHandle(info.handle, transform->position);
	
	// ���C�g�̗L��������K��
	SetLightRangeAttenHandle(info.handle, info.range, info.atten0, info.atten1, info.atten2);
	
	// ���C�g�̏Ǝˊp�x�p�����[�^�[��K������
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

	// ImGuiManager����Light�Ƃ������O�̃c���[��T��
	ImGuiRoot* lightTree = ImGuiManager::FindRoot("Light");
	if (lightTree == nullptr)
		return;

	imguiLabel = "Light Direction";

	// ���s�����Ɋւ���f�[�^�����c���[��T��
	ImGuiRoot* lightDirTree = lightTree->SearchChildren(imguiLabel);

	// ���s�����Ɋւ���f�[�^�����c���[�����݂��Ȃ��ꍇ�A�\�z����
	if (lightDirTree == nullptr)
		lightDirTree = lightTree->AddChild(new ImGuiRoot(imguiLabel));

	//====================================================================================================
	// ����]

	// ��LDRT = Light-Direction-Rotation-Tree (���s�����̉�]�p��ύX����c���[���\�z����)
	ImGuiRoot* LDRT = lightDirTree->AddChild(new ImGuiRoot("Rotation"));

	LDRT->Add(new ImGuiNode_SliderFloat("Rotation X", &m_rotation.x, -180.0f, 180.0f));
	LDRT->Add(new ImGuiNode_SliderFloat("Rotation Y", &m_rotation.y, -180.0f, 180.0f));
	LDRT->Add(new ImGuiNode_SliderFloat("Rotation Z", &m_rotation.z, -180.0f, 180.0f));

	//====================================================================================================
	// ���g�U�F

	// ��LDDCT = Light-Direction-Diffuse-Color-Tree (���s�����̊g�U�F��ύX����c���[���\�z����)
	ImGuiRoot* LDDCT = lightDirTree->AddChild(new ImGuiRoot("Dif Color"));

	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color R", &info.colorDif.r, 0.0f, 1.0f, "%.3f"));
	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color G", &info.colorDif.g, 0.0f, 1.0f, "%.3f"));
	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color B", &info.colorDif.b, 0.0f, 1.0f, "%.3f"));
	LDDCT->Add(new ImGuiNode_SliderFloat("Dif Color A", &info.colorDif.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// �����ːF

	// ��LDSCT = Light-Direction-Specular-Color-Tree (���s�����̔��ːF��ύX����c���[���\�z����)
	ImGuiRoot* LDSCT = lightDirTree->AddChild(new ImGuiRoot("Spc Color"));

	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color R", &info.colorSpc.r, 0.0f, 1.0f, "%.3f"));
	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color G", &info.colorSpc.g, 0.0f, 1.0f, "%.3f"));
	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color B", &info.colorSpc.b, 0.0f, 1.0f, "%.3f"));
	LDSCT->Add(new ImGuiNode_SliderFloat("Spc Color A", &info.colorSpc.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// �����F

	// ��LDACT = Light-Direction-Ambient-Color-Tree (���s�����̊��F��ύX����c���[���\�z����)
	ImGuiRoot* LDACT = lightDirTree->AddChild(new ImGuiRoot("Amb Color"));

	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color R", &info.colorAmb.r, 0.0f, 1.0f, "%.3f"));
	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color G", &info.colorAmb.g, 0.0f, 1.0f, "%.3f"));
	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color B", &info.colorAmb.b, 0.0f, 1.0f, "%.3f"));
	LDACT->Add(new ImGuiNode_SliderFloat("Amb Color A", &info.colorAmb.a, 0.0f, 1.0f, "%.3f"));
}

void Light::initImGuiNode_LightPoint() {

	// ImGuiManager����Light�Ƃ������O�̃c���[��T��
	ImGuiRoot* lightTree = ImGuiManager::FindRoot("Light");
	if (lightTree == nullptr)
		return;

	imguiLabel = "Light Point";

	// �_�����Ɋւ���f�[�^�����c���[��T��
	ImGuiRoot* lightPointTree = lightTree->SearchChildren(imguiLabel);

	// �_�����Ɋւ���f�[�^�����c���[�����݂��Ȃ��ꍇ�A�\�z����
	if (lightPointTree == nullptr)
		lightPointTree = lightTree->AddChild(new ImGuiRoot(imguiLabel));

	//====================================================================================================
	// �����W

	// ��LPPT = Light-Point-Position-Tree (�_�����̍��W��ύX����c���[���\�z����)
	ImGuiRoot* LPPT = lightPointTree->AddChild(new ImGuiRoot("Position"));

	LPPT->Add(new ImGuiNode_SliderFloat("Position X", &transform->position.x, -5000.0f, 5000.0f));
	LPPT->Add(new ImGuiNode_SliderFloat("Position Y", &transform->position.y, -5000.0f, 5000.0f));
	LPPT->Add(new ImGuiNode_SliderFloat("Position Z", &transform->position.z, -5000.0f, 5000.0f));

	//====================================================================================================
	// ���L�������Ƌ��������p�����[�^�[

	// ��LPRAAT = Light-Point-Range-And-Atten-Tree (�_�����̗L�������Ƌ��������p�����[�^�[��ύX����c���[���\�z����)
	ImGuiRoot* LPRAAT = lightPointTree->AddChild(new ImGuiRoot("Range & Atten"));

	LPRAAT->Add(new ImGuiNode_SliderFloat("Range", &info.range, -5000.0f, 5000.0f));
	LPRAAT->Add(new ImGuiNode_SliderFloat("Atten 0", &info.atten0, 0.0f, 1.0f, "%.5f"));
	LPRAAT->Add(new ImGuiNode_SliderFloat("Atten 1", &info.atten1, 0.0f, 1.0f, "%.5f"));
	LPRAAT->Add(new ImGuiNode_SliderFloat("Atten 2", &info.atten2, 0.0f, 1.0f, "%.5f"));

	//====================================================================================================
	// ���g�U�F

	// ��LPDCT = Light-Point-Diffuse-Color-Tree (�_�����̊g�U�F��ύX����c���[���\�z����)
	ImGuiRoot* LPDCT = lightPointTree->AddChild(new ImGuiRoot("Dif Color"));

	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color R", &info.colorDif.r, 0.0f, 1.0f, "%.3f"));
	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color G", &info.colorDif.g, 0.0f, 1.0f, "%.3f"));
	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color B", &info.colorDif.b, 0.0f, 1.0f, "%.3f"));
	LPDCT->Add(new ImGuiNode_SliderFloat("Dif Color A", &info.colorDif.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// �����ːF

	// ��LPSCT = Light-Point-Specular-Color-Tree (�_�����̔��ːF��ύX����c���[���\�z����)
	ImGuiRoot* LPSCT = lightPointTree->AddChild(new ImGuiRoot("Spc Color"));

	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color R", &info.colorSpc.r, 0.0f, 1.0f, "%.3f"));
	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color G", &info.colorSpc.g, 0.0f, 1.0f, "%.3f"));
	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color B", &info.colorSpc.b, 0.0f, 1.0f, "%.3f"));
	LPSCT->Add(new ImGuiNode_SliderFloat("Spc Color A", &info.colorSpc.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// �����F

	// ��LPACT = Light-Point-Ambient-Color-Tree(�_�����̊��F��ύX����c���[���\�z����)
	ImGuiRoot* LPACT = lightPointTree->AddChild(new ImGuiRoot("Amb Color"));

	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color R", &info.colorAmb.r, 0.0f, 1.0f, "%.3f"));
	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color G", &info.colorAmb.g, 0.0f, 1.0f, "%.3f"));
	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color B", &info.colorAmb.b, 0.0f, 1.0f, "%.3f"));
	LPACT->Add(new ImGuiNode_SliderFloat("Amb Color A", &info.colorAmb.a, 0.0f, 1.0f, "%.3f"));
}

void Light::initImGuiNode_LightStop() {

	// ImGuiManager����Light�Ƃ������O�̃c���[��T��
	ImGuiRoot* lightTree = ImGuiManager::FindRoot("Light");
	if (lightTree == nullptr)
		return;

	imguiLabel = "Light Spot";

	// �Ɩ������Ɋւ���f�[�^�����c���[��T��
	ImGuiRoot* lightSpotTree = lightTree->SearchChildren(imguiLabel);

	// �Ɩ������Ɋւ���f�[�^�����c���[�����݂��Ȃ��ꍇ�A�\�z����
	if (lightSpotTree == nullptr)
		lightSpotTree = lightTree->AddChild(new ImGuiRoot(imguiLabel));

	//====================================================================================================
	// �����W

	// ��LSPT = Light-Spot-Position-Tree (�Ɩ������̍��W��ύX����c���[���\�z����)
	ImGuiRoot* LSPT = lightSpotTree->AddChild(new ImGuiRoot("Position"));

	LSPT->Add(new ImGuiNode_SliderFloat("Position X", &transform->position.x, -5000.0f, 5000.0f));
	LSPT->Add(new ImGuiNode_SliderFloat("Position Y", &transform->position.y, -5000.0f, 5000.0f));
	LSPT->Add(new ImGuiNode_SliderFloat("Position Z", &transform->position.z, -5000.0f, 5000.0f));

	//====================================================================================================
	// ����]

	// ��LSRT = Light-Spot-Rotation-Tree (�Ɩ������̉�]�p��ύX����c���[���\�z����)
	ImGuiRoot* LSRT = lightSpotTree->AddChild(new ImGuiRoot("Rotation"));

	LSRT->Add(new ImGuiNode_SliderFloat("Rotation X", &m_rotation.x, -180.0f, 180.0f));
	LSRT->Add(new ImGuiNode_SliderFloat("Rotation Y", &m_rotation.y, -180.0f, 180.0f));
	LSRT->Add(new ImGuiNode_SliderFloat("Rotation Z", &m_rotation.z, -180.0f, 180.0f));

	//====================================================================================================
	// ���L�������Ƌ��������p�����[�^�[

	// ��LSRAAT = Light-Spot-Range-And-Atten-Tree (�Ɩ������̗L�������Ƌ��������p�����[�^�[��ύX����c���[���\�z����)
	ImGuiRoot* LSRAAT = lightSpotTree->AddChild(new ImGuiRoot("Range & Atten"));

	LSRAAT->Add(new ImGuiNode_SliderFloat("Range", &info.range, -5000.0f, 5000.0f));
	LSRAAT->Add(new ImGuiNode_SliderFloat("Atten 0", &info.atten0, 0.0f, 1.0f, "%.5f"));
	LSRAAT->Add(new ImGuiNode_SliderFloat("Atten 1", &info.atten1, 0.0f, 1.0f, "%.5f"));
	LSRAAT->Add(new ImGuiNode_SliderFloat("Atten 2", &info.atten2, 0.0f, 1.0f, "%.5f"));

	//====================================================================================================
	// ���L�������Ƌ��������p�����[�^�[

	// ��LSAT = Light-Spot-Angle-Tree (�Ɩ������̓��O�p�x��ύX����c���[���\�z����)
	ImGuiRoot* LSAT = lightSpotTree->AddChild(new ImGuiRoot("Angle"));

	LSAT->Add(new ImGuiNode_SliderFloat("OutAngle", &m_outAngle, -180.0f, 180.0f));
	LSAT->Add(new ImGuiNode_SliderFloat("InAngle", &m_inAngle, -180.0f, 180.0f));

	//====================================================================================================
	// ���g�U�F

	// ��LSDCT = Light-Spot-Diffuse-Color-Tree (�_�����̊g�U�F��ύX����c���[���\�z����)
	ImGuiRoot* LSDCT = lightSpotTree->AddChild(new ImGuiRoot("Dif Color"));

	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color R", &info.colorDif.r, 0.0f, 1.0f, "%.3f"));
	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color G", &info.colorDif.g, 0.0f, 1.0f, "%.3f"));
	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color B", &info.colorDif.b, 0.0f, 1.0f, "%.3f"));
	LSDCT->Add(new ImGuiNode_SliderFloat("Dif Color A", &info.colorDif.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// �����ːF

	// ��LSSCT = Light-Spot-Specular-Color-Tree (�_�����̔��ːF��ύX����c���[���\�z����)
	ImGuiRoot* LSSCT = lightSpotTree->AddChild(new ImGuiRoot("Spc Color"));

	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color R", &info.colorSpc.r, 0.0f, 1.0f, "%.3f"));
	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color G", &info.colorSpc.g, 0.0f, 1.0f, "%.3f"));
	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color B", &info.colorSpc.b, 0.0f, 1.0f, "%.3f"));
	LSSCT->Add(new ImGuiNode_SliderFloat("Spc Color A", &info.colorSpc.a, 0.0f, 1.0f, "%.3f"));

	//====================================================================================================
	// �����F

	// ��LSACT = Light-Spot-Ambient-Color-Tree (�_�����̊��F��ύX����c���[���\�z����)
	ImGuiRoot* LSACT = lightSpotTree->AddChild(new ImGuiRoot("Amb Color"));

	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color R", &info.colorAmb.r, 0.0f, 1.0f, "%.3f"));
	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color G", &info.colorAmb.g, 0.0f, 1.0f, "%.3f"));
	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color B", &info.colorAmb.b, 0.0f, 1.0f, "%.3f"));
	LSACT->Add(new ImGuiNode_SliderFloat("Amb Color A", &info.colorAmb.a, 0.0f, 1.0f, "%.3f"));
}

#endif // IMGUI