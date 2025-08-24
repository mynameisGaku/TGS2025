#include "UI_HitPoint_Icon.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/ui/UI_Manager.h"
#include <src/util/file/resource_loader/resourceLoader.h>
#include "SliceBar.h"

namespace
{
	constexpr float IMAGE_SIZE_X = 64.0f;	// アイコンの横幅
	constexpr float IMAGE_SIZE_Y = 64.0f;	// アイコンの縦幅
	constexpr float BACK_SIZE_ADD_X = 30.0f;	// バックバーのサイズに追加する値
	constexpr float BACK_SIZE_ADD_Y = 0.0f;	// バックバーのサイズに追加する値
}

UI_HitPoint_Icon::UI_HitPoint_Icon() : 
	UI_HitPoint_Icon(RectTransform(), -1)
{
}

UI_HitPoint_Icon::UI_HitPoint_Icon(const RectTransform& trs, int index)
{
	m_hBarImage = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar_left_grad_edge.png");
	assert(m_hBarImage > 0);

	m_CharaIndex = index;
	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	m_BackBar = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));
	m_BackBar->SetCenter(Vector2(0.5f, 0.5f));
	m_BackBar->InitImage(m_hBarImage);
	m_BackBar->SetPriority(Priority() - 1);
	m_BackBar->Color = RGBColor(30);

	SetValue(nullptr, 0.0f, 0.0f, 0.0f);

	m_NeedRelocation = false;
}

UI_HitPoint_Icon::~UI_HitPoint_Icon()
{
	delete m_BackBar;
}

void UI_HitPoint_Icon::Update()
{
	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();
}

void UI_HitPoint_Icon::Draw()
{
	if (m_pValue == nullptr)
		return;

	RectTransform saveTrs = *rectTransform;
	const RectTransform globalTrs = rectTransform->Global();
	Vector2 offset = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	if (*m_pValue <= m_ValueMax)
	{
		RectTransform dispTrs = globalTrs;

		for (int i = 0; i < *m_pValue; i++) {
			rectTransform->position.x = saveTrs.position.x + ImageSize().x * i + 10;
			UI_Canvas::Draw();
		}
	}
	else
	{
		UI_Canvas::Draw();
		DrawFormatStringF(globalTrs.position.x + ImageSize().x + 5.0f, globalTrs.position.y - ImageSize().y * 0.5f, 0xFFFFFF, "x %0f", *m_pValue);
	}

	rectTransform->position = saveTrs.position;
}

void UI_HitPoint_Icon::SetValue(float* value, float valueMin, float valueMax, float dispNum)
{
	m_pValue = value;
	m_ValueMin = valueMin;
	m_ValueMax = valueMax;
	m_DispNum = dispNum;

	float sizeX = IMAGE_SIZE_X * m_ValueMax;
	float sizeY = IMAGE_SIZE_Y;

	m_BackBar->rectTransform->scale.x = sizeX + BACK_SIZE_ADD_X;
	m_BackBar->rectTransform->scale.y = sizeY + BACK_SIZE_ADD_Y;
	m_BackBar->rectTransform->position = Vector2(m_BackBar->rectTransform->scale.x / 2.0f, -sizeY / 2.0f);
}
