#include "Animator.h"

// ���ėp
#include "../../Library/time.h"
#include "../../Library/resourceLoader.h"
#include "../../Library/magic_enum.hpp"
#include "../Util/Utils.h"
#include "../Object3D.h"
#include <assert.h>

Animator::Animator() {

	parentObj = nullptr;
	animCsv = nullptr;

	current.attachID = -1;
	prev.attachID = -1;

	parentModel = -1;
	mergeTime = 0.0f;
	mergeTimeMax = 1.0f;
	playSpeed = 1.0f;
	frameRate = 1.0f;

	origin = "";
	playingLabel = "";

	offsetMatrix = MGetIdent();
}

Animator::~Animator() {

	// �A�j���[�V�������f�^�b�`����

	if (current.attachID >= 1.0f)
		MV1DetachAnim(parentModel, current.attachID);

	if (prev.attachID >= 1.0f)
		MV1DetachAnim(parentModel, prev.attachID);

	// Csv�f�[�^���폜����
	Function::DeletePointer(animCsv);

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

	parentObj = dynamic_cast<Object3D*>(parent);

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

void Animator::LoadAnim(std::string animFilePath, std::string label, bool loop, bool fixedRoot, std::string extension) {

	anims[label].handle = ResourceLoader::MV1LoadModel((animFilePath + extension).c_str());
	assert(anims[label].handle >= 0);

	anims[label].animName = label;
	anims[label].isLoop = loop;
	anims[label].isFixedRoot = fixedRoot;
}

void Animator::LoadAnimCsv(std::string csvFilePath, std::string animFilePath) {

	if (csvFilePath == "")
		return;

	Function::DeletePointer(animCsv);

	animCsv = new CsvReader(csvFilePath);

	for (int i = 1; i < animCsv->Column(); i++) {

		std::string labelName = animCsv->GetString(i, LabelName);	// ���x����

		// ���x�������ݒ肳��Ă��Ȃ��Ȃ�A���̗v�f��
		if (labelName == "")
			continue;

		std::string fileName = animFilePath + animCsv->GetString(i, ResourceName);	// �f�[�^�����݂���ꏊ		
		bool loop = animCsv->GetBool(i, IsLoop);			// ���[�v�Đ��̗L��
		bool fixedRoot = animCsv->GetBool(i, IsFixedRoot);	// ���[�J�����W�̌Œ艻�̗L��

		// �f�[�^�̓ǂݍ���
		LoadAnim(fileName, labelName, loop, fixedRoot);

		anims[labelName].startFrame		= animCsv->GetFloat(i, StartFrame);
		anims[labelName].endFrame		= animCsv->GetFloat(i, EndFrame);
		anims[labelName].defAnimSpeed	= animCsv->GetFloat(i, DefAnimSpeed);

		// �R�}���h���ݒ肳��Ă��Ȃ��Ȃ�A���̗v�f��
		if (animCsv->GetString(i, Command) == "")
			continue;

		// �e�R�}���h���擾����
		for (int j = i; j < animCsv->Column(); j++) {

			std::string nextLabelName = animCsv->GetString(j, LabelName);	// ���̗�̃��x����

			// ���̃��x���������̃��x�����ƈ�v���Ă��Ȃ����A
			// ���̃��x�����ɖ��O�������Ă�����for���甲����
			if (nextLabelName != labelName && nextLabelName != "")
				break;

			AnimationEvent* event = new AnimationEvent(this);

			std::string  commandType = animCsv->GetString(j, Command);		// �R�}���h
			std::string commandContents = animCsv->GetString(j, Details);	// ���s���e
			float runFrame = animCsv->GetFloat(j, RunFrame);				// ���s�t���[��

			event->SetCommand(commandType, commandContents, runFrame);

			anims[labelName].event.push_back(event);
		}
	}
}

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

	// ��Csv�f�[�^���ǂݍ��܂�Ă�����
	if (animCsv != nullptr) {

		for (auto& event : anims[playingLabel].event) {
			event->Update();
		}
	}
}

void Animator::Play(std::string label, float speed)
{
	if (label == playingLabel) // ����ID�Ȃ疳������
		return;

	for (const auto& anim : anims) {
		// �����̃��x�����ƈ�v���Ă��Ȃ��ꍇ�A���̗v�f��
		if (anim.first != label)
			continue;

		// �O��̃A�j���[�V������؂藣��
		if (prev.attachID >= 0)
			MV1DetachAnim(parentModel, prev.attachID);

		playingLabel = label;
		prev = current;
		mergeTime = 0.0f;

		// �Đ����x��ݒ肷��
		if (speed < 0)
			playSpeed = anims[label].defAnimSpeed;
		else
			playSpeed = speed;

		// �Đ��J�n�n�_��ݒ肷��
		if (anims[label].startFrame < 0) {
			current.nowFrame	= 0.0f;
			current.beforeFrame = 0.0f;
		}
		else {
			current.nowFrame	= anims[label].startFrame;
			current.beforeFrame = anims[label].startFrame;
		}

		// �R�}���h�̎��s�����𕜊�������
		for (int i = 0; i < anims[playingLabel].event.size(); i++)
			anims[playingLabel].event[i]->SetUseCommand(true);

		// �A�j���[�V������K��
		current.attachID = MV1AttachAnim(parentModel, 0, anim.second.handle);
		current.attachName = label;

		// �Đ��I���n�_��ݒ肷��
		if (anims[label].endFrame <= 0)
			current.maxFrame = MV1GetAttachAnimTotalTime(parentModel, current.attachID);
		else
			current.maxFrame = anims[label].endFrame;

		break;
	}
}
