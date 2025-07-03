#include "UI_GameOver.h"
#include "src/util/screen/ScreenManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"

UI_GameOver::UI_GameOver(const RectTransform& trs, int index)
{
	m_CharaIndex = index;

	Vector2 beginPos = ScreenManager::GetScreenBeginPos(m_CharaIndex);
	Vector2 endPos = ScreenManager::GetScreenEndPos(m_CharaIndex);

	RectTransform rectTrs = trs;
	rectTrs.anchor.SetBegin(beginPos);
	rectTrs.anchor.SetEnd(endPos);

	SetTransform(rectTrs);

	m_hWinImage = ResourceLoader::LoadGraph("data/texture/UI/GameOver/Win.png");
	m_hLoseImage = ResourceLoader::LoadGraph("data/texture/UI/GameOver/Lose.png");
	m_hDrawImage = ResourceLoader::LoadGraph("data/texture/UI/GameOver/Draw.png");
}

UI_GameOver::~UI_GameOver()
{
	if (m_hWinImage > 0)
		ResourceLoader::DeleteGraph(m_hWinImage);

	if (m_hLoseImage > 0)
		ResourceLoader::DeleteGraph(m_hLoseImage);

	if (m_hDrawImage > 0)
		ResourceLoader::DeleteGraph(m_hDrawImage);
}

void UI_GameOver::Update()
{
	Vector2 beginPos = ScreenManager::GetScreenBeginPos(m_CharaIndex);
	Vector2 endPos = ScreenManager::GetScreenEndPos(m_CharaIndex);
	rectTransform->anchor.SetBegin(beginPos);
	rectTransform->anchor.SetEnd(endPos);

	UI_Canvas::Update();
}

void UI_GameOver::Draw()
{
	UI_Canvas::Draw();

	const RectTransform globalTrs = rectTransform->Global();

	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, m_hWinImage, true);
	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, m_hLoseImage, true);
	DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, m_hDrawImage, true);
}
