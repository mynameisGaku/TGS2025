#pragma once

// ���ėp
#include "src/util/math/vector2.h"
#include "src/util/math/vector3.h"

// ���ʂŕK�v�ȕ�
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/component/collider/CollisionData.h"
#include "src/common/component/collider/ColliderBase.h"
#include "src/common/component/collider/ColliderSphere.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/common/component/collider/ColliderModel.h"

using namespace ColDefine;

/// <summary>
/// �����蔻��̊֐�
/// </summary>
namespace ColFunction {

	/// <summary>
	/// ColliderBase���g�p���������蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderBase�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderBase�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData* ColCheck(ColliderBase* col1, ColliderBase* col2);

	//==========================================================================================
	// �����`

	/// <summary>
	/// 3D�̋��Ƌ��̓����蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderSphere�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderSphere�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck_SphereToSphere(ColliderSphere* col1, ColliderSphere* col2);

	/// <summary>
	/// 3D�̋��ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderSphere�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderCapsule�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck_SphereToCapsule(ColliderSphere* col1, ColliderCapsule* col2);

	/// <summary>
	/// 3D�̋��ƃ��f���̓����蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderSphere�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderModel�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck_SphereToModel(ColliderSphere* col1, ColliderModel* col2);

	//==========================================================================================
	// ���J�v�Z��

	/// <summary>
	/// 3D�̃J�v�Z���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderCapsule�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderCapsule�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck_CapsuleToCapsule(ColliderCapsule* col1, ColliderCapsule* col2);

	/// <summary>
	/// 3D�̃J�v�Z���ƃ��f���̓����蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderCapsule�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderModel�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck_CapsuleToModel(ColliderCapsule* col1, ColliderModel* col2);

	//==========================================================================================
	// �����f��

	/// <summary>
	/// 3D�̃��f���ƃ��f���̓����蔻��
	/// </summary>
	/// <param name="_col1">�����蔻����s��ColliderModel�̃|�C���^</param>
	/// <param name="_col2">�����蔻����s��ColliderModel�̃|�C���^</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck_ModelToModel(ColliderModel* col1, ColliderModel* col2);

	//==========================================================================================
	// ���R�[��

	CollisionData ColCheck_ConeToPoint(const Cone& cone, const Vector3& point);

	//==========================================================================================
	// ���v�Z

	/// <summary>
	/// 3D�̓����蔻��(�~)
	/// </summary>
	/// <param name="p1">�����蔻����s�����W</param>
	/// <param name="r1">p1�̔���̑傫��(���a)</param>
	/// <param name="p2">�����蔻����s�����W</param>
	/// <param name="r2">p2�̔���̑傫��(���a)</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck3D_Circle(Vector3 p1, float r1, Vector3 p2, float r2);

	/// <summary>
	/// 2D�̓����蔻��(�~)
	/// </summary>
	/// <param name="p1">�����蔻����s�����W</param>
	/// <param name="r1">p1�̔���̑傫��(���a)</param>
	/// <param name="p2">�����蔻����s�����W</param>
	/// <param name="r2">p2�̔���̑傫��(���a)</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck2D_Circle(Vector2 p1, float r1, Vector2 p2, float r2);

	/// <summary>
	/// 2D�̓����蔻��(�l�p�E�n�_�I�_)
	/// </summary>
	/// <param name="p1">�����蔻����s�����W</param>
	/// <param name="s1">p1�̔���̑傫��(�E��)</param>
	/// <param name="p2">�����蔻����s�����W</param>
	/// <param name="s2">p2�̔���̑傫��(�E��)</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck2D_Box(Vector2 p1, Vector2 s1, Vector2 p2, Vector2 s2);

	/// <summary>
	/// 2D�̓����蔻��(�l�p�E�x�N�g��)
	/// </summary>
	/// <param name="p1">�����蔻����s�����W</param>
	/// <param name="p2">�����蔻����s�����W</param>
	/// <param name="v">������Ƃ�x�N�g���̑傫��</param>
	/// <returns>���������ꍇ:True</returns>
	CollisionData ColCheck2D_Box(Vector2 p1, Vector2 p2, Vector2 v);

	/// <summary>
	/// ��p1p2p3���s�p���擾����
	/// </summary>
	/// <param name="p1">���W1</param>
	/// <param name="p2">���W2</param>
	/// <param name="p3">���W3</param>
	/// <returns>�s�p�̏ꍇ�Atrue</returns>
	bool IsSharpAngle(const Vector3& p1, const Vector3& p2, const Vector3& p3);

	/// <summary>
	/// �_�ƒ����̍ŒZ�������擾����
	/// </summary>
	/// <param name="p">�_</param>
	/// <param name="l">����</param>
	/// <param name="h">�_���牺�낵�������̑�(�߂�l)</param>
	/// <param name="t">�x�N�g���W��(�߂�l)</param>
	/// <returns>�ŒZ����</returns>
	float CalcPointLineDist(const Vector3& p, const ColDefine::Lay& l, Vector3& h, float& t);

	/// <summary>
	/// �_�Ɛ����̍ŒZ����
	/// </summary>
	/// <param name="p">�_</param>
	/// <param name="seg">����</param>
	/// <param name="h">�ŒZ�����ƂȂ�[�_(�߂�l)</param>
	/// <param name="t">�[�_�ʒu( t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O )</param>
	/// <returns>�ŒZ����</returns>
	float CalcPointSegmentDist(const Vector3& p, const ColDefine::Segment& seg, Vector3& h, float& t);

	/// <summary>
	/// 2�����̍ŒZ����
	/// </summary>
	/// <param name="l1">����1</param>
	/// <param name="l2">����2</param>
	/// <param name="p1">����1�̐����̑�(�߂�l)</param>
	/// <param name="p2">����2�̐����̑�(�߂�l)</param>
	/// <param name="t1">����1�̃x�N�g���W��(�߂�l)</param>
	/// <param name="t2">����1�̃x�N�g���W��(�߂�l)</param>
	/// <returns>�ŒZ����</returns>
	float CalcLineLineDist(const ColDefine::Lay& l1, const ColDefine::Lay& l2, Vector3& p1, Vector3& p2, float& t1, float& t2);

	/// <summary>
	/// 2�����̍ŒZ����
	/// </summary>
	/// <param name="s1">����1</param>
	/// <param name="s2">����2</param>
	/// <param name="p1">����1�̐����̑�(�߂�l)</param>
	/// <param name="p2">����2�̐����̑�(�߂�l)</param>
	/// <param name="t1">����1�̃x�N�g���W��(�߂�l)</param>
	/// <param name="t2">����2�̃x�N�g���W��(�߂�l)</param>
	/// <returns>�ŒZ����</returns>
	float CalcSegmentSegmentDist(const ColDefine::Segment& s1, const ColDefine::Segment& s2, Vector3& p1, Vector3& p2, float& t1, float& t2);

	float CollCheck_PointToAABB(const Vector3& point, const ColDefine::AABB& aabb);
};