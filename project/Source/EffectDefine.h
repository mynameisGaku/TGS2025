#pragma once
#include <string>

namespace EffectDefine {

	/// <summary>
	/// 次元
	/// </summary>
	enum class Dimensional {
		_None = -1,
		_2D,
		_3D,
		_Max,
	};

	/// <summary>
	/// エフェクトの情報
	/// </summary>
	/// <param name="handle">エフェクトのハンドル</param>
	/// <param name="playingHandle">再生用ハンドル</param>
	/// <param name="magnification">拡大率</param>
	/// <param name="defMagnification">既定拡大率</param>
	/// <param name="playSpeed">再生速度</param>
	/// <param name="defPlaySpeed">既定再生速度</param>
	/// <param name="isLoop">繰り返し表示するか?</param>
	/// <param name="fileName">データが置かれているファイルパス</param>
	/// <param name="typeName">自身の名前</param>
	/// <param name="dimension">次元</param>
	class EffectInfo {
	public:
		int handle;				// エフェクトのハンドル
		int playingHandle;		// 再生用ハンドル
		float magnification;	// 拡大率
		float defMagnification;	// 既定拡大率
		float playSpeed;		// 再生速度
		float defPlaySpeed;		// 既定再生速度
		bool isLoop;			// 繰り返し表示するか？
		std::string fileName;	// データが置かれているフォルダパス
		std::string typeName;	// 自身の名前
		Dimensional dimension;	// 次元

		EffectInfo() :
			handle(-1),
			playingHandle(-1),
			magnification(1.0f),
			defMagnification(1.0f),
			playSpeed(1.0f),
			defPlaySpeed(1.0f),
			isLoop(false),
			fileName(""),
			typeName(""),
			dimension(Dimensional::_None)
		{
		}

		~EffectInfo() {}
	};
};