#include "stage.h"

// ���ėp
#include "../Source/Util/Utils.h"

// �����o�E�@�\
//#include "effectManager.h"
//#include "effectBase.h"
//#include "collisionFunc.h"

// ���ʂŕK�v�ȕ�
//#include "charaManager.h"
//#include "player.h"

using namespace StageDefine;

namespace {

	Object3D* ground;	// �n�ʂ̏��
	Object3D* sky;		// ��̏��
	Object3D* wall;		// �ǂ̏��
}

void Stage::Init() {

	if (ground == nullptr)
		ground = new Object3D(MV1LoadModel("data/model/Stage/Plane1.mv1"), Transform(V3::ZERO, V3::ZERO, V3::HORIZONTAL * 7.0f));

	if (sky == nullptr)
		sky = new Object3D(MV1LoadModel("data/model/Stage/Sky.mv1"), Transform(V3::ZERO, V3::ZERO, Vector3(20.0f)));

	if (wall == nullptr)
		wall = new Object3D(MV1LoadModel("data/model/Stage/StageCollider.mv1"), Transform(V3::ZERO, V3::ZERO, Vector3(STAGE_RANGE / 50.0f)));

	MV1SetupCollInfo(ground->Model(), -1, 8, 8, 8);	//�R���W���������\�z����
	MV1RefreshCollInfo(ground->Model(), -1);		//�R���W���������X�V����

	//EffectManager::PlayEffect3D(EffectDefine::EffectID::eFeild_Sphere_Normal, wall->transform->Copy(), { TrsType::tAll }, 900, true);
}

void Stage::Update() {

	//EffectBase* feildEffect = EffectManager::IsPlay(EffectDefine::EffectID::eFeild_Sphere_Normal, 900);
	//if (feildEffect == nullptr) {
	//	EffectManager::PlayEffect3D(EffectDefine::EffectID::eFeild_Sphere_Normal, wall->transform->Copy(), { TrsType::tAll }, 900, true);
	//	return;
	//}

	//// �v���C���[���X�e�[�W�O���t�߂ɂ���ꍇ�A�G�t�F�N�g�̕s�����x���グ��
	//CharaManager* charaM = FindGameObject<CharaManager>();
	//if (charaM == nullptr) {
	//	feildEffect->SetRGBA(255, 255, 255, 0);
	//	return;
	//}

	//Player* player = charaM->Search<Player>();

	//if (player != nullptr) {
	//	auto collider = player->GetComponent<ColliderCapsule>();
	//	float outerCircumference = STAGE_RANGE * 0.85f;	// �O����������鋗��
	//	float diff = STAGE_RANGE - outerCircumference;	// �O���ƊO������̍�

	//	if (ColFunction::ColCheck3D_Circle(player->transform->position, collider->Radius(), feildEffect->transform->position, outerCircumference).IsCollision()) {
	//		feildEffect->SetRGBA(255, 255, 255, 0);
	//	}
	//	else {
	//		Vector3 VecToPlayer = player->transform->position - feildEffect->transform->position;
	//		float range = ((VecToPlayer).Size() - outerCircumference) / diff;
	//		feildEffect->SetRGBA(255, 255, 255, static_cast<int>(range * 255));
	//	}
	//}
	//else
	//	feildEffect->SetRGBA(255, 255, 255, 0);
}

void Stage::Draw() {

	if (sky != nullptr)
		sky->Draw();

	SetFogColor(50, 50, 150);
	SetFogStartEnd(150.0f, STAGE_RANGE * 5.0f);
	SetFogEnable(true);

	if (ground != nullptr)
		ground->Draw();

	if (wall != nullptr)
		wall->Draw();

	SetFogEnable(false);
}

void Stage::Release() {

	Function::DeletePointer(ground);
	Function::DeletePointer(sky);
	Function::DeletePointer(wall);
}

bool Stage::ColCheckGround(Vector3 begin, Vector3 end, Vector3* hitPos) {

	MV1_COLL_RESULT_POLY hit;
	hit = MV1CollCheck_Line(ground->Model(), 0, begin, end);
	if (hit.HitFlag > 0) {
		if (hitPos != nullptr)	// �����Ƀ|�C���^�������Ă�����A���������
			*hitPos = hit.HitPosition;
		return true;
	}

	return false;
}

bool Stage::ColCheckWall(Vector3 begin, Vector3 end, float r, Vector3* push) {

	bool hitFlag = false;
	VECTOR pushVec = VGet(0, 0, 0);

	MV1SetupCollInfo(wall->Model(), -1, 8, 8, 8);
	MV1_COLL_RESULT_POLY_DIM dim = MV1CollCheck_Capsule(wall->Model(), -1, begin, end, r);
	for (int i = 0; i < dim.HitNum; i++) {
		hitFlag = true;
		// �����蔻�������pushVec�����
		// �|���S���Ɛ����̋��������߂�
		float len = Segment_Triangle_MinLength(begin, end, dim.Dim[i].Position[0], dim.Dim[i].Position[1], dim.Dim[i].Position[2]);
		if (len > r)
			continue;
		// �����Ԃ��ʂ́A(r - len)�ɂȂ�
		// �|���S���̖@�����牟���Ԃ��ʂ���Z����
		VECTOR newPush = dim.Dim[i].Normal * (r - len);
		pushVec += newPush;
		VECTOR pushVecNorm = VNorm(pushVec);
		float pushIn = VDot(pushVecNorm, newPush);
		// ���ꂪ�V����������
		if (pushIn < VSize(pushVec)) {
			pushVec += newPush - pushIn * pushVecNorm;
		}
		else {
			pushVec = newPush;
		}
	}
	MV1CollResultPolyDimTerminate(dim);

	if (push != nullptr)
		*push = pushVec * -1.0f;
	return hitFlag;
}

Object3D* Stage::Ground() { return ground; }
Object3D* Stage::Sky() { return sky; }
Object3D* Stage::Wall() { return wall; }
