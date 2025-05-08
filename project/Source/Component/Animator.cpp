#include "Animator.h"

// ���ėp
#include "../../Library/time.h"
#include "../../Library/resourceLoader.h"
#include "../../Library/magic_enum.hpp"
#include "../Util/Utils.h"
#include "../Object3D.h"
#include <assert.h>

namespace {
	static const float MERGE_TIME_MAX = 0.25f;
	static const int ANIM_FRAMERATE = 60;
	static const int BLEND_COUNT_MAX = 5;
};

Animator::Animator() {
	current = nullptr;

	parentModel = -1;
	mergeTime = 0.0f;
	mergeTimeMax = 1.0f;
	playSpeed = 1.0f;
	frameRate = 1.0f;

	origin = "";
	playingLabel = "";
}

Animator::~Animator() {

	// �A�j���[�V�������f�^�b�`����
	if (current != nullptr)
		delete current;
	for (AttachedAnimation* prev : prevs) {
		if (prev != nullptr) {
			delete prev;
		}
	}

	// �R�}���h�̎��s�����𕜊�������
	for (auto& anim : anims) {
		for (auto& event : anim.second.event) {
			Function::DeletePointer(event);
		}
		anim.second.event.clear();
	}

	// �A�j���[�V���������������
	anims.clear();
}

void Animator::Init(std::string _origin, float _frameRate, float _mergeTimeMax) {

	Object3D* parentObj = dynamic_cast<Object3D*>(parent);

	// �e�̃��f����ݒ�
	if (parentObj != nullptr)
		parentModel = parentObj->Model();

	// ���f���̌��_�̖��O
	origin = _origin;

	// �A�j���[�V�����̌o�ߑ��x
	frameRate = _frameRate;

	// ���b�ŕ⊮�����邩
	mergeTimeMax = _mergeTimeMax;
}

#if FALSE
void Animator::Update() {

	// �@�O�p����␳
	// �A��p����␳
	// �B�O�p���ƌ�p���������Ńu�����h

	int hRoot = MV1SearchFrame(parentModel, origin.c_str());

	// ��U���Z�b�g
	MV1ResetFrameUserLocalMatrix(parentModel, hRoot);

	MATRIX currentM = MGetIdent();	// ���݂̃A�j���[�V�����̍s��
	MATRIX prevM = MGetIdent();		// �O��̃A�j���[�V�����̍s��

	// ���O��̃A�j���[�V�������Đ����Ȃ�
	if (prev.attachID >= 0) {
		// ����ԏ���
		{
			mergeTime += Time::DeltaTimeLapseRate() * playSpeed;

			if (mergeTime >= mergeTimeMax) {
				MV1DetachAnim(parentModel, prev.attachID);
				MV1SetAttachAnimBlendRate(parentModel, current.attachID, 1.0f);
				prev.attachID = -1;
			}
			else {
				float rate = mergeTime / mergeTimeMax;
				MV1SetAttachAnimBlendRate(parentModel, prev.attachID, 1.0f - rate);
				MV1SetAttachAnimBlendRate(parentModel, current.attachID, rate);
			}
		}

		// �O��̍s���ݒ�
		prevM = MV1GetFrameLocalMatrix(parentModel, hRoot);

		// ���␳���̍��W���擾
		const Vector3 framePos = MV1GetAttachAnimFrameLocalPosition(parentModel, prev.attachID, hRoot);

		// ���W�ړ���ł�����
		prevM *= MGetTranslate(framePos * -1.0f);

		// Y�����ێ��AXZ�����_
		prevM *= MGetTranslate(Vector3(0.0f, framePos.y, 0.0f));
	}

	// ���A�j���[�V�������Đ����Ȃ�
	if (current.attachID >= 0) {

		current.beforeFrame = current.nowFrame;
		current.nowFrame += frameRate * Time::DeltaTimeLapseRate() * playSpeed;

		// �A�j���[�V���������Đ��t���[���܂ōĐ�������
		if (current.nowFrame >= current.maxFrame) {

			if (!anims[playingLabel].isLoop)
				current.nowFrame = current.maxFrame;
			else {
				current.nowFrame -= current.maxFrame;

				// �R�}���h�̎��s�����𕜊�������
				for (int i = 0; i < anims[playingLabel].event.size(); i++)
					anims[playingLabel].event[i]->SetUseCommand(true);
			}
		}

		// �A�j���[�V������K��������
		MV1SetAttachAnimTime(parentModel, current.attachID, current.nowFrame);

		// ���݂̍s����擾
		currentM = MV1GetFrameLocalMatrix(parentModel, hRoot);


		// �����[�J�����W�̌Œ艻���L���Ȃ�
		if (anims[playingLabel].isFixedRoot)
		{
			// ���␳���̍��W���擾
			const Vector3 framePos = MV1GetAttachAnimFrameLocalPosition(parentModel, current.attachID, hRoot);

			// ���W�ړ���ł�����
			currentM *= MGetTranslate(framePos * -1.0f);

			// Y�����ێ��AXZ�����_
			currentM *= MGetTranslate(Vector3(0.0f, framePos.y, 0.0f));
		}

		// ���O��̃A�j���[�V�������Đ����Ȃ�A�u�����h����
		if (prev.attachID >= 0)
		{
			// root�p�������炩�ɑJ��
			float progress = mergeTime / mergeTimeMax;

			// ���p���ƑO�p��������
			// �Œ�l + (�ő�l - �Œ�l) * progress
			currentM = MAdd(prevM, MAdd(currentM, prevM * MGetScale(V3::ONE * -1.0f)) * MGetScale(V3::ONE * progress));
		}

		// �e�X�g
		currentM *= offsetMatrix;

		// �Z�b�g
		MV1SetFrameUserLocalMatrix(parentModel, hRoot, currentM);
	}


#if FALSE
	// ��Csv�f�[�^���ǂݍ��܂�Ă�����
	if (animCsv != nullptr) {

		for (auto& event : anims[playingLabel].event) {
			event->Update();
		}
	}
#endif // FALSE
}

#endif // FALSE

void Animator::Update() {
	if (current == nullptr) return;

	// ���[�g�t���[���擾
	int hRoot = MV1SearchFrame(parentModel, origin.c_str());

	// ��U���Z�b�g
	MV1ResetFrameUserLocalMatrix(parentModel, hRoot);

	// ���Ԃɉ����ău�����h����ς���
	float rate = 1.0f;

	// �u�����h�i�s����
	if (prevs.size() > 0) {
		// �u�����h���Ԃ�i�߂�
		mergeTime += Time::DeltaTime() * playSpeed;

		// �u�����h�I�����Ȃ�
		if (mergeTime >= MERGE_TIME_MAX) {

			// �O�A�j���[�V�����I������
			for (AttachedAnimation* prev : prevs) {
				delete prev;
				prev = nullptr;
			}
			prevs.clear();

			mergeTime = MERGE_TIME_MAX;
		}

		// ���Ԃɉ����ău�����h����ς���
		rate = mergeTime / MERGE_TIME_MAX;
		// �S�̂�1�ɂȂ�悤�Ƀu�����h
		current->SetBlendRate(rate);

		if (prevs.size() > 0) {
			float prevRate = 1.0f - rate;
			for (AttachedAnimation* prev : prevs) {
				prev->SetBlendRate(prevRate);
			}
		}
	}

	// ���[�g�̈ړ��s��
	MATRIX currentM = MGetIdent();
	MATRIX prevM = MGetScale(V3::ZERO);

	// �A�j���[�V�����̍X�V
	for (AttachedAnimation* prev : prevs) {
		prev->Update();
	}
	current->Update();

	// ���[�g�␳�̌v�Z
	for (AttachedAnimation* prev : prevs) {
		prev->UpdateRootMatrix();
	}
	current->UpdateRootMatrix();

	// ���[�g�̈ړ��s����擾
	currentM = MScale(current->RootMatrix(), current->BlendRate());
	for (AttachedAnimation* prev : prevs) {
		prevM += MScale(prev->RootMatrix(), prev->BlendRate());
	}

	// ���p���ƑO�p��������
	currentM += prevM;

	// �Z�b�g
	MV1SetFrameUserLocalMatrix(parentModel, hRoot, currentM);

	for (auto& item : frameMatrix)
	{
		int frame = MV1SearchFrame(parentModel, item.first.c_str());
		//MV1ResetFrameUserLocalMatrix(parentModel, frame);

		MATRIX frameM = MV1GetFrameLocalMatrix(parentModel, frame);
		frameM = item.second * frameM;

		MV1SetFrameUserLocalMatrix(parentModel, frame, frameM);
	}
}

void Animator::LoadAnim(std::string folder, std::string name, AnimOption option) {
	std::string fullPath = folder + name;
	if (name.find(".mv1") == std::string::npos)
	{
		fullPath += ".mv1";
	}
	anims[name].handle = ResourceLoader::MV1LoadModel(fullPath.c_str());
	assert(anims[name].handle >= 0);

	anims[name].animName = name;
	anims[name].option = option;

	// ToDo:start,end���ǂ����悤
	anims[name].startFrame = 0.0;
	anims[name].endFrame = MV1GetAnimTotalTime(anims[name].handle, 0);
}

void Animator::Play(std::string label, float speed) {
	if (label == playingLabel) // �����A�j���[�V�������Ȃ疳������
		return;
	if (not anims.contains(label))	// ���݂��Ȃ��A�j���[�V�������Ȃ疳������
		return;

	AnimInfo anim = anims.at(label);

	// �ǉ��O�ɏ����̂��H
	bool over = false;
	float rate = 0.0f;
	if (prevs.size() >= BLEND_COUNT_MAX) {
		rate = prevs.front()->BlendRate();
		delete prevs.front();
		prevs.pop_front();
		over = true;
	}

	if (current != nullptr) {
		prevs.push_back(current);
		mergeTime = 0.0f;
	}

	for (AttachedAnimation* prev : prevs) {
		prev->RefreshDefaultBlendRate();
	}

	if (over)
	{
		for (AttachedAnimation* prev : prevs) {
			prev->SetDefaultBlendRate(prev->DefaultBlendRate() + rate / prevs.size());
		}
	}

	playingLabel = label;

	current = new AttachedAnimation(parentModel, anim);
	current->SetPlaySpeed(playSpeed);

	if (prevs.empty())
	{
		mergeTime = 1.0f;
		current->SetBlendRate(1.0f);
	}

#if FALSE
	// �Đ��J�n�n�_��ݒ肷��
	if (anim.startFrame < 0) {
		current->SetFrame(0.0f);
	}
	else {
		current->SetFrame(anim.startFrame);
	}

	// �Đ��I���n�_��ݒ肷��
	if (anim.endFrame <= 0)
		current.maxFrame = MV1GetAttachAnimTotalTime(parentModel, current.attachID);
	else
		current.maxFrame = anim.endFrame;
#endif // FALSE
}
