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
	//"root���ŏ��ɓ����"
	if (_searchName == "") {
		if (root == nullptr) {
			root = new BehaviorNode(_entryName, nullptr, priority, _selectRule, 1);
			return root;
		}
	}
	else {
		//Node����
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
	//Node������Task�ǉ�
	BehaviorNode* searchNode = root->SearchNode(_entryName);
	if (searchNode == nullptr) {
		return;
	}

	Task* task = new Task(_addTask);
	searchNode->AddTask(task);
}

void BehaviorTree::AddDecorator(const std::string& _entryName, bool(*_addDecorator)(GameObject* obj))
{
	//Node������Decorator�ǉ�
	BehaviorNode* searchNode = root->SearchNode(_entryName);
	if (searchNode == nullptr) {
		return;
	}

	Decorator* deco = new Decorator(_addDecorator);
	searchNode->AddDecorator(deco);
}

void BehaviorTree::SetParent(GameObject* obj)
{
	//����Tree���g�p���Ă���GameObject������
	if (parent != nullptr) {
		assert(false);
	}
	parent = obj;
}

// ���_
void BehaviorTree::Inference(BehaviorNode* _node, BehaviorData* _data)
{
	BehaviorNode* nowNode = _node;

	if (nowNode == nullptr) {
		_data->Clear();// �f�[�^�����Z�b�g���ĊJ�n
		nowNode = root->Inference(parent, _data);// ���_
	}

	if (nowNode != nullptr) {
		//�s��
		TASK::STATE state = TASK::STATE::COMPLETE;
		nowNode->Run(parent, state);

		if (state == TASK::STATE::COMPLETE) {

			// �V�[�P���X�̓r�����𔻒f
			BehaviorNode* sequence_node = _data->PopSequenceNode();

			if (sequence_node != nullptr) { // �r���Ȃ炻������n�߂�
				nowNode = sequence_node->Inference(parent, _data);// �V�[�P���X�m�[�h����̐��_�J�n
			}
			else if (nowNode->HasChild()) { //�q�����݂���Α�����
				nowNode = nowNode->Inference(parent, _data);
			}
			else {                          // �Ō�Ȃ烊�Z�b�g
				nowNode = nullptr;
			}
		}
	}

	//���ʂ�Ԃ�
	_node = nowNode;
}
