#include "src/util/shadow_map/ShadowMap.h"

// ���ėp
#include "framework/myDxLib.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/camera/CameraManager.h"

namespace {

	static const Vector3 SHADOW_MAP_DRAW_AREA = Vector3(1500.0f);	// �e�`����s���͈�

	int hShadowMap = -1;		// �V���h�E�}�b�v�̃n���h��
	bool isActive = false;		// �ғ����Ă��邩
}

void ShadowMap::Init(int sizeX, int sizeY) {

	// �V���h�E�}�b�v�𐶐�����
	if (hShadowMap == -1)
		hShadowMap = MakeShadowMap(sizeX, sizeY);

	// �e���o�����C�g�����߂�(�^���Ɍ�����)
	SetLightDirection(Vector3(0.5f, -1.0f, 0.0f));
	SetLightAmbColor(COLOR_F(0.4f, 0.4f, 0.8f, 1.0f));
	SetLightDifColor(COLOR_F(0.6f, 0.6f, 0.6f, 1.0f));
	SetShadowMapLightDirection(hShadowMap, GetLightDirection());

	// �ғ����ɐݒ�
	SetIsActive(true);
}

void ShadowMap::DrawBegin(int cameraIndex) {

	if (isActive == false)
		return;

	Camera* camera = CameraManager::GetCamera(cameraIndex);
	if (not camera)
		return;

	Vector3 camPos = camera->transform->Global().position;
	Vector3 offset = VTransform(SHADOW_MAP_DRAW_AREA, MGetRotY(camera->transform->Global().rotation.y));

	Vector3 minPosition = camPos - offset;	// �e�`��J�n�n�_
	Vector3 maxPosition = camPos + offset;	// �e�`��I���n�_

	// �e�v�Z������G���A�͈�
	SetShadowMapDrawArea(hShadowMap, minPosition, maxPosition);

	ShadowMap_DrawSetup(hShadowMap);
}

void ShadowMap::DrawEnd() {

	if (isActive == false)
		return;

	ShadowMap_DrawEnd();
	SetUseShadowMap(0, hShadowMap);
}

void ShadowMap::CleanUp() {

	SetUseShadowMap(0, -1);
}

void ShadowMap::Draw(void (*func)(), int cameraIndex) {

	ShadowMap::DrawBegin(cameraIndex);
	func();
	ShadowMap::DrawEnd();
	func();
}

void ShadowMap::Release() {

	if (hShadowMap != -1) {
		DeleteShadowMap(hShadowMap);
		hShadowMap = -1;
	}
}

void ShadowMap::SetIsActive(bool value) {

	isActive = value;
}

bool ShadowMap::IsActive() {

	return isActive;
}
