#pragma once

class GameObject;

/// <summary>
/// �r�w�C�r�A�c���[�Ŏg�p���锻�菈��
/// </summary>
class Decorator {
public:
	Decorator(bool (*_judge) (GameObject* obj)) : judge(_judge) {}
	~Decorator() { judge = nullptr; }

	inline bool IsJudge(GameObject* obj) {

		if (judge == nullptr || obj == nullptr)
			return false;

		return judge(obj);
	}

	inline bool (*GetJudgeFunc())(GameObject* obj) { return judge; }

private:
	bool (*judge) (GameObject* obj);
};

namespace DECORATOR {

	// �����ɔ��菈��(������GameObject������bool��Ԃ��֐�)��񋓂���
}

