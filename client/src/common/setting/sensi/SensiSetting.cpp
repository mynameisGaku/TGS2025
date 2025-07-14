#include "src/common/setting/sensi/SensiSetting.h"

#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"
#include <src/util/ptr/PtrUtil.h>
#include "src/util/math/MathUtil.h"

using namespace KeyDefine;

SensiSetting::SensiSetting() {

	stickSensitivity = PadController::StickSensitivity();
	mouseSensitivity = MouseController::Info().sensitivity;

	sliders.push_back(new UI_Slider("�}�E�X���x X��", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 125.0f)), &mouseSensitivity.x, 0.1f, MOUSE_SENSI_MAX.x, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("�}�E�X���x Y��", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 200.0f)), &mouseSensitivity.y, 0.1f, MOUSE_SENSI_MAX.y, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	
	sliders.push_back(new UI_Slider("�X�e�B�b�N���x X��", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 275.0f)), &stickSensitivity.x, 0.1f, STICK_SENSI_MAX.x, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("�X�e�B�b�N���x Y��", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 350.0f)), &stickSensitivity.y, 0.1f, STICK_SENSI_MAX.y, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));

	isAdjustable = false;
}

SensiSetting::~SensiSetting() {

	for (auto& itr : sliders) {
		// UI_Manager����؂藣��
		UI_Manager::Detach(itr);

		// ���̂��폜����
		PtrUtil::SafeDelete(itr);
	}

	sliders.clear();
}

void SensiSetting::Update() {

	SettingBase::Update();

	if (isAdjustable == false)
		return;

	Apply();
}

void SensiSetting::LoadCsv(std::string filename) {

	CsvReader* csv = new CsvReader(filename);
	if (csv == nullptr) {
		SaveCsv(filename);
		return;
	}

	for (int i = 0; i < csv->Column(); i++) {
		std::string item = csv->GetString(i, Label);	// �ݒ荀�ڂ̖��O

		if (item != "���x�ݒ�")
			continue;

		mouseSensitivity.x = csv->GetFloat(i + 1, MouseSensiX);
		mouseSensitivity.y = csv->GetFloat(i + 1, MouseSensiY);

		stickSensitivity.x = csv->GetFloat(i + 1, StickSensiX);
		stickSensitivity.y = csv->GetFloat(i + 1, StickSensiY);
	}

	delete csv;
}

void SensiSetting::SaveCsv(std::string filename) {

	std::ofstream file(filename, std::ios::app);

	file << "���x�ݒ�,�}�E�X���x X��,�}�E�X���x Y��,�X�e�B�b�N���x X��,�X�e�B�b�N���x Y��\n";	// Csv�ɒ�^������������
	std::vector<std::string> data;	// �t�@�C���ɏ����o�������s���Ƃɂ܂Ƃ߂�

	data.push_back("");
	data.push_back(std::to_string(MathUtil::Rounding(mouseSensitivity.x, 1)));
	data.push_back(std::to_string(MathUtil::Rounding(mouseSensitivity.y, 1)));
	data.push_back(std::to_string(MathUtil::Rounding(stickSensitivity.x, 1)));
	data.push_back(std::to_string(MathUtil::Rounding(stickSensitivity.y, 1)));

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
	if (newLine == false)
		file << '\n';

	data.clear();	// �f�[�^���������

	file.close();	// �t�@�C�������

	OutputDebugString((filename + "�Ƀf�[�^��ۑ����܂����B\n").c_str());
}

void SensiSetting::Apply() {

	mouseSensitivity.x = MathUtil::Rounding(mouseSensitivity.x, 2);
	mouseSensitivity.y = MathUtil::Rounding(mouseSensitivity.y, 2);
	stickSensitivity.x = MathUtil::Rounding(stickSensitivity.x, 2);
	stickSensitivity.y = MathUtil::Rounding(stickSensitivity.y, 2);

	MouseController::SetMouseSensitivity(mouseSensitivity);
	PadController::SetStickSensitivity(stickSensitivity);
}
