#pragma once
#include "../Library/gameObject.h"
#include <unordered_map>

class SettingBase;

/// <summary>
/// �ݒ荀�ڂ��Ǘ�����
/// </summary>
class SettingManager : public GameObject {
public:
	//==========================================================================================
	// ���񋓌^

	/// <summary>
	/// �ݒ荀��
	/// </summary>
	enum SettingItem {
		Sensi,	// ���x�ݒ�
		Volume,	// ���ʐݒ�
		Max,
	};

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	enum class State {
		None = -1,
		Close,	// ���Ă���
		Open,	// �J���Ă���
	};

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	SettingManager();
	~SettingManager();

	//==========================================================================================
	// ���e��֐�

	void Start() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// Csv��ǂݍ���
	/// </summary>
	void LoadCsv();

	/// <summary>
	/// Csv�ɕۑ�����
	/// </summary>
	void SaveCsv();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �ݒ��ʂ̕\���E��\����ݒ肷��
	/// </summary>
	inline void SetDisplay(bool value) { isDisplay = value; }

	/// <summary>
	/// �ݒ��ʂ̕\�����\�ɂ��邩
	/// </summary>
	inline void SetCanDisplay(bool value) { canDisplay = value; }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �ݒ��ʂ��J���Ă��邩
	/// </summary>
	inline bool IsDisplay() const { isDisplay; }

private:
	void closeStateUpdate();
	void openStateUpdate();

	std::unordered_map<SettingItem, SettingBase*> setting;	// �ݒ荀��

	State state;			// �X�e�[�g
	SettingItem curItem;	// ���ݑI�𒆂̑I������

	bool canDisplay;	// �ݒ��ʂ̕\�����\�ɂ��邩
	bool isDisplay;		// �ݒ��ʂ̕\���E��\��
};
