#pragma once
#include "src/common/component/Component.h"
#include "framework/myDxLib.h"
#include <vector>
#include <string>

struct BallTexture
{
	std::vector<int> Textures;
	int FrameCountAll = 0;
	int FrameCountX = 0;
};

/// <summary>
/// 円形の画像を受け取り、球体を描画するコンポーネント
/// </summary>
/// <author>佐藤紘斗</author>
class BallRenderer : public Component
{
public:
	static const int SLICES_COUNT;	// 球の横方向の分割数
	static const int STACKS_COUNT;// 球の縦方向の分割数
	static const float RADIUS;	// 球の半径
	static const float FRAME_INTERVAL;
	static const float TEXTURE_RADIUS;

	BallRenderer();
	~BallRenderer();

	void InitVertices();
	void SetTexture(const BallTexture& texture, const std::string& mapKey = "");
	void Update() override;
	void Draw() override;
	const BallTexture GetTexture() const;
	const std::string& GetTextureKey() const { return m_UsingTexKey; }
private:
	
	BallTexture m_Texture{};
	int m_Frame{};
	float m_FrameTimer{};
	float m_Radius{};
	std::string m_UsingTexKey{};

	bool m_WasSetTransform;	// 描画前のトランスフォーム適用をやったか

	int m_SlicesCount{};
	int m_StacksCount{};

	std::vector<std::vector<VERTEX3D>> m_Vertices{};
	std::vector<VERTEX3D> m_LinearVertices{};
};
