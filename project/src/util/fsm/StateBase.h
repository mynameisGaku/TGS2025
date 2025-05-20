#pragma once
#include "../Library/GameObject.h"
#include "StateManager.h"

class StateBase {
public:
	StateBase(StateManager* p) : parent(p) {}
	virtual ~StateBase() {}

	virtual void Update() {}
	virtual void Draw() {}
	virtual void Resume() {}	// �����̃N���X�����삷�鎞
	virtual void Suspend() {}	// �����̃N���X����~���鎞

	template<class C>
	C* Object() const { return dynamic_cast<C*>(parent->Object()); }

protected:
	StateManager* parent;
};