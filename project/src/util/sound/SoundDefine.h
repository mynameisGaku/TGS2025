#pragma once
#include <string>
#include "EasingUtils.h"

namespace SoundDefine {

	/// <summary>
	/// 音源の分類
	/// </summary>
	enum class SoundCategory {
		cNone = -1,
		cMaster,
		cBGM,
		cSE,
		cVoice,
		cENV,
		cMax
	};

	/// <summary>
	/// 音に関する情報
	/// </summary>
	/// <param name="handle">サウンドのハンドル</param>
	/// <param name="curVolume">現在の音量</param>
	/// <param name="defVolume">既定の音量</param>
	/// <param name="playType">再生形式(DX_PLAYTYPE_～～)</param>
	/// <param name="category">音の分類</param>
	/// <param name="fileName">ファイルパス</param>
	/// <param name="typeName">自身の名前</param>
	class SoundInfo {
	public:
		int handle;		// サウンドのハンドル
		int curVolume;	// 現在の音量
		int defVolume;	// 既定の音量
		int playType;	// 再生形式(DX_PLAYTYPE_～～)
		SoundCategory category;	// 音の分類
		std::string fileName;	// ファイルパス
		std::string typeName;	// 自身の名前

		SoundInfo() :
			handle(-1),
			curVolume(0),
			defVolume(0),
			playType(DX_PLAYTYPE_BACK),
			category(SoundCategory::cNone),
			fileName(""),
			typeName("")
		{
		}

		~SoundInfo() {
			fileName.clear();
			typeName.clear();
		}
	};

	typedef EasingFloat SoundFade;
}