#include "src/util/behavior_tree/BehaviorData.h"

BehaviorData::BehaviorData()
{
	runStep.clear();
}

BehaviorData::~BehaviorData()
{
	Clear();
}

void BehaviorData::Clear()
{
	while (stack.size() > 0)
		stack.pop();

	runStep.clear();
}

BehaviorNode* BehaviorData::PopSequenceNode()
{
	if (stack.empty()) {
		return nullptr;
	}

	BehaviorNode* node = stack.top();
	if (node != nullptr) { // 取り出したデータを削除
		stack.pop();
	}
	return node;
}

int BehaviorData::GetSequenceStep(std::string name)
{
	if (runStep.count(name) == 0) {
		runStep[name] = 0;
	}
	return runStep[name];
}
