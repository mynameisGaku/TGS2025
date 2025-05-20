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
	NON = 0,  // �{�����Ȃ�
	PRIORITY, // �D�揇��
	SEQUENCE, // �V�[�P���X
	RANDOM,   // �����_��
	SR_MAX,   // (��{����Ȃ�)
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

	// �m�[�h����
	BehaviorNode* SearchNode(const std::string& _searchName);

	// �m�[�h���_
	BehaviorNode* Inference(GameObject* obj, BehaviorData* _data);

	// ���s
	void Run(GameObject* obj, TASK::STATE& _state);

	//priority���Ƀ\�[�g
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

	// �D�揇�ʑI��
	BehaviorNode* SelectPriority(const std::vector<BehaviorNode*>& _list);

	// �����_���I��
	BehaviorNode* SelectRandom(const std::vector<BehaviorNode*>& _list);

	// �V�[�P���X�I��
	BehaviorNode* SelectSequence(const std::vector<BehaviorNode*>& _list, BehaviorData* _data);

	// �w��ԍ��̎q�|�C���^�[�擾
	BehaviorNode* GetSpecifyChild(const int& _num);

	std::string name;					// ���g�̖��O
	SELECT_RULE selectRule;				// �I�����[��
	std::list<Decorator*> decoList;		// ����N���X
	std::list<Task*> taskList;			// ���s�N���X
	int priority;						// �D�揇��
	BehaviorNode* parent;				// �e�m�[�h
	std::list<BehaviorNode*> childList;	// �q�m�[�h
	int hierarchyNo;					// �K�w�ԍ�

};