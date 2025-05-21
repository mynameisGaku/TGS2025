#pragma once

class GameObject;

/// <summary>
/// ビヘイビアツリーで使用する判定処理
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

	// ここに判定処理(引数にGameObjectを持つboolを返す関数)を列挙する
}

