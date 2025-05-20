//#pragma once
//#include "../Library/gameObject.h"
//#include "../Source/Util/Utils.h"
//#include "../Source/Util/Vector3.h"
//#include "../Source/Util/Vector2.h"
//#include "gauge.h"
//
//#include <math.h>
//#include <algorithm>
//#include <list>
//#include <string>
//
////画像の輝度に関する変数をまとめたもの。関数追加可能
//
///// <summary>
///// UIに使用する画像の輝度を表すクラス
///// </summary>
///// <author> G.F </author>
//class UI_Bright {
//public:
//	inline UI_Bright(float r = 255, float g = 255, float b = 255, float a = 255) {
//		red = r;
//		green = g;
//		blue = b;
//		alpha = a;
//	}
//
//	float red;
//	float green;
//	float blue;
//	float alpha;
//};
//
///// <summary>
///// UIに使用する画像データクラス
///// </summary>
///// <author> G.F </author>
//class UI_Graph {
//public:
//	inline UI_Graph() {
//		handle = -1;
//		size_x = 0;
//		size_y = 0;
//		cell_x = 0;
//		cell_y = 0;
//		center_x = 0;
//		center_y = 0;
//		alpha = 255;
//		ext = 1;
//		angle = 0;
//		bright = UI_Bright();
//		pos = V2::ZERO;
//	}
//	int handle;
//	int size_x;
//	int size_y;
//	int cell_x;
//	int cell_y;
//	float center_x;
//	float center_y;
//	float alpha;
//	float ext;
//	float angle;
//	UI_Bright bright;
//	Vector2 pos;
//};
//
///// <summary>
///// ビルボードゲージ用のパラメータクラス
///// </summary>
///// <author> G.F </author>
//class BillGaugeParam {
//public:
//	float x1, y1, x2, y2, x3, y3, x4, y4;
//	float u1, v1, u2, v2, u3, v3, u4, v4;
//
//	inline BillGaugeParam(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
//		float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
//	{
//		this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2; this->x3 = x3; this->y3 = y3; this->x4 = x4; this->y4 = y4;
//		this->u1 = u1; this->v1 = v1; this->u2 = u2; this->v2 = v2; this->u3 = u3; this->v3 = v3; this->u4 = u4; this->v4 = v4;
//	}
//
//	inline void SetUV(float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
//	{
//		this->u1 = u1; this->v1 = v1; this->u2 = u2; this->v2 = v2; this->u3 = u3; this->v3 = v3; this->u4 = u4; this->v4 = v4;
//	}
//	
//	inline void SetPos(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
//	{
//		this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2; this->x3 = x3; this->y3 = y3; this->x4 = x4; this->y4 = y4;
//	}
//
//};
//namespace
//{
//	const int VERT_INDEX = 4;
//}
//
///// <summary>
///// ビルボードによるゲージ描画クラス
///// </summary>
///// <author> G.F </author>
//class BillGauge
//{
//public:
//	BillGauge()
//	{
//		for (int i = 0; i < VERT_INDEX; i++)
//		{
//			m_vert[i].pos = Vector3::Zero;				//左下 0, 1
//			m_vert[i].norm = Vector3::Zero;
//			m_vert[i].dif = GetColorU8(255, 255, 255, 255);
//			m_vert[i].spc = GetColorU8(255, 255, 255, 255);
//			m_vert[i].u = 0.0f;
//			m_vert[i].v = 0.0f;
//			m_vert[i].sv = 0.0f;
//			m_vert[i].su = 0.0f;
//		}
//		m_polygonNum = 2;
//		m_graphHandle = -1;
//		m_pos = Vector3::Zero;
//		m_pRawVertexNorm = VGet(0, 0, 1);
//	}
//	VERTEX3D m_vert[VERT_INDEX];             //ビルボード描画用頂点座標  このクラスはゲージの為のクラスなので基本頂点は4個。
//	int m_polygonNum;             //ポリゴン数   基本２枚
//	int m_graphHandle;            //ビルボードに貼る画像
//	Vector3 m_pos;               //ビルボードの座標
//	Vector3 m_pRawVertexNorm;   //オリジナルの法線情報を保持する
//};
//
///// <summary>
///// UIのベースとなるクラス。
///// </summary>
///// <author> G.F </author>
//class UI_Base
//{
//public:
//	UI_Base() { Initialize(); }
//	virtual ~UI_Base() {}
//
//	virtual void Start() = 0;
//	virtual void Update() = 0;
//	virtual void Draw() = 0;
//
//	/// <summary>
//	/// 全てのUI派生クラスに共通する初期化関数。
//	/// これを走らせとけばとりあえず初期化できる。
//	/// </summary>
//	void Initialize();
//
//	/// <summary>
//	/// シーンのポインタをセットする
//	/// </summary>	
//	inline void SetSceneInfo(SceneBase* _scene) { scene = _scene; }
//
//protected:
//
//	//シーンポインタ
//	SceneBase* scene;
//
//	//画像データ
//	UI_Graph graph;
//
//	//ビルボードデータ
//	BillGauge billgauge;
//
//	//ゲージ
//	Gauge gauge;
//
//	//揺れが予約されているか？
//	bool isShake;
//	//拡縮が予約されているか？
//	bool isBound;
//
//	/*---デバッグ---*/
//	bool isShow;	//表示する？
//
//	//桁数取得
//	int GetDigitCount(int number)
//	{
//		if (number == 0) return 1;
//		int digits = 0;
//		while (number != 0) {
//			number /= 10;
//			digits++;
//		}
//		return digits;
//	}
//
//
//public:
//
//	////////////////////////////////////////////////////////////////////////////////////////////
//	/*			ゲージ描画			*/
//
//	// 横棒ゲージを描画
//	// Pos: ゲージの位置を示す座標
//	// cx: ゲージの中心座標のX値 (0.0f～1.0f) 回転の原点
//	// cy: ゲージの中心座標のY値 (0.0f～1.0f) 回転の原点
//	// Size: ゲージのサイズ
//	// MaxValue: ゲージの最大値
//	// CurrentValue: 現在の値
//	// Angle: ゲージの傾き（ラジアン単位）
//	// GrHandle: 描画する画像のハンドル
//	// TransFlag: 画像の透明度を有効にするかどうか（TRUE: 有効, FALSE: 無効）
//	// ReverseFlagX: 画像を左右反転するかどうか（true: 有効, false: 無効）
//	// ReverseFlagY: 画像を上下反転するかどうか（true: 有効, false: 無効）
//	void DrawHorizontalGauge(Vector2 Pos, float cx, float cy, float Size, float MaxValue, float CurrentValue, float Angle, int GrHandle, int TransFlag, bool ReverseFlagX = false, bool ReverseFlagY = false);
//
//	// 平行四辺形のゲージを描画 (画像非対応)
//	// Pos: ゲージの位置を示す座標
//	// cx: ゲージの中心座標のX値 (0.0f～1.0f)
//	// cy: ゲージの中心座標のY値 (0.0f～1.0f)
//	// Size: ゲージのサイズ
//	// MaxValue: ゲージの最大値
//	// CurrentValue: 現在の値
//	// Angle: ゲージの傾き（ラジアン単位）
//	// Color: ゲージの色
//	// FillFlag: ゲージを塗りつぶすかどうか（TRUE: 塗りつぶす, FALSE: 枠線のみ）
//	// ReverseFlagX: ゲージを左右反転するかどうか（true: 有効, false: 無効）
//	// ReverseFlagY: ゲージを上下反転するかどうか（true: 有効, false: 無効）
//	void DrawParallelGauge(Vector2 Pos, float cx, float cy, float Width, float Height, float MaxValue, float CurrentValue, float Angle, int Color, int FillFlag, bool ReverseFlagX = false, bool ReverseFlagY = false);
//
//	/// <summary>
//	/// ビルボードでゲージを描画する
//	/// </summary>
//	/// <param name="_pos">描画先の座標</param>
//	/// <param name="left_down">左下の頂点座標</param>
//	/// <param name="left_up">左上の頂点座標</param>
//	/// <param name="right_down">右下の頂点座標</param>
//	/// <param name="right_up">右上の頂点座標</param>
//	/// <param name="uv_pos_left">正面から見たビルボードの左側のUV座標</param>
//	/// <param name="uv_pos_right">正面から見たビルボードの右側のUV座標</param>
//	/// <param name="current">ゲージに適用させる値</param>
//	/// <param name="graph">画像ハンドル	-1で画像を使用しない</param>
//	/// <param name="transflag">透過させるか？ </param>
//	void DrawBillGauge3D(Vector3 _pos, BillGaugeParam param, float current, float currentMax, int graph, bool transflag);
//
//	/*		画像に関する関数		*/
//
//	/*----------	セッター	----------*/
//	// 使用する画像をセットする(ついでに画像サイズと画像の中央座標を自動で格納)
//	const void SetGraph(int _graph) { graph.handle = _graph; GetGraphSize(graph.handle, &graph.size_x, &graph.size_y); graph.center_x = (float)graph.size_x / 2; graph.center_y = (float)graph.size_y / 2; }
//	// 使用する画像とその分割数(x,y)を指定する。
//	const void SetGraph(int _graph, int cell_x, int cell_y) { graph.handle = _graph; graph.cell_x = cell_x; graph.cell_y = cell_y; }
//	// 座標を設定
//	const void SetPosition(Vector2 pos) { graph.pos = pos; }
//	// 拡大率を設定
//	const void SetExtendRate(float ext) { graph.ext = ext; }
//	// 回転を設定
//	const void SetAngle(float angle) { graph.angle = angle; }
//	// 輝度を設定
//	const void SetBright(float r, float g, float b) { graph.bright.red = r, graph.bright.green = g, graph.bright.blue = b; }
//
//	/*-----------	ゲッター	-----------*/
//	// 画像を取得
//	const int GetGraph() const { return graph.handle; }
//	// 画像X軸の大きさを取得
//	const int GetGraphSize_X() const { return graph.size_x; }
//	// 画像Y軸の大きさを取得
//	const int GetGraphSize_Y() const { return graph.size_y; }
//	// 画像X軸の大きさを取得(拡大率適用)
//	const int GetGraphSize_X_EXT() const { return (int)(graph.size_x * graph.ext); }
//	// 画像Y軸の大きさを取得(拡大率適用)
//	const int GetGraphSize_Y_EXT() const { return (int)(graph.size_y * graph.ext); }
//	// X軸セル1マスの大きさを取得(ピクセル)
//	const int GetCellSize_X() const { return graph.size_x / graph.cell_x; }
//	// Y軸セル1マスの大きさを取得(ピクセル)
//	const int GetCellSize_Y() const { return graph.size_y / graph.cell_y; }
//	// 画像の回転を取得
//	const float GetAngle() const { return graph.angle; }
//	// 画像の拡大率を取得
//	const float GetExtend() const { return graph.ext; }
//	// 画像の座標を取得
//	const Vector2 GetPosision() const { return graph.pos; }
//	// 画像の輝度を取得
//	const UI_Bright GetBright() const { return graph.bright; }
//
//	/*---デバッグ---*/
//	void SetShowFlag(bool flag) { isShow = flag; }
//};