#pragma once
#include <list>
#include <vector>
#include <string>
#include "framework/myDxLib.h"

#include "src/util/behavior_tree/BehaviorDecorator.h"
#include "src/util/behavior_tree/BehaviorTask.h"

class GameObject;
class BehaviorData;

enum class SELECT_RULE {
	NON = 0,  // 捜索しない
	PRIORITY, // 優先順位
	SEQUENCE, // シーケンス
	RANDOM,   // ランダム
	SR_MAX,   // (基本入らない)
};

class BehaviorNode {
public:
	BehaviorNode(const std::string& name, BehaviorNode* parent, const int& priority, const SELECT_RULE& rule, const int& number);
	~BehaviorNode();

	void AddChild(BehaviorNode* _child);
	void SubChild(BehaviorNode* _child);

	void AddDecorator(Decorator* _deco);
	void SubDecorator(bool (*_deco) (GameObject* obj));

	void AddTask(Task* _task);
	void SubTask(TASK::STATE(*_task) (GameObject* obj));

	// ノード検索
	BehaviorNode* SearchNode(const std::string& _searchName);

	// ノード推論
	BehaviorNode* Inference(GameObject* obj, BehaviorData* _data);

	// 実行
	void Run(GameObject* obj, TASK::STATE& _state);

	//priority順にソート
	void PrioritySort();

	const std::string& GetName() { return name; }
	const int						 GetHirerchyNo() { return hierarchyNo; }
	const SELECT_RULE& GetSelectRule() { return selectRule; }
	const std::list<Decorator*>& GetDecorators() { return decoList; }
	const bool						 HasDecorator() { return !decoList.empty(); }
	const std::list<Task*>& GetTasks() { return taskList; }
	const bool						 HasTask() { return !taskList.empty(); }
	BehaviorNode* GetParent() { return parent; }
	const std::list<BehaviorNode*>& GetChild() { return childList; }
	const bool						 HasChild() { return !childList.empty(); }
	const int						 GetPriority() { return priority; }

private:
	bool Judgment(GameObject* obj);

	// 優先順位選択
	BehaviorNode* SelectPriority(const std::vector<BehaviorNode*>& _list);

	// ランダム選択
	BehaviorNode* SelectRandom(const std::vector<BehaviorNode*>& _list);

	// シーケンス選択
	BehaviorNode* SelectSequence(const std::vector<BehaviorNode*>& _list, BehaviorData* _data);

	// 指定番号の子ポインター取得
	BehaviorNode* GetSpecifyChild(const int& _num);

	std::string name;					// 自身の名前
	SELECT_RULE selectRule;				// 選択ルール
	std::list<Decorator*> decoList;		// 判定クラス
	std::list<Task*> taskList;			// 実行クラス
	int priority;						// 優先順位
	BehaviorNode* parent;				// 親ノード
	std::list<BehaviorNode*> childList;	// 子ノード
	int hierarchyNo;					// 階層番号

};