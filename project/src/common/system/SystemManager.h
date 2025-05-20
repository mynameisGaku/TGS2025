#pragma once
#include "../Library/gameObject.h"

class LoadScreen;

/// <summary>
/// 汎用処理の管理を行う
/// </summary>
class SystemManager : public GameObject {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	SystemManager();
	~SystemManager();

	//================================================================================
	// ▼各種関数

	void Start() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 最初に行う描画処理
	/// </summary>
	void DrawBefore();

	/// <summary>
	/// ロード処理
	/// </summary>
	void LoadUpdate();

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// ロードが終了したか
	/// </summary>
	/// <returns>終了していればtrue</returns>
	bool IsLoadEnd() const;

#ifdef _DEBUG

	//================================================================================
	// ▼デバッグ

	/// <summary>
	/// デバッグ機能を行う
	/// </summary>
	void Debug();

#endif // _DEBUG

private:

	/// <summary>
	/// ロードを行う汎用処理
	/// </summary>
	enum LoadingType {
		ltNone = -1,
		ltFont,
		ltImGuiManager,
		ltInputManager,
		ltSoundManager,
		ltEffectManager,
		ltFader,
		ltCameraManager,
		ltLightManager,
		ltShadowMap,
		ltStage,
		ltStageManager,
		ltUI_Manager,
		ltMax
	};
	LoadingType loadIndex;	// ロードを行っている汎用処理の番号

	LoadScreen* loadScreen;	// ロード画面
	bool isLoading;	// ロード中か
};

void DrawBefore();