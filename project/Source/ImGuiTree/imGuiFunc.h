#pragma once
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiNodeが使用できる関数をまとめたnamespace
/// </summary>
namespace ImGuiFunc {

	//======================================================================
	// ▼FPS関連

	namespace FPS {

		static const char* DISP_TEXT = "FPS %.1f";	// FPS値を表示する文字列
		static const int RENDER_INTERVAL = 15;		// FPS値を更新する間隔(フレーム)
		static int fpsRenderInterval;				// FPS値を更新する間隔

		/// <summary>
		/// 引数のImGuiNode_TextにFPS値を書き写す
		/// </summary>
		/// <param name="node">書き写すImGuiNode_Textの実体</param>
		void TranscribeFPS(ImGuiNode_Base* node);

		/// <summary>
		/// FPS値を書き写すための間隔を更新する
		/// </summary>
		void Update();
	}

	//======================================================================
	// ▼Scene関連

	namespace Scene {

		/// <summary>
		/// 引数のImGuiNode_Textにシーンの種類名を書き写す
		/// </summary>
		/// <param name="node">書き写すImGuiNode_Textの実体</param>
		void TranscribeSceneName(ImGuiNode_Base* node);
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
}

#endif // IMGUI