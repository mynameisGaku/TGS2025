#pragma once
#include "src/util/ui/UI_Canvas.h"
#include "src/util/object2D/Object2D.h"

class TargetManager;

class UI_Target : public UI_Canvas {
public:
	UI_Target();
	~UI_Target();

	void Update() override;
	void Draw() override;

	void SetTargetManager(TargetManager* targetM);

private:
	TargetManager* p_TargetM;
};