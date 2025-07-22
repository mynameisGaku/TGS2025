#pragma once
#include "framework/gameObject.h"
#include "src/util/pool/Pool.h"
#include "src/common/component/renderer/BallRenderer.h"
#include <list>
#include <unordered_map>

#define USE_POOL

class Ball;

/// <summary>
/// ボールの生成等マネージャー
/// </summary>
/// <author>佐藤紘斗</author>
class BallManager : public GameObject
{
public:
	BallManager();
	~BallManager();

	void Update() override;
	void Draw() override;

	Ball* CreateBall(const Vector3& position, bool isSpawn = false);

	Ball* GetBall(uint32_t index);

	int GetTrailImage(const std::string& teamColor);

	const BallTexture& GetBallTexture(std::string key);

private:
	Ball* initfunc(uint32_t index, Ball* pBall);
	void loadTextures();

	const std::string FOLDER_TEXTURE = "data/Img/BallTexture/";
	const std::string FOLDER_JSON = "data/Json/Ball/Texture/";

	int m_Model;
	std::unordered_map<std::string, BallTexture> m_Textures;
	std::unordered_map<std::string, int> m_hTrails;

#ifdef USE_POOL
	Pool<Ball>* m_pPool;
#else
	std::list<Ball*> m_Balls; // 破片リスト
#endif
};