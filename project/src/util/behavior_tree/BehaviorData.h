#pragma once
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

class BehaviorNode;

/// <summary>
/// シーケンスノードの情報管理を行う
/// </summary>
class BehaviorData {
public:
	BehaviorData();
	~BehaviorData();

	/// <summary>
	/// 削除
	/// </summary>
	void Clear();

	/// <summary>
	/// シーケンスノードのプッシュ
	/// </summary>
	/// <param name="node"></param>
	void PushSequenceNode(BehaviorNode* node) { stack.push(node); }

	/// <summary>
	/// シーケンスノードのポップ
	/// </summary>
	/// <returns></returns>
	BehaviorNode* PopSequenceNode();

	// シーケンスステップのゲッター
	int GetSequenceStep(std::string name);

	// シーケンスステップのセッター
	void SetSequenceStep(std::string name, const int& step) { runStep[name] = step; }

private:
	std::stack<BehaviorNode*> stack;				// シーケンスノードスタック
	std::unordered_map<std::string, int> runStep;// 実行シーケンスのステップマップ
};
