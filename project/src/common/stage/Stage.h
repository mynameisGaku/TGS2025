#pragma once
#include "src/common/stage/stageDefine.h"
#include "src/util/object3D/Object3D.h"

/// <summary>
/// ステージのクラスを管理する(地形・空・壁)
/// </summary>
namespace Stage {

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 地面モデル(ポリゴン)と線分との当たり判定をする
	/// </summary>
	/// <param name="begin">線分の始点</param>
	/// <param name="end">線分の終点</param>
	/// <param name="hitPos">当たった場合に座標を返すポインター</param>
	/// <returns>当たったら True</returns>
	bool ColCheckGround(Vector3 begin, Vector3 end, Vector3* hitPos = nullptr);

	/// <summary>
	/// 壁モデル(カプセル)との線分との当たり判定
	/// </summary>
	/// <param name="begin">カプセルの始点</param>
	/// <param name="end">カプセルの終点</param>
	/// <param name="r">カプセルの半径</param>
	/// <param name="push">押し出すベクトル</param>
	/// <returns>当たっていたらtrue</returns>
	bool ColCheckWall(Vector3 begin, Vector3 end, float r, Vector3* push = nullptr);

	/// <summary>
	/// 地面のクラス
	/// </summary>
	Object3D* Ground();

	/// <summary>
	/// 空のクラス
	/// </summary>
	Object3D* Sky();

	/// <summary>
	/// 壁のクラス
	/// </summary>
	Object3D* Wall();
}
