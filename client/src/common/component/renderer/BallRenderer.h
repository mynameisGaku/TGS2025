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
/// �~�`�̉摜���󂯎��A���̂�`�悷��R���|�[�l���g
/// </summary>
/// <author>�����h�l</author>
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
	static const int SLICES_COUNT;	// ���̉������̕�����
	static const int STACKS_COUNT;// ���̏c�����̕�����
	static const float RADIUS;	// ���̔��a
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
