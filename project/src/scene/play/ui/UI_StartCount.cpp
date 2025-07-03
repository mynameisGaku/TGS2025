#include "UI_StartCount.h"
#include "src/reference/game/GameRef.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"

UI_StartCount::UI_StartCount() : UI_StartCount(RectTransform(), 0)
{
}

UI_StartCount::UI_StartCount(const RectTransform& trs, int index)
{
	m_pMatchManager = nullptr;
	m_CharaIndex = index;
	m_Count = 0;
	m_Scale = 5.0f;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);
}

UI_StartCount::~UI_StartCount()
{
}

void UI_StartCount::Update()
{
	// ��ʕ������؂�ւ����ɃA���J�[�̈ʒu���X�V(�f�o�b�O�p)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	if (m_pMatchManager == nullptr) {
		m_pMatchManager = FindGameObject<MatchManager>();

		if (m_pMatchManager == nullptr)
			return;
	}

	float timer = m_pMatchManager->GetReadyTimerSec();

	m_Count = (int)ceilf(timer);

	UI_Canvas::Update();
}

void UI_StartCount::Draw()
{
	UI_Canvas::Draw();

	std::string text;

	if (m_Count > 0)
	{
		text = StringUtil::FormatToString("%d", m_Count).c_str();
	}
	else
	{
		text = "�s���I";
	}

	int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * m_Scale);

	const Vector2 adjust = Vector2(0, 0);
	const RectTransform globalTrs = rectTransform->Global();

	DrawExtendFormatString(
		globalTrs.position.x - width * 0.5f,	// ��������
		globalTrs.position.y - adjust.y * 0.5f,
		m_Scale,
		m_Scale,
		GetColor(255, 255, 255),
		text.c_str());
}

bool UI_StartCount::isStart() const
{
	return m_Count <= 0;
}
