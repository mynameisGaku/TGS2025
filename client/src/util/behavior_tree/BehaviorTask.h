#pragma once

class GameObject;

namespace TASK {
	enum class STATE {
		COMPLETE, // 実行成功
		RUNDING,  // 実行中
		FAILED,   // 実行失敗
	};

	// ここに実行処理(引数にGameObjectを持つTASK::STATEを返す関数)を列挙する

}

/// <summary>
/// ビヘイビアツリーで使用する実行処理
/// </summary>
class Task {
public:
	Task(TASK::STATE(*_action) (GameObject* obj)) : action(_action) {}
	~Task() { action = nullptr; }

	// 実行処理
	inline TASK::STATE Action(GameObject* obj) {

		if (action == nullptr || obj == nullptr)
			return TASK::STATE::FAILED;

		return action(obj);
	}

	inline TASK::STATE(*GetActionFunc())(GameObject* obj) { return action; }

private:
	TASK::STATE(*action) (GameObject* obj);

};
