#pragma once

#include <vector>
#include <string>
#include "src/util/math/Vector3.h"

class CameraDefineRef
{
private:
	const std::string   FILEPATH = "data/Json/Camera/CameraDefine.json";// ファイルのパス
	const std::string   FILEKEY = "CameraDefine";                   // ファイル識別用キー

	static CameraDefineRef* instance;
public:
	static CameraDefineRef* Inst()
	{
		if (instance == nullptr)
		{
			instance = new CameraDefineRef;
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

	float m_Near;		// 描画処理を行う近辺の位置
	float m_Far;		// 描画処理を行う遠方の位置
	float m_Perspective;// 視野角

	float m_ConeRange;	// コーンの長さ
	float m_ConeAngle;	// コーンの角度

	float m_RotX_Max;		// X回転の最大値
	float m_RotX_Min;		// X回転の最小値
	float m_RotSpeedLimit;	// カメラの回転速度の制限

	Vector3 m_OffsetDef;	// カメラの既定相対座標
	Vector3 m_TargetDef;	// カメラの既定注視座標

	Vector3 m_OffsetChase;	// 追尾カメラの既定相対座標
	Vector3 m_TargetChase;	// 追尾カメラの既定注視座標

	Vector3 m_OffsetAim;	// エイムカメラの既定相対座標
	Vector3 m_TargetAim;	// エイムカメラの既定注視座標

	void Load(bool ForceLoad = false);

private:

	bool                m_WasLoad = false; // ロード済みか？ 重複回避用

	CameraDefineRef() :
		m_Near(0.0f),
		m_Far(0.0f),
		m_Perspective(0.0f),
		m_ConeRange(0.0f),
		m_ConeAngle(0.0f),
		m_RotX_Max(0.0f),
		m_RotX_Min(0.0f),
		m_RotSpeedLimit(0.0f),
		m_OffsetDef(Vector3::Zero),
		m_TargetDef(Vector3::Zero),
		m_OffsetChase(Vector3::Zero),
		m_TargetChase(Vector3::Zero)
	{ /*DO NOTHING*/
	}

	~CameraDefineRef()
	{ /*DO NOTHING*/
	}
};

#define CAMERADEFINE_REF (*CameraDefineRef::Inst())