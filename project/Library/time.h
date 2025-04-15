#pragma once

/// <summary>
/// 時間に関する処理をまとめたnamespace
/// </summary>
namespace Time {

	/// <summary>
	/// 時間加算方法
	/// </summary>
	enum class AdditionMethod {
		None = 0,	// 加算しない
		Usual,		// 加算する(時間経過率を含まない)
		Rate		// 加算する(時間経過率を含む)
	};

	void Init();
	void Refresh();

	/// <summary>
	/// デルタタイム
	/// </summary>
	float DeltaTime();

	/// <summary>
	/// 時間経過率を設定する
	/// </summary>
	/// <param name="rate">0.0f～1.0fの間</param>
	void SetLapseRate(float rate);

	/// <summary>
	/// 時間経過率
	/// </summary>
	float LapseRate();

	/// <summary>
	///  デルタタイムに時間経過率をかけたもの
	/// </summary>
	float DeltaTimeLapseRate();

	/// <summary>
	/// ヒットストップを設定する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	void SetHitStop(int frame);

	/// <summary>
	/// 現在のヒットストップのフレーム数を取得する
	/// </summary>
	int HitStop();

	/// <summary>
	/// ヒットストップ中か
	/// </summary>
	/// <returns>ヒットストップ中ならtrue</returns>
	bool IsHitStop();

	//==================================================
	// ▼変換

	// フレームを秒に変換
	template<typename Ty = float>
	inline float FrameToSec(Ty frame) { return static_cast<float>(frame) / 60.0f; }

	// フレームを分に変換
	template<typename Ty = float>
	inline float FrameToMin(Ty frame) { return static_cast<float>(frame) / 60.0f * 60.0f; }

	// フレームを時間に変換
	template<typename Ty = float>
	inline float FrameToHour(Ty frame) { return static_cast<float>(frame) / 3600.0f * 60.0f; }

	// 秒をフレームに変換
	template<typename Ty = float>
	inline float SecToFrame(Ty sec) { return static_cast<float>(sec) * 60.0f; }

	// 分をフレームに変換
	template<typename Ty = float>
	inline float MinToFrame(Ty min) { return static_cast<float>(min) * 60.0f * 60.0f; }

	// 時間をフレームに変換
	template<typename Ty = float>
	inline float HourToFrame(Ty hour) { return static_cast<float>(hour) * 60.0f * 60.0f * 60.0f; }
};
