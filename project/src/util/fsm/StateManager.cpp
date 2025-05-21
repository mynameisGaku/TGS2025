#include "src/util/fsm/StateManager.h"
#include "src/util/fsm/StateBase.h"

StateManager::StateManager(GameObject* parent) {

	this->parent = parent;
	currentStateID = -1;
	prevStateID = -1;
	nextStateID = -1;
}

StateManager::~StateManager() {

	for (auto& inst : stateInstances)
		delete inst.second;

	stateInstances.clear();
}

void StateManager::Update()
{
	if (currentStateID != nextStateID) {
		if (stateInstances.count(currentStateID) > 0) {
			prevStateID = currentStateID;
			stateInstances[currentStateID]->Suspend();
		}

		currentStateID = nextStateID;

		if (stateInstances.count(currentStateID) > 0)
			stateInstances[currentStateID]->Resume();
	}

	if (currentStateID >= 0 && stateInstances.count(currentStateID) > 0)
		stateInstances[currentStateID]->Update();
}

void StateManager::Draw() {

	if (stateInstances.count(currentStateID) > 0)
		stateInstances[currentStateID]->Draw();
}

void StateManager::AddInstance(int id, StateBase* inst) {

	stateInstances[id] = inst;
}

void StateManager::ChangeState(int newState) {

	nextStateID = newState;
}
