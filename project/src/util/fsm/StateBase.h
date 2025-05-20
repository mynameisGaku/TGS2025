#pragma once
#include "../Library/GameObject.h"
#include "StateManager.h"

class StateBase {
public:
	StateBase(StateManager* p) : parent(p) {}
	virtual ~StateBase() {}

	virtual void Update() {}
	virtual void Draw() {}
	virtual void Resume() {}	// 自分のクラスが動作する時
	virtual void Suspend() {}	// 自分のクラスが停止する時

	template<class C>
	C* Object() const { return dynamic_cast<C*>(parent->Object()); }

protected:
	StateManager* parent;
};