#include "ShadowMap.h"

// ���ėp
#include "../Library/myDxLib.h"
#include "../Library/resourceLoader.h"

namespace {

	static const Vector3 SHADOW_MAP_DRAW_AREA = Vector3(-1200.0f);
	static const Vector3 SHADOW_MAP_DRAW_OFFSET = Vector3(0.0f, 0.0f, 500.0f);

	int hShadowMap = -1;		// �V���h�E�}�b�v�̃n���h��
	bool isActive = false;		// �ғ����Ă��邩
}

void ShadowMap::Init() {

	// �V���h�E�}�b�v�𐶐�����
	if (hShadowMap == -1)
		hShadowMap = MakeShadowMap(4096, 4096);

	// �e���o�����C�g�����߂�(�^���Ɍ�����)
	SetShadowMapLightDirection(hShadowMap, VGet(0.5, -1.0f, 0.0f));

	SetIsActive(true);
}

void ShadowMap::DrawBegin() {

	if (isActive == false)
		return;

	// �J�����̍��W
	VECTOR camPos = GetCameraPosition();

	// Direct3D�Ŏ����K�p�����r���[�|�[�g�s����擾����
	MATRIX camM = GetCameraAPIViewportMatrix();

	// ���΍��W�ƃr���[�{�[�g�s��ŉe�v�Z�͈͂����߂�
	VECTOR offset = VTransform(SHADOW_MAP_DRAW_OFFSET, camM);

	// �e�v�Z������G���A�͈�
	SetShadowMapDrawArea(hShadowMap, camPos + offset, camPos + SHADOW_MAP_DRAW_AREA);

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
