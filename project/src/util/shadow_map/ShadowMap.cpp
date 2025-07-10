#include "src/util/shadow_map/ShadowMap.h"

// ���ėp
#include "framework/myDxLib.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/camera/CameraManager.h"

namespace {

	static const Vector3 SHADOW_MAP_DRAW_AREA = Vector3(2500.0f);	// �e�`����s���͈�
	static const Vector3 SHADOW_MAP_DRAW_OFFSET = Vector3(0.0f, 0.0f, 2500.0f);	// �J��������ǂ̈ʒu�܂ŉe�`����s����

	int hShadowMap = -1;		// �V���h�E�}�b�v�̃n���h��
	bool isActive = false;		// �ғ����Ă��邩
}

void ShadowMap::Init(int sizeX, int sizeY) {

	// �V���h�E�}�b�v�𐶐�����
	if (hShadowMap == -1)
		hShadowMap = MakeShadowMap(sizeX, sizeY);

	// �e���o�����C�g�����߂�(�^���Ɍ�����)
	SetShadowMapLightDirection(hShadowMap, VGet(0.5, -1.0f, 0.0f));

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
	Vector3 offset = VTransform(SHADOW_MAP_DRAW_OFFSET, GetCameraViewMatrix());

	Vector3 minPosition = camPos - SHADOW_MAP_DRAW_AREA;// �e�`��J�n�n�_
	Vector3 maxPosition = camPos + offset;				// �e�`��I���n�_

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
