#include "src/util/sound/SoundManager.h"

#include "src/util/math/MathUtil.h"
#include "src/util/ptr/PtrUtil.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/util/file/csv/CsvReader.h"
#include "src/util/file/json/settings_json.h"
#include <vendor/magic_enum/magic_enum.hpp>

namespace {

	std::unordered_map<std::string, SoundInfo>* soundInfoDatas;	// �T�E���h�̏�������
	std::unordered_map<SoundCategory, float>* categoryVolumeRate;	// �J�e�S���\�ʂ̉��ʔ{��
	std::list<SoundBase*>* sounds;	// �Đ�����Ă���T�E���h
	std::string* csvFilePath;		// �ǂݍ��ݗpCsv�t�@�C���̃p�X

	bool initialize = false;	// �������������s��ꂽ��
}

void SoundManager::Init() {

	if (soundInfoDatas == nullptr) {
		soundInfoDatas = new std::unordered_map<std::string, SoundInfo>();
	}

	if (categoryVolumeRate == nullptr) {
		categoryVolumeRate = new std::unordered_map<SoundCategory, float>();

		for (int i = 0; i < static_cast<int>(SoundCategory::cMax); i++) {
			SoundCategory category = static_cast<SoundCategory>(i);	// �J�e�S���[
			(*categoryVolumeRate)[category] = 1.0f;
		}
	}

	if (sounds == nullptr) {
		sounds = new std::list<SoundBase*>();
	}

	initialize = false;
}

void SoundManager::Start() {

	if (initialize)
		return;

#ifdef IMGUI
		InitImGui();
#endif // IMGUI

	initialize = true;
}

void SoundManager::Update() {

	// �������������܂��s���Ă��Ȃ��ꍇ
	if (initialize == false)
		Start();

	if (categoryVolumeRate != nullptr) {
		for (auto& itr : *categoryVolumeRate) {
			itr.second = MathUtil::Rounding(itr.second, 2);
		}
	}

	// �Đ����̃T�E���h���Ȃ��ꍇ
	if (sounds == nullptr)
		return;

	// �Đ����̃T�E���h��Update���Ăяo��
	for (auto itr = sounds->begin(); itr != sounds->end();) {
		if ((*itr)->IsPlaying() && (*itr)->IsDestroy() == false) {
			(*itr)->Update();
			itr++;
			continue;
		}

		delete* itr;
		itr = sounds->erase(itr);
		if (itr == sounds->end()) break;
	}
}

void SoundManager::Release() {

	StopAll();

	// �Đ����̃T�E���h�̎��̂��폜����
	if (sounds != nullptr) {
		for (auto itr = sounds->begin(); itr != sounds->end();) {
			if (*itr == nullptr) {
				itr++;
				continue;
			}

			delete* itr;
			itr = sounds->erase(itr);
			if (itr == sounds->end()) break;
		}

		sounds->clear();
		PtrUtil::SafeDelete(sounds);
	}

	// �J�e�S���[�ʂ̉��ʔ{�����폜����
	PtrUtil::SafeDelete(categoryVolumeRate);

	// �ێ����Ă����ǂݍ��ݗpCsv�t�@�C���̃p�X���폜����
	PtrUtil::SafeDelete(csvFilePath);

	// �ǂݍ��񂾑S�ẴT�E���h�f�[�^���������
	AllReleaseInfo();
}

void SoundManager::ApplyCategoryVolume() {

	// �T�E���h�f�[�^�̕ۑ����\��
	if (soundInfoDatas == nullptr)
		return;

	for (auto& itr : *soundInfoDatas) {
		itr.second.curVolume = static_cast<int>(itr.second.defVolume * (*categoryVolumeRate)[itr.second.category]);
	}
}

void SoundManager::Load(const SoundInfo& info) {

	// �T�E���h�f�[�^�̕ۑ����\��
	if (soundInfoDatas == nullptr)
		return;

	// �����f�[�^�����ɑ��݂��Ă���ꍇ�A�ǉ����Ȃ�
	if ((*soundInfoDatas).contains(info.typeName))
		return;

	// �T�E���h���
	SoundInfo newInfo = SoundInfo(info);

	// �n���h�������݂��Ȃ��ꍇ
	if (newInfo.handle < 0)
		newInfo.handle = ResourceLoader::LoadSoundMem(info.fileName + info.typeName);

	// �f�[�^�̓o�^
	(*soundInfoDatas)[info.typeName] = newInfo;
}

void SoundManager::LoadToCsv(const std::string& filename) {

	// �T�E���h�f�[�^�̕ۑ����\��
	if (soundInfoDatas == nullptr)
		return;

	// Csv�f�[�^���擾����
	CsvReader* csv = new CsvReader(filename);

	// Csv�f�[�^�����݂��Ȃ��ꍇ
	if (csv == nullptr)
		return;

	// �ēǂݍ��ݗp��sv�t�@�C���̃p�X��ۑ����Ă���
	if (csvFilePath == nullptr)
		csvFilePath = new std::string();

	*csvFilePath = filename;

	for (int i = 0; i < csv->Column(); i++) {
		
		std::string beginning = csv->GetString(i, 0);	// �ŏ��̍s

		// �R�����g�Ȃ玟�̗v�f��
		if (beginning.substr(0, 2) == "//")
			continue;

		// �T�E���h�̏��
		SoundInfo info;

		//==================================================
		// ���f�[�^���A�t�@�C���p�X�̐ݒ�

		info.typeName = beginning;
		info.fileName = csv->GetString(i, 1);
		
		//==================================================
		// �������̕��ސݒ�

		std::string category = csv->GetString(i, 2);

		if (category == "BGM")
			info.category = SoundCategory::cBGM;
		else if (category == "ENV")
			info.category = SoundCategory::cENV;
		else if (category == "SE")
			info.category = SoundCategory::cSE;
		else if (category == "Voice")
			info.category = SoundCategory::cVoice;
		else
			assert(0 && "�����̕��ރf�[�^���Ԉ���Ă��܂��B");

		//==================================================
		// �����艹�ʂ̐ݒ�

		info.defVolume = csv->GetInt(i, 3);
		info.curVolume = info.defVolume;

		//==================================================
		// ���Đ��`���̐ݒ�

		std::string playType = csv->GetString(i, 4);

		if (playType == "LOOP")
			info.playType = DX_PLAYTYPE_LOOP;
		else if (playType == "BACK")
			info.playType = DX_PLAYTYPE_BACK;
		else if (playType == "NORMAL")
			info.playType = DX_PLAYTYPE_NORMAL;
		else
			assert(0 && "�Đ��`���̃f�[�^���Ԉ���Ă��܂��B");

		//==================================================
		// ���ǂݍ��ݏ���

		Load(info);
	}

	delete csv;
}

void SoundManager::LoadFromJson(const std::string& filename)
{
	auto jsonSetting = Settings_json::Inst();
	jsonSetting->LoadSettingJson(filename, filename);

	auto json = jsonSetting->GetJson(filename);

	for (const auto& [type, value] : json["Params"].items())
	{
		const std::string& path = value["Path"];
		const auto& datas = value["Datas"];
		std::string cat = value["Category"];

		for (const auto& data : datas)
		{
			std::string fileName = data["FileName"];
			std::string name = data["Name"];
			int volume = data["Volume"];
			bool isLoop = data["IsLoop"];

			std::string fullPath = path + fileName;

			SoundInfo info{};
			info.typeName = type + "_" + name;
			info.fileName = fullPath;

			if (cat == "BGM")
				info.category = SoundCategory::cBGM;
			else if (cat == "ENV")
				info.category = SoundCategory::cENV;
			else if (cat == "SE")
				info.category = SoundCategory::cSE;
			else if (cat == "Voice")
				info.category = SoundCategory::cVoice;
			else
				info.category = SoundCategory::cMaster; // �f�t�H���g�̓}�X�^�[

			info.defVolume = volume;
			info.curVolume = volume;
			info.playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_NORMAL;
			info.handle = ResourceLoader::LoadSoundMem(fullPath);

			Load(info);
		}
	}
}

SoundBase* SoundManager::Play(const std::string& typeName, const std::string& label) {

	// �f�[�^�����݂��Ă��邩���m�F����
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// ���ɍĐ����Ȃ�A��~���Ă���Đ�����
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// ���̂𐶐�
	SoundBase* sound = new SoundBase();
	
	// �Đ�����
	sound->Play(&(*soundInfoDatas)[typeName], label);

	// �Đ����T�E���h�̃��X�g�ɒǉ�
	sounds->push_back(sound);

	return sound;
}

SoundBase* SoundManager::PlayRandom(const std::vector<std::string>& typeName, const std::string& label) {

	// �f�[�^�����݂��Ă��邩���m�F����
	if (typeName.empty())
		return nullptr;

	// �ő吔�����߂�
	int size = static_cast<int>(typeName.size()) - 1;

	// �������擾
	int random = GetRand(size);

	return Play(typeName[random], label);
}

SoundBase* SoundManager::PlaySetPan(const std::string& typeName, const std::string& label, Vector3* playPos) {

	// �f�[�^�����݂��Ă��邩���m�F����
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// ���ɍĐ����Ȃ�A��~���Ă���Đ�����
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// ���̂𐶐�
	SoundBase* sound = new SoundBase();
	
	// �Đ�����
	sound->PlaySetPan(&(*soundInfoDatas)[typeName], label, playPos);

	// �Đ����T�E���h�̃��X�g�ɒǉ�
	sounds->push_back(sound);
	
	return sound;
}

void SoundManager::SetPan(const std::string& typeName, const std::string& label, Vector3* playPos) {

	// �Đ�����
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return;

	// �p���̐ݒ��K��������
	sound->SetPan(playPos);
}

SoundBase* SoundManager::PlaySetFrequency(const std::string& typeName, const std::string& label, const float& frequency) {

	// �f�[�^�����݂��Ă��邩���m�F����
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// ���ɍĐ����Ȃ�A��~���Ă���Đ�����
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// ���̂𐶐�
	SoundBase* sound = new SoundBase();
	
	// �Đ�����
	sound->PlaySetFrequency(&(*soundInfoDatas)[typeName], label, frequency);

	// �Đ����T�E���h�̃��X�g�ɒǉ�
	sounds->push_back(sound);

	return sound;
}

void SoundManager::SetFrequency(const std::string& typeName, const std::string& label, const float& frequency) {

	// �Đ�����
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return;

	// ���g���̐ݒ��K��������
	sound->SetFrequency(frequency);
}

SoundBase* SoundManager::FadeIn(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing) {

	// �f�[�^�����݂��Ă��邩���m�F����
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// ���ɍĐ����Ȃ�A��~���Ă���Đ�����
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// ���̂𐶐�
	SoundBase* sound = new SoundBase();

	// �t�F�[�h���
	SoundFade fade;
	float begin = 0.0f;	// �n�_
	float end = static_cast<float>((*soundInfoDatas)[typeName].defVolume);	// �I�_
	fade.SetEasing(begin, end, sec, easing, true);

	// �Đ�����
	sound->FadeIn(&(*soundInfoDatas)[typeName], label, fade);

	// �Đ����T�E���h�̃��X�g�ɒǉ�
	sounds->push_back(sound);

	return sound;
}

SoundBase* SoundManager::FadeOut(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing, const bool& isFadeOutEnd) {

	// �Đ�����
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return nullptr;

	// �t�F�[�h���
	SoundFade fade;
	float begin = static_cast<float>(sound->Info()->curVolume);
	float end = 0.0f;
	fade.SetEasing(begin, end, sec, easing, true);

	sound->FadeOut(fade, isFadeOutEnd);

	return sound;
}

SoundBase* SoundManager::IsPlaying(const std::string& typeName, const std::string& label) {

	// �Đ����T�E���h�̊Ǘ����o���ĂȂ��ꍇ
	if (sounds == nullptr)
		return nullptr;

	for (const auto& itr : *sounds) {
		// �����̏��ƈ�v���Ă��邩�A�������`�F�b�N���s��
		if (itr->CheckConsistency(typeName, label))
			return itr;
	}

	return nullptr;
}

void SoundManager::Stop(const std::string& typeName, const std::string& label) {

	// �Đ�����
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return;

	sound->Stop();
}

void SoundManager::StopCategory(const SoundCategory& category) {

	// �Đ����T�E���h�̊Ǘ����o���ĂȂ��ꍇ
	if (sounds == nullptr)
		return;

	// �Đ�����Ă���T�E���h�̒�����A���ނ���v�������̍Đ����~�߂�
	for (const auto& itr : *sounds) {
		if (itr->Info()->category == category)
			itr->Stop();
	}
}

void SoundManager::StopAll() {

	// �Đ����T�E���h�̊Ǘ����o���ĂȂ��ꍇ
	if (sounds == nullptr)
		return;

	for (const auto& itr : *sounds)
		itr->Stop();
}

void SoundManager::SetVolumeRate(const SoundCategory& category, const float& rate) {

	if (categoryVolumeRate == nullptr)
		return;

	(*categoryVolumeRate)[category] = rate;
}

std::unordered_map<SoundCategory, float>* SoundManager::CategoryVolumeRate() {

	return categoryVolumeRate;
}

bool SoundManager::CheckLoadDate(const std::string& typeName) {

	if (soundInfoDatas == nullptr) {
		OutputDebugString("============================================================\n");
		OutputDebugString("=   !!!!! Warning !!!!!\n");
		OutputDebugString(("=   "+ typeName + "�͍Đ��ł��܂���B\n").c_str());
		OutputDebugString("=   �T�E���h�̏��������̂���������Ă��Ȃ����߁A���̂���������Ă���Đ������Ă��������B\n");
		OutputDebugString("============================================================\n");
		return false;
	}

	if (soundInfoDatas->contains(typeName) == false) {
		OutputDebugString("============================================================\n");
		OutputDebugString("=   !!!!! Warning !!!!!\n");
		OutputDebugString(("=   " + typeName + "�͍Đ��ł��܂���B\n").c_str());
		OutputDebugString("=   �ȉ��̏ꍇ���l�����܂��B\n");
		OutputDebugString("=   �f�[�^���ǂݍ��߂Ă��Ȃ��B�f�[�^�̖��O����v���Ă��Ȃ��B\n");
		OutputDebugString("============================================================\n");
		return false;
	}

	return true;
}

void SoundManager::Reload() {

	// �ǂݍ��ݗpCsv�t�@�C���̃p�X�����݂��Ȃ��ꍇ
	if (csvFilePath == nullptr)
		return;

	// �ǂݍ��񂾑S�ẴG�t�F�N�g�f�[�^���������
	AllReleaseInfo();

	// �ď���������
	Init();

	// Csv����G�t�F�N�g����ǂݍ���
	LoadToCsv(*csvFilePath);
}

void SoundManager::AllReleaseInfo() {

	// �T�E���h�f�[�^�����݂��Ă��Ȃ��ꍇ
	if (soundInfoDatas == nullptr)
		return;

	for (auto itr = soundInfoDatas->begin(); itr != soundInfoDatas->end();) {
		ResourceLoader::DeleteSoundMem((*itr).second.handle);
		itr = soundInfoDatas->erase(itr);
	}

	PtrUtil::SafeDelete(soundInfoDatas);
}

#ifdef IMGUI

#include "src/util/debug/imgui/imGuiManager.h"

void SoundManager::InitImGui() {

	if (soundInfoDatas == nullptr)
		return;

	// ���\�[�X��������Root
	ImGuiRoot* resourceTree = ImGuiManager::FindRoot("Resource");
	if (resourceTree == nullptr) {
		resourceTree = ImGuiManager::AddRoot(new ImGuiRoot("Resource"));
	}

	// �T�E���h��������Root
	ImGuiRoot* soundTree = resourceTree->AddChild(new ImGuiRoot("Sound"));

	// ���T�E���h�̃��\�[�X���Ǘ����s��
	{
		ImGuiRoot* soundResourceTree = soundTree->AddChild(new ImGuiRoot("Resource"));

		soundResourceTree->SetChild(200.0f, 150.0f);

		for (auto& itr : *soundInfoDatas) {

			// ���[�h���ꂽ�T�E���h���ɑ΂���Root�𐶐�����
			ImGuiRoot* item = soundResourceTree->AddChild(new ImGuiRoot(itr.first));

			std::function<void()> playFunc = std::bind(&Play, itr.first, "Debug");	// �Đ�����֐�
			std::function<void()> stopFunc = std::bind(&Stop, itr.first, "Debug");	// ��~����֐�

			item->Add(new ImGuiNode_SliderInt("Volume", &itr.second.curVolume, 0, 255));
			item->Add(new ImGuiNode_Button("Play", playFunc));
			item->Add(new ImGuiNode_Button("Stop", stopFunc));
		}
	}

	// ���T�E���h�̐ݒ���s��
	{
		ImGuiRoot* soundSettingTree = soundTree->AddChild(new ImGuiRoot("Setting"));

		// �J�e�S���\�ʂ̉��ʔ{����ݒ肷��
		for (int i = 0; i < static_cast<int>(SoundCategory::cMax); i++) {

			// �J�e�S���[
			SoundCategory category = static_cast<SoundCategory>(i);

			// �J�e�S���[��
			std::string categoryStr = magic_enum::enum_name(category).data();
			categoryStr.erase(categoryStr.begin());

			// ���ʔ{����ω������鏈��
			soundSettingTree->Add(new ImGuiNode_SliderFloat(categoryStr + " Volume", &(*categoryVolumeRate)[category], 0.0f, 1.0f, "%.1f"));
		}
	}
}

#endif // IMGUI