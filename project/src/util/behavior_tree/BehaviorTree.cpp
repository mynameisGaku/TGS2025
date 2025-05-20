#include "src/util/behavior_tree/BehaviorTree.h"
#include <assert.h>
#include <fstream>

BehaviorTree::~BehaviorTree()
{
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

BehaviorNode* BehaviorTree::AddNode(const std::string& _searchName, const std::string& _entryName, const int& priority, const SELECT_RULE& _selectRule)
{
	//"rootを最初に入れる"
	if (_searchName == "") {
		if (root == nullptr) {
			root = new BehaviorNode(_entryName, nullptr, priority, _selectRule, 1);
			return root;
		}
	}
	else {
		//Node検索
		BehaviorNode* search_node = root->SearchNode(_searchName);

		if (search_node != nullptr) {
			BehaviorNode* add_node = new BehaviorNode(_entryName, search_node, priority, _selectRule, search_node->GetHirerchyNo() + 1);
			search_node->AddChild(add_node);
			search_node->PrioritySort();
			return add_node;
		}
	}
	return nullptr;
}

void BehaviorTree::AddTask(const std::string& _entryName, TASK::STATE(*_addTask)(GameObject* obj))
{
	//Node検索後Task追加
	BehaviorNode* searchNode = root->SearchNode(_entryName);
	if (searchNode == nullptr) {
		return;
	}

	Task* task = new Task(_addTask);
	searchNode->AddTask(task);
}

void BehaviorTree::AddDecorator(const std::string& _entryName, bool(*_addDecorator)(GameObject* obj))
{
	//Node検索後Decorator追加
	BehaviorNode* searchNode = root->SearchNode(_entryName);
	if (searchNode == nullptr) {
		return;
	}

	Decorator* deco = new Decorator(_addDecorator);
	searchNode->AddDecorator(deco);
}

void BehaviorTree::SetParent(GameObject* obj)
{
	//このTreeを使用しているGameObjectを持つ
	if (parent != nullptr) {
		assert(false);
	}
	parent = obj;
}

// 推論
void BehaviorTree::Inference(BehaviorNode* _node, BehaviorData* _data)
{
	BehaviorNode* nowNode = _node;

	if (nowNode == nullptr) {
		_data->Clear();// データをリセットして開始
		nowNode = root->Inference(parent, _data);// 推論
	}

	if (nowNode != nullptr) {
		//行動
		TASK::STATE state = TASK::STATE::COMPLETE;
		nowNode->Run(parent, state);

		if (state == TASK::STATE::COMPLETE) {

			// シーケンスの途中かを判断
			BehaviorNode* sequence_node = _data->PopSequenceNode();

			if (sequence_node != nullptr) { // 途中ならそこから始める
				nowNode = sequence_node->Inference(parent, _data);// シーケンスノードからの推論開始
			}
			else if (nowNode->HasChild()) { //子が存在すれば続ける
				nowNode = nowNode->Inference(parent, _data);
			}
			else {                          // 最後ならリセット
				nowNode = nullptr;
			}
		}
	}

	//結果を返す
	_node = nowNode;
}
