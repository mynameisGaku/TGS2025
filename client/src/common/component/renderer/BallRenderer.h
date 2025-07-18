#pragma once
#include "src/common/component/Component.h"
#include "framework/myDxLib.h"
#include <vector>
#include <string>

struct BallTexture
{
	int Texture = -1;
	int FrameCountAll = 0;
	int FrameCountX = 0;
};

/// <summary>
/// ‰~Œ`‚Ì‰æ‘œ‚ğó‚¯æ‚èA‹…‘Ì‚ğ•`‰æ‚·‚éƒRƒ“ƒ|[ƒlƒ“ƒg
/// </summary>
/// <author>²“¡h“l</author>
class BallRenderer : public Component
{
public:
	BallRenderer();
	~BallRenderer();

	void InitVertices();
	void SetTexture(const BallTexture& texture, const std::string& mapKey = "");
	void Update() override;
	void Draw() override;
	const BallTexture GetTexture() const;
	const std::string& GetTextureKey() const { return m_UsingTexKey; }
private:
	static const int SLICES_COUNT;	// ‹…‚Ì‰¡•ûŒü‚Ì•ªŠ„”
	static const int STACKS_COUNT;// ‹…‚Ìc•ûŒü‚Ì•ªŠ„”
	static const float RADIUS;	// ‹…‚Ì”¼Œa
	static const float FRAME_INTERVAL;
	static const float TEXTURE_RADIUS;
	
	BallTexture m_Texture{};
	int m_Frame{};
	float m_FrameTimer{};
	float m_Radius{};
	std::string m_UsingTexKey{};

	int m_SlicesCount{};
	int m_StacksCount{};

	std::vector<std::vector<VERTEX3D>> m_Vertices{};
};
