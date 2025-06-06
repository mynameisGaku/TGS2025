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
	void DrawBallPosMarker(const Vector3& ballPos, int targetCharaID);
	void DrawWarning();
	void DrawThorn(const Vector3& ballPos, int targetCharaID);

	BallManager* ballManager;
	CharaManager* charaManager;
	std::unordered_map<int, int> targetList;	// Key: myIndex, Value: targetIndex
	int hArrow;
};