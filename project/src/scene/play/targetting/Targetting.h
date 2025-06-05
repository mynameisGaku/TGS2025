#pragma once
#include "framework/GameObject.h"
#include <unordered_map>

class BallManager;
class CharaManager;

class Targetting : public GameObject {
public:
	Targetting();
	~Targetting();

	void Start() override;
	void Update() override;
	void Draw() override;

	int IsTargetting(int myIndex);

private:
	BallManager* ballManager;
	CharaManager* charaManager;
	std::unordered_map<int, int> targetList;	// Key: myIndex, Value: targetIndex
	int hArrow;	
};