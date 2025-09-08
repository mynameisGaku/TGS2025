#include "UI_Target.h"

#include "src/common/camera/CameraManager.h"
#include "src/util/ui/UI_Manager.h"

#include "src/scene/play/targetting/TargetManager.h"

UI_Target::UI_Target()
{
	SetTargetManager(nullptr);
}

UI_Target::~UI_Target()
{
}

void UI_Target::Update()
{
	if (p_TargetM)
		p_TargetM->UpdateUI();

	p_TargetM = nullptr;
}

void UI_Target::Draw()
{
	if (p_TargetM)
		p_TargetM->DrawUI();

	p_TargetM = nullptr;
}

void UI_Target::SetTargetManager(TargetManager* targetM)
{
	p_TargetM = targetM;
}
