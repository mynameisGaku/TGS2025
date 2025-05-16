#include "AttachedAnimation.h"
#include "Library/myDxLib.h"
#include "Library/time.h"
#include <assert.h>

namespace
{
	//ToDo:アニメーターからもらうようにする
	static const std::string ROOT_NAME = "mixamorig9:Hips";
	static const int ANIM_FRAMERATE = 30;
}

AttachedAnimation::AttachedAnimation(int parentModel, const AnimInfo& info)
{
	assert(parentModel >= 0);

	m_info = info;

	m_parentModel = parentModel;	// モデルハンドル
	m_attachID = -1;	// アタッチ済アニメーションのハンドル

	m_frame = 0.0f;	// 現在フレーム（小数もある）
	m_maxFrame = MV1GetAnimTotalTime(info.handle, 0);	// 最大フレーム
	m_playSpeed = 1.0f;	// 再生速度倍率
	m_blendRate = 0.0f;	// ブレンド率
	m_defaultBlendRate = 1.0f;	// 初期ブレンド率

	m_first = true;	// 最初のアップデート時true
}

AttachedAnimation::~AttachedAnimation()
{
	if (m_attachID >= 0)
		MV1DetachAnim(m_parentModel, m_attachID);
}

void AttachedAnimation::updateFrame()
{
	// 時間を進める
	m_frame += Time::DeltaTimeLapseRate() * m_playSpeed * ANIM_FRAMERATE;

	// アニメーション終了時間を超えたら
	if (m_frame >= m_maxFrame) {
		// ループなら戻す
		if (m_info.option.isLoop) {
			m_frame -= m_maxFrame;
		}
		// ループじゃなければ最大値で止める
		else {
			m_frame = m_maxFrame;
		}
	}
	// 逆再生時のループ処理
	else if (m_frame < 0) {
		// ループなら戻す
		if (m_info.option.isLoop) {
			m_frame += m_maxFrame;
		}
		// ループじゃなければ最小値で止める
		else {
			m_frame = 0;
		}
	}
}

void AttachedAnimation::RefreshDefaultBlendRate()
{
	m_defaultBlendRate = BlendRate();
}

//===============================================================================

AttachedAnimation_Main::AttachedAnimation_Main(int parentModel, const AnimInfo& info) : AttachedAnimation(parentModel, info)
{
	m_hRoot = MV1SearchFrame(m_parentModel, ROOT_NAME.c_str());	// モデルのルートフレームのハンドル
	m_rootMatrix = MGetIdent();	// ルートフレーム移動用の行列
}

void AttachedAnimation_Main::Update()
{
	if (m_first)
	{
		m_attachID = MV1AttachAnim(m_parentModel, 0, m_info.handle);

		m_first = false;
	}

	// 時間を進める
	updateFrame();

	// 再生時間を適用
	MV1SetAttachAnimTime(m_parentModel, m_attachID, m_frame);
	MV1SetAttachAnimBlendRate(m_parentModel, m_attachID, m_blendRate * m_defaultBlendRate);
}

void AttachedAnimation_Main::UpdateRootMatrix()
{
	// ルートの移動行列を取得
	m_rootMatrix = MV1GetFrameLocalMatrix(m_parentModel, m_hRoot);

	// 無補正時の座標を取得
	const Vector3 framePos = MV1GetAttachAnimFrameLocalPosition(m_parentModel, m_attachID, m_hRoot);
	// 座標移動を打ち消す
	m_rootMatrix *= MGetTranslate(framePos * -1.0f);
	Vector3 matPos = framePos;

	// 固定ルートなら0にする
	for (int i = 0; i < 3; i++)
	{
		if (m_info.option.isFixedRoot[i])
		{
			matPos.Set(i, 0);
		}
	}
	// オフセットを移動量に足して適用
	matPos += m_info.option.offset;
	m_rootMatrix *= MGetTranslate(matPos);
}

//===============================================================================

AttachedAnimation_Sub::AttachedAnimation_Sub(int parentModel, const AnimInfo& info, std::string target) : AttachedAnimation(parentModel, info)
{
	m_targetID = MV1SearchFrame(parentModel, target.c_str());
}

void AttachedAnimation_Sub::Update()
{
	if (m_first)
	{
		m_attachID = MV1AttachAnim(m_parentModel, 0, m_info.handle);
		MV1SetAttachAnimBlendRate(m_parentModel, m_attachID, 0.0f);

		m_first = false;
	}

	// 時間を進める
	updateFrame();

	// 再生時間を適用
	MV1SetAttachAnimTime(m_parentModel, m_attachID, m_frame);
	MV1SetAttachAnimBlendRateToFrame(m_parentModel, m_attachID, m_targetID, m_blendRate * m_defaultBlendRate);
}
