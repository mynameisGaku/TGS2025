#pragma once

/// <summary>
/// ボールのJsonパラメータを読み込む
/// </summary>
/// <author>佐藤紘斗</author>
class BloomRef
{
private:
	const std::string PATH = "data/Json/Bloom/Bloom.json";
	const std::string FILEKEY = "Bloom";

	static BloomRef* instance;
public:
	struct Parameter
	{
		int DownScale;	// フィルタをかけるときの縮小度、大きいほど処理が軽い（1..8くらい）
		int MinBrightness;	// 高輝度を抜き出すときの最小明るさ（0..255）
		int GaussianRate;	// ガウスぼかしの強さ（0..1000くらい）
		int AddRate;	// 加算する強さ（0..255）
	};

	static BloomRef* Inst()
	{
		if (instance == nullptr)
		{
			instance = new BloomRef;
		}
		return instance;
	}

	// " リファレンス解放 "
	// ↑を文字列検索したら着くところで解放してます。(おそらくメインループのすぐあと)
	void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
		instance = nullptr;
	}

	void Load();

	Parameter Param;	// ブルームのパラメータ
};

#define BLOOM_REF (*BloomRef::Inst())

// Json読み込み用
#include <vendor/nlohmann/json_fwd.hpp>

void to_json(nlohmann::json& j, const BloomRef::Parameter& param);
void from_json(const nlohmann::json& j, BloomRef::Parameter& param);

