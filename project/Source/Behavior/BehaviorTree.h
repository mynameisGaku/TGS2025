#pragma once
#include "BehaviorData.h"
#include "BehaviorNode.h"

class BehaviorTree {
public:
	BehaviorTree() : parent(nullptr), root(nullptr) {}
	~BehaviorTree();

	// 実行ノードを推論する
	void Inference(BehaviorNode* _node, BehaviorData* _data);

	/// <summary>
	///  Nodeの追加
	/// </summary>
	/// <param name="search_name">親の名前</param>
	/// <param name="entry_name"> 追加するNodeの名前</param>
	/// <param name="priority">優先度</param>
	/// <param name="select_rule">推論形式</param>
	/// <returns>追加したNodeのポインター</returns>
	BehaviorNode* AddNode(const std::string& _searchName, const std::string& _entryName, const int& priority, const SELECT_RULE& _selectRule);

	//行動追加
	void AddTask(const std::string& _entryName, TASK::STATE(*_addTask)(GameObject* obj));

	//条件追加
	void AddDecorator(const std::string& _entryName, bool(*_addDecorator)(GameObject* obj));

	//親追加
	void SetParent(GameObject* _set);

	//ルートノードを取得
	BehaviorNode* GetRoot() { return root; }

	//名前(保存する時に使う)
	const std::string& GetName() { return treeName; }
	void SetName(const std::string& _name) { treeName = _name; }
private:
	//ポインター
	//誰がこのツリーを持っているか
	GameObject* parent;

	// ルートノード
	//Nodeであり頭
	BehaviorNode* root;

	//Treeの名前
	//保存する時に使う,なくても可
	std::string treeName;
};
