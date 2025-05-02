#include "EffectManager.h"

#include <EffekseerForDXLib.h>
#include <unordered_map>
#include <list>

#include "../Library/csvReader.h"
#include "Util/Utils.h"
#include "settings_json.h"

namespace {

	std::unordered_map<std::string, EffectInfo>* effectInfoDatas;	// �ǂݍ��񂾃G�t�F�N�g�f�[�^��ێ�����
	std::list<EffectBase*>* effects;	// �Đ����̃G�t�F�N�g
	std::string* csvFilePath;			// �ǂݍ��ݗpCsv�t�@�C���̃p�X

	bool initialize = false;	// �������������s��ꂽ��
}

void EffectManager::Init() {

	if (effectInfoDatas == nullptr)
		effectInfoDatas = new std::unordered_map<std::string, EffectInfo>();

	if (effects == nullptr)
		effects = new std::list<EffectBase*>();

	if (csvFilePath == nullptr)
		csvFilePath = new std::string();

	initialize = false;
}

void EffectManager::Start() {

	if (initialize)
		return;

#ifdef IMGUI
		InitImGui();
#endif // IMGUI

	initialize = true;
}

void EffectManager::Update() {

	// �������������܂��s���Ă��Ȃ��ꍇ
	if (initialize == false)
		Start();

	UpdateEffekseer2D();
	UpdateEffekseer3D();

	// �Đ����̃G�t�F�N�g���Ȃ��ꍇ
	if (effects == nullptr)
		return;

	// �Đ����̃G�t�F�N�g��Update���Ăяo��
	for (auto itr = effects->begin(); itr != effects->end();) {
		if ((*itr)->IsActive() && (*itr)->IsDestroy() == false) {
			(*itr)->Update();
			itr++;
			continue;
		}

		delete* itr;
		itr = effects->erase(itr);
		if (itr == effects->end()) break;
	}
}

void EffectManager::Draw() {

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	DrawEffekseer2D();
	DrawEffekseer3D();

	// �Đ����̃G�t�F�N�g���Ȃ��ꍇ�A�����𔲂���
	if (effects == nullptr)
		return;

	// �Đ����̃G�t�F�N�g��Draw���Ăяo��
	for (const auto& itr : *effects) {
		if (itr->IsActive())
			itr->Draw();
	}
}

void EffectManager::Release() {

	// �Đ����̃G�t�F�N�g�̎��̂��폜����
	if (effects != nullptr) {

		for (auto itr = effects->begin(); itr != effects->end();) {
			if (*itr == nullptr) {
				itr++;
				continue;
			}

			delete* itr;
			itr = effects->erase(itr);
			if (itr == effects->end()) break;
		}

		effects->clear();
		Function::DeletePointer(effects);
	}

	// �ێ����Ă����ǂݍ��ݗpCsv�t�@�C���̃p�X���폜����
	Function::DeletePointer(csvFilePath);

	// �ǂݍ��񂾑S�ẴG�t�F�N�g�f�[�^���������
	AllReleaseInfo();
}

void EffectManager::Load(const EffectInfo& info) {

	// �G�t�F�N�g�f�[�^�̕ۑ����\��
	if (effectInfoDatas == nullptr)
		return;

	// �����f�[�^�����ɑ��݂��Ă���ꍇ
	if ((*effectInfoDatas).contains(info.typeName))
		return;

	// �G�t�F�N�g���
	EffectInfo newInfo = EffectInfo(info);

	// �n���h�������݂��Ȃ��ꍇ
	if (newInfo.handle < 0)
		newInfo.handle = LoadEffekseerEffect((info.fileName + info.typeName).c_str(), newInfo.defMagnification);

	// �f�[�^�̓o�^
	(*effectInfoDatas)[info.typeName] = newInfo;
}

void EffectManager::LoadToCsv(const std::string& filename) {

	// �G�t�F�N�g�f�[�^�̕ۑ����\��
	if (effectInfoDatas == nullptr)
		return;

	// Csv�f�[�^���擾����
	CsvReader* csv = new CsvReader(filename);

	// Csv�f�[�^�����݂��Ȃ��ꍇ
	if (csv == nullptr)
		return;

	if (csvFilePath == nullptr)
		csvFilePath = new std::string();

	// �ēǂݍ��ݗp��Csv�t�@�C���̃p�X��ێ�����
	*csvFilePath = filename;

	for (int i = 0; i < csv->Column(); i++) {

		std::string beginning = csv->GetString(i, 0);	// �ŏ��̍s

		// �R�����g�Ȃ玟�̗v�f��
		if (beginning.substr(0, 2) == "//")
			continue;

		// �G�t�F�N�g�̏��
		EffectInfo info;

		//==================================================
		// ���f�[�^���A�t�@�C���p�X�̐ݒ�

		info.typeName = beginning;
		info.fileName = csv->GetString(i, 1);

		//==================================================
		// ������g�嗦

		info.defMagnification = csv->GetFloat(i, 2);
		info.magnification = info.defMagnification;

		//==================================================
		// ������Đ����x

		info.defPlaySpeed = csv->GetFloat(i, 3);
		info.playSpeed = info.defPlaySpeed;

		//==================================================
		// ���ǂݍ��ݏ���

		Load(info);
	}

	delete csv;
}

void EffectManager::LoadFromJson(const std::string& filename)
{
	auto setting = Settings_json::Inst();
	setting->LoadSettingJson(filename, filename, true);

	EffectInfo desc = {};
	desc.magnification		= setting->GetOrDefault<float>(		 "Param.magnification",		0.0f,		filename);
	desc.defMagnification	= setting->GetOrDefault<float>(		 "Param.defMagnification",	0.0f,		filename);
	desc.playingHandle		= setting->GetOrDefault<float>(		 "Param.playSpeed",			0.0f,		filename);
	desc.defPlaySpeed		= setting->GetOrDefault<float>(		 "Param.defPlaySpeed",		0.0f,		filename);
	desc.isLoop				= setting->GetOrDefault<bool>(		 "Param.isLoop",			false,		filename);
	desc.fileName			= setting->GetOrDefault<std::string>("Param.fileName",			"NO INFO",	filename);
	desc.typeName			= setting->GetOrDefault<std::string>("Param.typeName",			"NO INFO",	filename);
	std::string dim			= setting->GetOrDefault<std::string>("Param.dimension",			"2D",		filename);
	if (dim == "2D")
	{
		desc.dimension = Dimensional::_2D;
	}
	else
	{
		desc.dimension = Dimensional::_3D;
	}

	Load(desc);
}

EffectBase* EffectManager::Play3D(const std::string& typeName, const Transform& trs, const std::string& label, const bool& isLoop) {

	// �G�t�F�N�g�f�[�^��Đ����G�t�F�N�g�̊Ǘ����o���Ă��Ȃ��ꍇ
	if ((effectInfoDatas != nullptr && effects != nullptr) == false)
		return nullptr;

	// ���̂𐶐�
	EffectBase* effect = new EffectBase();

	// �Đ�����
	effect->Play3D((*effectInfoDatas)[typeName], trs, label, isLoop);

	// �Đ����G�t�F�N�g�̃��X�g�ɒǉ�
	effects->push_back(effect);

	return effect;
}

EffectBase* EffectManager::Play3D_Loop(const std::string& typeName, const Transform& trs, const std::string& label) {

	EffectBase* effect = IsPlaying(typeName, label);

	if (effect != nullptr)
		effect->SetTransform3D(trs);
	else
		effect = Play3D(typeName, trs, label);

	return effect;
}

EffectBase* EffectManager::Play2D(const std::string& typeName, const RectTransform& trs, const std::string& label, const bool& isLoop) {

	// �G�t�F�N�g�f�[�^�����݂��Ă��Ȃ��������́A�Đ����G�t�F�N�g�̊Ǘ����o���Ă��Ȃ��ꍇ
	if ((effectInfoDatas != nullptr && effects != nullptr) == false)
		return nullptr;

	// ���̂𐶐�
	EffectBase* effect = new EffectBase();
	
	// �Đ�����
	effect->Play2D((*effectInfoDatas)[typeName], trs, label, isLoop);
	
	// �Đ����G�t�F�N�g�̃��X�g�ɒǉ�
	effects->push_back(effect);

	return effect;
}

EffectBase* EffectManager::Play2D_Loop(const std::string& typeName, const RectTransform& trs, const std::string& label) {

	EffectBase* effect = IsPlaying(typeName, label);

	if (effect != nullptr)
		effect->SetTransform2D(trs);
	else
		effect = Play2D(typeName, trs, label);

	return effect;
}

EffectBase* EffectManager::IsPlaying(const std::string& typeName, const std::string& label) {

	// �Đ����G�t�F�N�g�̊Ǘ����o���ĂȂ��ꍇ
	if (effects == nullptr)
		return nullptr;

	for (const auto& itr : *effects) {
		// �����̏��ƈ�v���Ă��邩�A�������`�F�b�N���s��
		if (itr->CheckConsistency(typeName, label) && itr->IsActive())
			return itr;
	}

	return nullptr;
}

void EffectManager::Stop(const std::string& typeName, const std::string& label) {

	// �Đ�����
	EffectBase* effect = IsPlaying(typeName, label);
	if (effect == nullptr)
		return;

	effect->Stop();
}

void EffectManager::StopAll() {

	// �Đ����G�t�F�N�g�̊Ǘ����o���ĂȂ��ꍇ
	if (effects == nullptr)
		return;

	for (const auto& itr : *effects)
		itr->Stop();
}

void EffectManager::Reload() {

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

void EffectManager::AllReleaseInfo() {

	// �G�t�F�N�g�f�[�^�����݂��Ă��Ȃ��ꍇ
	if (effectInfoDatas == nullptr)
		return;

	for (auto& itr : *effectInfoDatas) {
		itr.second.playingHandle = -1;
		DeleteEffekseerEffect(itr.second.handle);
	}

	Function::DeletePointer(effectInfoDatas);
}
#ifdef IMGUI

#include "ImGuiTree/imGuiManager.h"

void EffectManager::InitImGui() {

	if (effectInfoDatas == nullptr)
		return;

	// ���\�[�X��������Root
	ImGuiRoot* resourceTree = ImGuiManager::FindRoot("Resource");
	if (resourceTree == nullptr) {
		resourceTree = ImGuiManager::AddRoot(new ImGuiRoot("Resource"));
	}

	// �G�t�F�N�g��������Root
	ImGuiRoot* effectTree = resourceTree->AddChild(new ImGuiRoot("Effect"));
	effectTree->SetChild(200.0f, 150.0f);

	for (const auto& itr : *effectInfoDatas) {

		// ���[�h���ꂽ�G�t�F�N�g���ɑ΂���Root�𐶐�����
		ImGuiRoot* item = effectTree->AddChild(new ImGuiRoot(itr.first));

		// �Đ�����֐�
		std::function<void()> playFunc = std::bind(&Play2D, itr.first, RectTransform(Anchor::Preset::Middle), "Debug", false);
		
		// ��~����֐�
		std::function<void()> stopFunc = std::bind(&Stop, itr.first, "Debug");

		item->Add(new ImGuiNode_Button("Play", playFunc));
		item->Add(new ImGuiNode_Button("Stop", stopFunc));
	}
}

#endif // IMGUI