#pragma once
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiNode���g�p�ł���֐����܂Ƃ߂�namespace
/// </summary>
namespace ImGuiFunc {

	//======================================================================
	// ��FPS�֘A

	namespace FPS {

		static const char* DISP_TEXT = "FPS %.1f";	// FPS�l��\�����镶����
		static const int RENDER_INTERVAL = 15;		// FPS�l���X�V����Ԋu(�t���[��)
		static int fpsRenderInterval;				// FPS�l���X�V����Ԋu

		/// <summary>
		/// ������ImGuiNode_Text��FPS�l�������ʂ�
		/// </summary>
		/// <param name="node">�����ʂ�ImGuiNode_Text�̎���</param>
		void TranscribeFPS(ImGuiNode_Base* node);

		/// <summary>
		/// FPS�l�������ʂ����߂̊Ԋu���X�V����
		/// </summary>
		void Update();
	}

	//======================================================================
	// ��Scene�֘A

	namespace Scene {

		/// <summary>
		/// ������ImGuiNode_Text�ɃV�[���̎�ޖ��������ʂ�
		/// </summary>
		/// <param name="node">�����ʂ�ImGuiNode_Text�̎���</param>
		void TranscribeSceneName(ImGuiNode_Base* node);
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
}

#endif // IMGUI