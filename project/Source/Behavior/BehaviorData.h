#pragma once
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

class BehaviorNode;

/// <summary>
/// �V�[�P���X�m�[�h�̏��Ǘ����s��
/// </summary>
class BehaviorData {
public:
	BehaviorData();
	~BehaviorData();

	/// <summary>
	/// �폜
	/// </summary>
	void Clear();

	/// <summary>
	/// �V�[�P���X�m�[�h�̃v�b�V��
	/// </summary>
	/// <param name="node"></param>
	void PushSequenceNode(BehaviorNode* node) { stack.push(node); }

	/// <summary>
	/// �V�[�P���X�m�[�h�̃|�b�v
	/// </summary>
	/// <returns></returns>
	BehaviorNode* PopSequenceNode();

	// �V�[�P���X�X�e�b�v�̃Q�b�^�[
	int GetSequenceStep(std::string name);

	// �V�[�P���X�X�e�b�v�̃Z�b�^�[
	void SetSequenceStep(std::string name, const int& step) { runStep[name] = step; }

private:
	std::stack<BehaviorNode*> stack;				// �V�[�P���X�m�[�h�X�^�b�N
	std::unordered_map<std::string, int> runStep;// ���s�V�[�P���X�̃X�e�b�v�}�b�v
};
