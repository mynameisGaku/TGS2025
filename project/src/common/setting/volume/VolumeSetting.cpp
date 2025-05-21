#include "src/common/setting/volume/VolumeSetting.h"

#include "src/util/sound/SoundManager.h"

VolumeSetting::VolumeSetting() {

	auto volumes = SoundManager::CategoryVolumeRate();
	if (volumes == nullptr)
		return;

	sliders.push_back(new UI_Slider("�S�̉���",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 125.0f)), &(*volumes)[SoundCategory::cMaster],	0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("BGM����",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 200.0f)), &(*volumes)[SoundCategory::cBGM],		0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("SE����",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 275.0f)), &(*volumes)[SoundCategory::cSE],		0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("�{�C�X����",	RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 350.0f)), &(*volumes)[SoundCategory::cVoice],	0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("������",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 425.0f)), &(*volumes)[SoundCategory::cENV],		0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));

	isAdjustable = false;
}

VolumeSetting::~VolumeSetting() {

	for (auto& itr : sliders) {
		// UI_Manager����؂藣��
		UI_Manager::Detach(itr);

		// ���̂��폜����
		Function::DeletePointer(itr);
	}

	sliders.clear();
}

void VolumeSetting::Update() {

	SettingBase::Update();

	if (isAdjustable == false)
		return;

	Apply();
}

void VolumeSetting::LoadCsv(std::string filename) {

	auto volumes = SoundManager::CategoryVolumeRate();
	if (volumes == nullptr)
		return;

	CsvReader* csv = new CsvReader(filename);
	if (csv == nullptr) {
		SaveCsv(filename);
		delete csv;
		return;
	}

	for (int i = 0; i < csv->Column(); i++) {
		std::string item = csv->GetString(i, Label);	// �ݒ荀�ڂ̖��O

		if (item != "���ʐݒ�")
			continue;

		(*volumes)[SoundCategory::cMaster]	= csv->GetFloat(i + 1, Volume_Master);
		(*volumes)[SoundCategory::cBGM]		= csv->GetFloat(i + 1, Volume_BGM);
		(*volumes)[SoundCategory::cSE]		= csv->GetFloat(i + 1, Volume_SE);
		(*volumes)[SoundCategory::cVoice]	= csv->GetFloat(i + 1, Volume_Voice);
		(*volumes)[SoundCategory::cENV]		= csv->GetFloat(i + 1, Volume_ENV);
	}

	delete csv;
}

void VolumeSetting::SaveCsv(std::string filename) {

	using namespace Function;

	auto volumes = SoundManager::CategoryVolumeRate();
	if (volumes == nullptr)
		return;

	std::ofstream file(filename, std::ios::app);

	file << "���ʐݒ�,�S�̉���,�w�i����,���ʉ���,������,������\n";	// Csv�ɒ�^������������
	std::vector<std::string> data;	// �t�@�C���ɏ����o�������s���Ƃɂ܂Ƃ߂�

	data.push_back("");
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cMaster],	1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cBGM],		1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cSE],		1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cVoice],	1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cENV],		1)));

	bool newLine = false;	// �s�̍Ō�̕����ɉ��s���t���Ă��邩

	for (auto dat = data.begin(); dat != data.end(); dat++) {
		// ���s�������A�J���}�������ꍇ�A�J���}��ǋL����
		if ((*dat).find('\n') == std::string::npos) {
			if ((*dat).find(',') == std::string::npos)
				(*dat) += ',';
		}
		// ���s������ꍇ�AnewLine��True�ɂ���
		else
			newLine = true;

		file << (*dat).c_str();	// �t�@�C���ɏ����o��
	}

	// ���s��������Ή��s����
	if (!newLine)
		file << '\n';

	data.clear();	// �f�[�^���������

	file.close();	// �t�@�C�������

	OutputDebugString((filename + "�Ƀf�[�^��ۑ����܂����B\n").c_str());
}

void VolumeSetting::Apply() {

	SoundManager::ApplyCategoryVolume();
}
