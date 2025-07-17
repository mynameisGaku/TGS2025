#pragma once

class GameObject;

namespace TASK {
	enum class STATE {
		COMPLETE, // ���s����
		RUNDING,  // ���s��
		FAILED,   // ���s���s
	};

	// �����Ɏ��s����(������GameObject������TASK::STATE��Ԃ��֐�)��񋓂���

}

/// <summary>
/// �r�w�C�r�A�c���[�Ŏg�p������s����
/// </summary>
class Task {
public:
	Task(TASK::STATE(*_action) (GameObject* obj)) : action(_action) {}
	~Task() { action = nullptr; }

	// ���s����
	inline TASK::STATE Action(GameObject* obj) {

		if (action == nullptr || obj == nullptr)
			return TASK::STATE::FAILED;

		return action(obj);
	}

	inline TASK::STATE(*GetActionFunc())(GameObject* obj) { return action; }

private:
	TASK::STATE(*action) (GameObject* obj);

};
