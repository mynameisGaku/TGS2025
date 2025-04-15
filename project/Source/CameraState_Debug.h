#pragma onces

// ◇継承元
#include "StateBase.h"

// ◇汎用
#include "Util/Vector3.h"

/// <summary>
/// カメラのデバッグ中のステート
/// </summary>
class CameraState_Debug : public StateBase {
public:
	//=====================================================
	// ▼コンストラクタ・デストラクタ

	CameraState_Debug(StateManager* p);
	~CameraState_Debug();

	//=====================================================
	// ▼各種関数

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 自身のステートが動作する時に呼ぶ
	/// </summary>
	void Resume() override;

	/// <summary>
	/// 自身のステートが停止する時に呼ぶ
	/// </summary>
	void Suspend() override;

	/// <summary>
	/// 移動処理
	/// </summary>
	void MoveProcess();

private:
	bool canMove;		// 移動可能か
};