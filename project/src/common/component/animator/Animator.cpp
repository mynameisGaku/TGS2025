#include "Animator.h"

// ���ėp
#include "src/util/time/GameTime.h"
#include "src/util/file/resource_loader/resourceLoader.h"
#include <vendor/magic_enum/magic_enum.hpp>
#include "src/util/object3D/Object3D.h"
#include "src/util/file/json/settings_json.h"
#include <assert.h>
#include <src/util/ptr/PtrUtil.h>

//ToDo:�O����
namespace {
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

	offsetMatrix = MGetIdent();
}

Animator::~Animator() {

	// �A�j���[�V�������f�^�b�`����
	if (current != nullptr)
		delete current;
	for (auto item : currentSubs) {
		AttachedAnimation_Sub* currentSub = item.second;
		if (currentSub != nullptr) {
			delete currentSub;
		}
	}
	for (AttachedAnimation* prev : prevs) {
		if (prev != nullptr) {
			delete prev;
		}
	}

	DeleteAnimInfos();
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
		mergeTime += GTime.deltaTime * playSpeed;
		const float animMergeTimeMax = GetAnimInfo().mergeTimeMax;

		// �u�����h�I�����Ȃ�
		if (mergeTime >= animMergeTimeMax) {

			// �O�A�j���[�V�����I������
			for (AttachedAnimation* prev : prevs) {
				delete prev;
				prev = nullptr;
			}
			prevs.clear();

			mergeTime = animMergeTimeMax;
		}

		// ���Ԃɉ����ău�����h����ς���
		if (animMergeTimeMax > 0)
			rate = mergeTime / animMergeTimeMax;
		else
			rate = 1.0f;
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
	MATRIX prevM = MGetScale(Vector3::Zero);

	// �A�j���[�V�����̍X�V
	for (AttachedAnimation_Main* prev : prevs) {
		prev->Update();
	}
	current->Update();

	// ���[�g�␳�̌v�Z
	for (AttachedAnimation_Main* prev : prevs) {
		prev->UpdateRootMatrix();
	}
	current->UpdateRootMatrix();

	// ���[�g�̈ړ��s����擾
	currentM = MScale(current->RootMatrix(), current->BlendRate());
	for (AttachedAnimation_Main* prev : prevs) {
		prevM += MScale(prev->RootMatrix(), prev->BlendRate());
	}

	// ���p���ƑO�p��������
	currentM += prevM;

	// �I�t�Z�b�g��K�p
	currentM *= offsetMatrix;

	// �Z�b�g
	MV1SetFrameUserLocalMatrix(parentModel, hRoot, currentM);

#if TRUE

	// ���Ԃɉ����ău�����h����ς���
	float rateSub = 1.0f;

	// �u�����h�i�s����
	for (auto& item : prevSubs) {
		const std::string& frameName = item.first;
		std::list<AttachedAnimation_Sub*>& prevSubList = item.second;
		float& mergeTimeSub = mergeTimeSubs.at(frameName);
		AttachedAnimation_Sub* currentSub = currentSubs.at(frameName);

		if (not prevSubList.empty()) {
			// �u�����h���Ԃ�i�߂�
			mergeTimeSub += GTime.deltaTime * playSpeed;
			const float animMergeTimeMax = currentSub->Info().mergeTimeMax;

			// �u�����h�I�����Ȃ�
			if (mergeTimeSub >= animMergeTimeMax) {

				// �O�A�j���[�V�����I������
				for (AttachedAnimation_Sub* prevSub : prevSubList) {
					delete prevSub;
					prevSub = nullptr;
				}
				prevSubList.clear();

				mergeTimeSub = animMergeTimeMax;
			}

			// ���Ԃɉ����ău�����h����ς���
			if (animMergeTimeMax > 0)
				rateSub = mergeTimeSub / animMergeTimeMax;
			else
				rateSub = 1.0f;
			// �S�̂�1�ɂȂ�悤�Ƀu�����h
			currentSub->SetBlendRate(rateSub);

			float prevRate = 1.0f - rateSub;
			for (AttachedAnimation_Sub* prevSub : prevSubList) {
				prevSub->SetBlendRate(prevRate);
			}
		}

	}

#endif 

	// �T�u�A�j���[�V�����X�V
	for (auto& item : currentSubs)
	{
		std::string frameName = item.first;
		AttachedAnimation_Sub* currentSub = item.second;

		currentSub->UpdateBrendRate(current->AttachID());

		currentSub->Update();
	}
	// �T�u�A�j���[�V�����X�V
	for (auto& item : prevSubs)
	{
		std::string frameName = item.first;
		std::list<AttachedAnimation_Sub*> prevSubList = item.second;
		for (AttachedAnimation_Sub* prevSub : prevSubList)
		{
			//prevSub->UpdateBrendRate(current->AttachID());

			prevSub->Update();
		}
	}

	for (auto& item : frameMatrix)
	{
		int frame = MV1SearchFrame(parentModel, item.first.c_str());
		MV1ResetFrameUserLocalMatrix(parentModel, frame);

		MATRIX frameM = MV1GetFrameLocalMatrix(parentModel, frame);
		frameM = item.second * frameM;

		MV1SetFrameUserLocalMatrix(parentModel, frame, frameM);
	}
}

void Animator::LoadAnim(std::string folder, std::string name, AnimOption option) {
	std::string fullPath = folder + name;
	// �g���q��⊮
	if (name.find(".mv1") == std::string::npos)
	{
		fullPath += ".mv1";
	}
	anims[name].handle = ResourceLoader::MV1LoadModel(fullPath.c_str());
	assert(anims[name].handle >= 0);

	anims[name].animName = name;
	anims[name].option = option;

	anims[name].startFrame = 0.0;
	anims[name].endFrame = MV1GetAnimTotalTime(anims[name].handle, 0);
}

void Animator::LoadAnimsFromJson(std::string path) {
	// �g���q��⊮
	if (path.find(".json") == std::string::npos) {
		path += ".json";
	}

	std::string key = path;

	// JSON �ǂݍ���
	auto jsonLoader = Settings_json::Inst();
	jsonLoader->LoadSettingJson(path, key);

	std::string folder = jsonLoader->GetOrDefault<std::string>("Folder", "", key);

	std::unordered_map<std::string, nlohmann::json> files;
	files = jsonLoader->GetOrDefault<std::unordered_map<std::string, nlohmann::json>>("Files", files, key);

	for (auto itr = files.begin(); itr != files.end(); itr++) {
		std::string name = (*itr).first;
		AnimOption option;

		option.defaultAnimSpeed = jsonLoader->GetOrDefault<float>("Files." + name + ".DefaultAnimSpeed", 1.0f, key);
		option.isFixedRoot[0] = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsFixedRoot.X", false, key);
		option.isFixedRoot[1] = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsFixedRoot.Y", false, key);
		option.isFixedRoot[2] = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsFixedRoot.Z", false, key);
		option.isLoop = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsLoop", false, key);
		option.offset.x = jsonLoader->GetOrDefault<float>("Files." + name + ".Offset.X", 0.0f, key);
		option.offset.y = jsonLoader->GetOrDefault<float>("Files." + name + ".Offset.Y", 0.0f, key);
		option.offset.z = jsonLoader->GetOrDefault<float>("Files." + name + ".Offset.Z", 0.0f, key);

		LoadAnim(folder, name, option);

		anims.at(name).startFrame = jsonLoader->GetOrDefault<float>("Files." + name + ".StartFrame", anims.at(name).startFrame, key);
		anims.at(name).endFrame = jsonLoader->GetOrDefault<float>("Files." + name + ".EndFrame", anims.at(name).endFrame, key);
		anims.at(name).mergeTimeMax = jsonLoader->GetOrDefault<float>("Files." + name + ".MergeTime", mergeTimeMax, key);
	}
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

	current = new AttachedAnimation_Main(parentModel, anim);
	current->SetPlaySpeed(playSpeed);

	if (prevs.empty())
	{
		mergeTime = anim.mergeTimeMax;
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

void Animator::PlaySub(std::string frameName, std::string label, float speed) {
	if (not anims.contains(label))	// ���݂��Ȃ��A�j���[�V�������Ȃ疳������
		return;

	// �����A�j���[�V�����Ȃ疳������
	if (currentSubs.contains(frameName)) {
		if (currentSubs.at(frameName)->Info().animName == label)
			return;
	}

	AnimInfo anim = anims.at(label);

	// ���݂̃T�u�A�j���[�V�������u�����h���ɂ���
	if (currentSubs.contains(frameName)) {

		AttachedAnimation_Sub* prevSub = currentSubs.at(frameName);
		currentSubs.erase(frameName);
		prevSubs.at(frameName).push_back(prevSub);

		for (AttachedAnimation_Sub* prevSub : prevSubs[frameName]) {
			prevSub->RefreshDefaultBlendRate();
		}
	}
	// ���݂̃��C���A�j���[�V�������T�u�A�j���[�V�����ɂ��ău�����h���ɂ���
	else if (current != nullptr)	{
		AttachedAnimation_Sub* prevSub = new AttachedAnimation_Sub(parentModel, anims.at(playingLabel), frameName);
		if (not prevSubs.contains(frameName)) {
			prevSubs.emplace(frameName, std::list<AttachedAnimation_Sub*>());
		}
		prevSub->SetBlendRate(1.0f);

		prevSubs.at(frameName).push_back(prevSub);
	}

	AttachedAnimation_Sub* newSub = new AttachedAnimation_Sub(parentModel, anim, frameName);

	newSub->SetPlaySpeed(playSpeed);
	currentSubs.emplace(frameName, newSub);

	if (prevSubs.empty()) {
		mergeTimeSubs[frameName] = anim.mergeTimeMax;
		newSub->SetBlendRate(1.0f);
	}
	else {
		mergeTimeSubs[frameName] = 0.0f;
		newSub->SetBlendRate(0.0f);
	}
}

void Animator::StopSub(std::string frameName)
{
	// �u�����h���g��
	if (not currentSubs.contains(frameName)) return;

	PlaySub(frameName, anims.at(playingLabel).animName, playSpeed);
}

void Animator::DeleteAnimInfos() {
	// �A�j���[�V���������������
	for (auto& anim : anims) {
		for (auto& event : anim.second.event) {
			PtrUtil::SafeDelete(event);
		}
		anim.second.event.clear();
		MV1DeleteModel(anim.second.handle);
	}

	anims.clear();
}

void Animator::SetPlaySpeed(float speed) {
	playSpeed = speed;
	current->SetPlaySpeed(playSpeed);
}
