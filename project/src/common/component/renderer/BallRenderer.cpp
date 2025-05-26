#include "BallRenderer.h"
#include "src/util/math/Vector3.h"
#include "src/util/object3D/Object3D.h"
#include "src/util/time/GameTime.h"
#include "src/util/easing/easing.h"

const int BallRenderer::SLICES_COUNT = 8;	// 球の横方向の分割数
const int BallRenderer::STACKS_COUNT = 8;	// 球の縦方向の分割数
const float BallRenderer::RADIUS = 100.0f;	// 球の半径
const float BallRenderer::FRAME_INTERVAL = 0.05f;
const float BallRenderer::TEXTURE_RADIUS = 0.85f;

BallRenderer::BallRenderer()
{
	m_Radius = 0.0f;
	m_SlicesCount = 0;
	m_StacksCount = 0;
	m_Frame = 0;
	m_FrameTimer = 0.0f;
}

BallRenderer::~BallRenderer()
{
}

void BallRenderer::InitVertices()
{
	m_SlicesCount = SLICES_COUNT;
	m_StacksCount = STACKS_COUNT;
	m_Radius = RADIUS;

	// 半径1の球を作成する
	for (size_t i = 0; i < m_SlicesCount; i++)
	{
		std::vector<VERTEX3D> stacks;

		float depth = (float)i / (m_SlicesCount - 1);	// 0..1

		float rad = asinf((depth * 2.0f - 1.0f));

		float sliceRadius = cosf(rad);	// 0..1..0
		for (size_t j = 0; j < m_StacksCount; j++)
		{
			VERTEX3D vertex;
			vertex.dif = GetColorU8(255, 255, 255, 255);
			vertex.spc = GetColorU8(0, 0, 0, 0);

			// xy は j によって円周上で変化
			// z は i によって変化
			Vector3 position;
			
			float angle = (float)j / m_StacksCount * DX_PI_F * 2.0f;	// 0..2π

			position.x = sinf(angle) * sliceRadius;	// -1..1
			position.y = cosf(angle) * sliceRadius;	// -1..1
			position.z = depth * 2.0f - 1.0f;	// -1..1

			vertex.norm = position;
			vertex.pos = position;

			float u = position.x * TEXTURE_RADIUS;
			float v = position.y * TEXTURE_RADIUS;

			u *= EasingFunc::OutCirc(1.0f - fabsf(position.z), 1.0f);
			v *= EasingFunc::OutCirc(1.0f - fabsf(position.z), 1.0f);

			vertex.u = (u + 1.0f) / 2.0f;
			vertex.v = (1.0f - (v + 1.0f) / 2.0f);

			stacks.push_back(vertex);
		}

		m_Vertices.push_back(stacks);
	}
}

void BallRenderer::SetTexture(const BallTexture& texture)
{
	m_Texture = texture;
}

void BallRenderer::Update()
{
	m_FrameTimer += GTime.deltaTime;

	if (m_FrameTimer > FRAME_INTERVAL)
	{
		m_Frame++;
		if (m_Frame >= m_Texture.FrameCountAll)
		{
			m_Frame = 0;
		}

		m_FrameTimer -= FRAME_INTERVAL;
	}
}

void BallRenderer::Draw()
{
	Transform trs = Parent<Object3D>()->transform->Global();
	
	float uAdd = (float)(m_Frame % m_Texture.FrameCountX) / m_Texture.FrameCountX;
	float vAdd = (float)(m_Frame / m_Texture.FrameCountX) / m_Texture.FrameCountX;

	MATERIALPARAM MatParam;

	MatParam.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);	// ディフューズカラーは白
	MatParam.Ambient = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);	// アンビエントカラーは白( ライトのアンビエントカラーをそのまま反映する )
	MatParam.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);	// スペキュラカラーは無し
	MatParam.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);	// エミッシブカラー( 自己発光 )もなし
	MatParam.Power = 0.0f;						// スペキュラはないので０

	// マテリアルのパラメータをセット
	SetMaterialParam(MatParam);

	for (int i = 0; i < SLICES_COUNT; i++)
	{
		for (int j = 0; j < STACKS_COUNT; j++)
		{
			VERTEX3D vertices[6];
			vertices[0] = m_Vertices[i][j];
			vertices[1] = m_Vertices[i][(j + 1) % STACKS_COUNT];
			vertices[2] = m_Vertices[(i + 1) % SLICES_COUNT][j];
			vertices[3] = vertices[1];
			vertices[4] = m_Vertices[(i + 1) % SLICES_COUNT][(j + 1) % STACKS_COUNT];
			vertices[5] = vertices[2];

			for (VERTEX3D& v : vertices)
			{
				v.u /= m_Texture.FrameCountX;
				v.u += uAdd;
				if (v.u >= 1.0f)
				{
					v.u -= 1.0f;
				}

				v.v /= m_Texture.FrameCountX;
				v.v += vAdd;
				if (v.v >= 1.0f)
				{
					v.v -= 1.0f;
				}

				v.pos *= m_Radius;
				v.pos *= trs.Matrix();
			}

			DrawPolygon3D(vertices, 2, m_Texture.Texture, FALSE);
		}
	}
}
