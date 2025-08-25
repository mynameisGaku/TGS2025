#pragma once
#include "src/util/sound/SoundDefine.h"

/**
	@brief	音に関する情報を管理するクラス
*/
class SoundBase {
public:

	//=================================================================================
	//
	//		コンストラクタ・デストラクタ
	//
	//=================================================================================

	/**
		@brief		コンストラクタ
	*/
	SoundBase();

	/**
		@brief		デストラクタ
	*/
	~SoundBase();

	//=================================================================================
	//
	//		各種関数
	//
	//=================================================================================

	/**
		@brief		更新処理を行う
	*/
	void Update();

	/**
		@brief		音量を適応させる
	*/
	void ApplyVolume();

	/**
		@brief		フェード用の数値で音量を適応させる
	*/
	void ApplyVolumeFade();

	//=================================================================================
	//
	//		再生処理
	//
	//=================================================================================

	/**
		@brief		サウンドを再生する
		@param		info	:	再生したいサウンドの情報のポインター
		@param		label	:	判別名
	*/
	void Play(const SoundDefine::SoundInfo* info, const std::string& label);

	//=================================================================================
	//
	//		停止処理
	//
	//=================================================================================

	/**
		@brief		サウンドの再生を停止する
	*/
	void Stop();

	//=================================================================================
	//
	//		フェード処理
	//
	//=================================================================================

	/**
		@brief		サウンドをフェードインして再生する
		@param		info			:	再生したいサウンドの情報のポインター
		@param		label			:	判別名
		@param		begin			:	フェード開始値
		@param		end				:	フェード終了値
		@param		duration_sec	:	効果時間(秒)
		@param		easing			:	補間方法
	*/
	void FadeIn(const SoundDefine::SoundInfo* info, const std::string& label, int begin, int end, float duration_sec, EasingType easing = EasingType::Linear);

	/**
		@brief		サウンドをフェードアウトする
		@param		end				:	フェード終了値
		@param		duration_sec	:	効果時間(秒)
		@param		easing			:	保管方法
		@param		isFadeOutEnd	:	フェードアウト後に再生を終了するかを設定
	*/
	void FadeOut(int end, float duration_sec, EasingType easing = EasingType::Linear, bool isFadeOutEnd = true);

	//=================================================================================
	//
	//		セッター
	//
	//=================================================================================

	/**
		@brief		立体音響で使用するサウンドの再生地点を設定する
		@param		position	:	再生地点
	*/
	void SetPan(const VECTOR& position);

	/**
		@brief		立体音響で使用するサウンドの再生地点を設定する
		@param		position	:	再生地点のポインタ
	*/
	void SetPanPointer(const VECTOR* position);

	/**
		@brief		周波数を設定する
		@param		frequency	:	再生周波数(ヘルツ単位。100～100,000 : -1でデフォルトに戻す)

		@details	値が小さいほど、遅く低く、値が大きいほど速く高く再生されます。
					音の元の再生周波数が22.05KHzだった場合は【22050】を指定した場合が通常の再生値となり、
					44.10KHzだった場合は【44100】が通常の再生値となる。
	*/
	void SetFrequency(float frequency);

	//=================================================================================
	//
	//		ゲッター
	//
	//=================================================================================

	/**
		@brief		自身が持つサウンドの情報を取得する
	*/
	inline const SoundDefine::SoundInfo* GetSoundInfo() const { return soundInfo; }

	/**
		@brief		判別名を取得する
	*/
	inline const std::string Label() const { return label; }

	/**
		@brief		フェードの最中かを取得する
	*/
	inline const bool IsFade() const { return fade.info.isActive; }

	/**
		@brief		再生中かを取得する
	*/
	inline bool IsPlaying() const;

	/**
		@brief		フェードアウト後に再生を終了するかを取得する
	*/
	bool IsFadeOutEnd() const { return isFadeOutEnd; }

	//=================================================================================
	//
	//		確認用
	//
	//=================================================================================

	/**
		@brief		整合性チェックを行う
		@param		dataName	:	サウンドのデータ名
		@param		label		:	判別名
		@returns	一致していた場合、true。一致していないならば、false。
	*/
	bool CheckConsistency(const std::string& name, const std::string& label) const;

private:
	const SoundDefine::SoundInfo* soundInfo;	// サウンドの情報(実態はManagerが管理する)
	const VECTOR* playPosition;	// 再生座標
	EasingFloat fade;		// フェード情報

	std::string label;		// 判別名
	bool isFadeIn;			// フェードイン中か
	bool isFadeOut;			// フェードアウト中か

	bool isFadeOutEnd;		// フェードアウト後に再生を終了するか
	bool isSetPan;			// パン設定を行うか

};