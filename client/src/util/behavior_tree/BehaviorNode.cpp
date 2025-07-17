#include "src/util/behavior_tree/BehaviorNode.h"
#include "src/util/behavior_tree/BehaviorData.h"

BehaviorNode::BehaviorNode(const std::string& _name, BehaviorNode* _parent, const int& _priority, const SELECT_RULE& rule, const int& number) {

	name = _name;
	parent = _parent;
	priority = _priority;
	selectRule = rule;
	hierarchyNo = number;

	childList.clear();
	decoList.clear();
	taskList.clear();
}

BehaviorNode::~BehaviorNode() {

	for (auto c : childList)
		delete c;

	for (auto d : decoList)
		delete d;

	for (auto t : taskList)
		delete t;

	childList.clear();
	decoList.clear();
	taskList.clear();
}

void BehaviorNode::AddChild(BehaviorNode* _child) {

	//�q�������ďz�Q�ƂɂȂ�Ȃ��悤��
	if (parent == _child)
		return;

	//�������͓̂���Ȃ�
	for (BehaviorNode* c : childList) {
		if (c == _child)
			return;
	}

	childList.emplace_back(_child);
}

void BehaviorNode::SubChild(BehaviorNode* _child) {

	for (auto it = childList.begin(); it != childList.end(); it++) {
		if ((*it) == _child) {
			childList.erase(it);
			return;
		}
	}
}

void BehaviorNode::AddDecorator(Decorator* _deco) {

	if (_deco == nullptr)
		return;

	for (Decorator* d : decoList) {
		//�������̂���������ǉ����Ȃ�
		if (d->GetJudgeFunc() == _deco->GetJudgeFunc()) {
			delete _deco;
			_deco = nullptr;
			return;
		}
	}

	decoList.emplace_back(_deco);
}

void BehaviorNode::SubDecorator(bool (*_deco) (GameObject* obj)) {

	for (auto it = decoList.begin(); it != decoList.end(); it++) {
		if ((*it)->GetJudgeFunc() != _deco)
			continue;

		delete (*it);
		decoList.erase(it);
		return;
	}
}

void BehaviorNode::AddTask(Task* _task) {

	if (_task == nullptr)
		return;

	for (Task* t : taskList) {
		//�������̂���������ǉ����Ȃ�
		if (t->GetActionFunc() == _task->GetActionFunc()) {
			delete _task;
			_task = nullptr;
			return;
		}
	}

	taskList.emplace_back(_task);
}

void BehaviorNode::SubTask(TASK::STATE(*_task) (GameObject* obj)) {

	for (auto it = taskList.begin(); it != taskList.end(); it++) {
		if ((*it)->GetActionFunc() != _task)
			continue;

		delete (*it);
		taskList.erase(it);
		return;
	}
}

BehaviorNode* BehaviorNode::SearchNode(const std::string& _searchName) {

	// ���O����v
	if (name == _searchName) {
		return this;
	}
	else {
		// �q�m�[�h�Ō���
		for (auto it = childList.begin(); it != childList.end(); it++) {
			BehaviorNode* ret = (*it)->SearchNode(_searchName);
			if (ret != nullptr) {
				return ret;
			}
		}
	}

	//�������null��Ԃ�
	return nullptr;
}

BehaviorNode* BehaviorNode::Inference(GameObject* obj, BehaviorData* _data) {

	std::vector<BehaviorNode*> list;
	list.clear();
	BehaviorNode* result = nullptr;

	// �q�m�[�h�Ŏ��s�\�ȃm�[�h��T��
	for (BehaviorNode* c : childList) {
		// ����N���X���Ȃ���Ζ������ɒǉ�
		if (c->Judgment(obj)) {
			list.emplace_back(c);
		}
	}

	//���s�\�ȃm�[�h��������
	if (list.empty()) {
		return nullptr;
	}

	switch (selectRule) {
		// �D�揇��
	case SELECT_RULE::PRIORITY:
		result = SelectPriority(list);
		break;
		// �����_��
	case SELECT_RULE::RANDOM:
		result = SelectRandom(list);
		break;
		// �V�[�P���X
	case SELECT_RULE::SEQUENCE:
		result = SelectSequence(list, _data);
		break;
	}
	list.clear();

	if (result != nullptr) {
		// ���܂����m�[�h�Ő��_�J�n
		BehaviorNode* keep = result->Inference(obj, _data);
		if (keep != nullptr) {
			result = keep;
		}
	}
	return result;
}

void BehaviorNode::Run(GameObject* obj, TASK::STATE& _state) {

	// �A�N�V�����m�[�h������Ό��ʂ����s
	for (Task* t : taskList) {
		const TASK::STATE& keep = t->Action(obj);
		if ((int)_state < (int)keep)
			_state = keep;
	}
}

void BehaviorNode::PrioritySort() {

	if (childList.size() <= 1)
		return;

	childList.sort([](BehaviorNode* _a, BehaviorNode* _b) {return _a->GetPriority() > _b->GetPriority(); });
}

bool BehaviorNode::Judgment(GameObject* obj) {

	//����N���X�������Ă���Ύ��s�A�Ȃ����true����
	bool check = true;
	for (Decorator* d : decoList) {
		//��ł����s�s�Ȃ�false����
		if (d->IsJudge(obj))
			continue;

		check = false;
		break;
	}
	return check;
}

BehaviorNode* BehaviorNode::SelectPriority(const std::vector<BehaviorNode*>& _list) {

	if ((int)_list.size() <= 0)
		return nullptr;

	//�������o��
	auto it = _list.begin();
	BehaviorNode* selectNode = (*it);
	int priority = (*it)->GetPriority();

	// ��ԗD�揇�ʂ������m�[�h��T��
	for (it++; it != _list.end(); it++) {
		BehaviorNode* n = (*it);
		if (priority < n->GetPriority()) {
			selectNode = n;
			priority = n->GetPriority();
		}
	}
	return selectNode;
}

BehaviorNode* BehaviorNode::SelectRandom(const std::vector<BehaviorNode*>& _list) {

	if ((int)_list.size() <= 0)
		return nullptr;

	int select_no = rand() % (int)_list.size();
	return _list[select_no];
}

BehaviorNode* BehaviorNode::SelectSequence(const std::vector<BehaviorNode*>& _list, BehaviorData* _data) {

	if ((int)_list.size() <= 0)
		return nullptr;

	// ���̃m�[�h�̃V�[�P���X�̃X�e�b�v���擾
	int step = _data->GetSequenceStep(name);

	//step�Ԗڂ�Node���擾
	BehaviorNode* nowNode = GetSpecifyChild(step);
	if (nowNode == nullptr) {
		return nullptr;
	}

	// ���Ԃ̃m�[�h�����s�ł��邩�̃`�F�b�N
	for (BehaviorNode* node : _list) {
		if (nowNode != node) {
			continue;
		}
		// �V�[�P���X�̓r���Ȃ�X�V
		if (step < childList.size()) {
			// �V�[�P���X�m�[�h���L�^
			_data->PushSequenceNode(this);
			// �V�[�P���X�X�e�b�v���X�V
			_data->SetSequenceStep(name, step + 1);
		}
		return nowNode;
	}
	return nullptr;
}

BehaviorNode* BehaviorNode::GetSpecifyChild(const int& _num) {

	if (_num >= childList.size())
		return nullptr;

	auto it = std::next(childList.begin(), _num);
	return (*it);
}
