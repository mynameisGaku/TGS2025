#pragma once
#include "BehaviorData.h"
#include "BehaviorNode.h"

class BehaviorTree {
public:
	BehaviorTree() : parent(nullptr), root(nullptr) {}
	~BehaviorTree();

	// ���s�m�[�h�𐄘_����
	void Inference(BehaviorNode* _node, BehaviorData* _data);

	/// <summary>
	///  Node�̒ǉ�
	/// </summary>
	/// <param name="search_name">�e�̖��O</param>
	/// <param name="entry_name"> �ǉ�����Node�̖��O</param>
	/// <param name="priority">�D��x</param>
	/// <param name="select_rule">���_�`��</param>
	/// <returns>�ǉ�����Node�̃|�C���^�[</returns>
	BehaviorNode* AddNode(const std::string& _searchName, const std::string& _entryName, const int& priority, const SELECT_RULE& _selectRule);

	//�s���ǉ�
	void AddTask(const std::string& _entryName, TASK::STATE(*_addTask)(GameObject* obj));

	//�����ǉ�
	void AddDecorator(const std::string& _entryName, bool(*_addDecorator)(GameObject* obj));

	//�e�ǉ�
	void SetParent(GameObject* _set);

	//���[�g�m�[�h���擾
	BehaviorNode* GetRoot() { return root; }

	//���O(�ۑ����鎞�Ɏg��)
	const std::string& GetName() { return treeName; }
	void SetName(const std::string& _name) { treeName = _name; }
private:
	//�|�C���^�[
	//�N�����̃c���[�������Ă��邩
	GameObject* parent;

	// ���[�g�m�[�h
	//Node�ł��蓪
	BehaviorNode* root;

	//Tree�̖��O
	//�ۑ����鎞�Ɏg��,�Ȃ��Ă���
	std::string treeName;
};
