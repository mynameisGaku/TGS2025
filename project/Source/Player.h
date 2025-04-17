#pragma once
#include "CharaBase.h"

class Player : public CharaBase {
public:
	
	Player();
	~Player();

	void Update() override;
	void Draw() override;

private:

};