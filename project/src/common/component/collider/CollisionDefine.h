#pragma once

// ���ėp
#include "src/util/transform/Transform.h"
#include <list>

class ColliderBase;

/// <summary>
/// �����蔻��Ɋւ����`�l���܂Ƃ߂�namespace
/// </summary>
namespace ColDefine {

	//================================================================================
	// ���񋓌^

	// �����蔻��̌`
	enum class Shape {
		sNone = -1,
		sSphere,	// ��
		sCapsule,	// �J�v�Z��
		sModel,		// ���f��
	};

	// �����蔻��̌n��
	enum class Tag {
		tNone = -1,
		tCharaRed,	// �ԃ`�[���L����
		tCharaBlue,	// �`�[���L����
		tBallRed,	// �ԃ`�[���{�[��
		tBallBlue,	// �`�[���{�[��
		tCatchRed,	// �ԃ`�[���L���b�`
		tCatchBlue,	// �`�[���L���b�`
		tTerrain,	// �n�`
		tMax,
	};

	struct ColBaseParam {
		Transform trs;
		Tag tag = Tag::tNone;
		std::list<ColDefine::Tag> targetTags;
		bool onlyOnce = false;
		bool push = false;
	};

	// �_�ƕ����x�N�g��
	struct Lay {
		Vector3 point;	// ���W
		Vector3 vec;	// �����x�N�g��
		Lay() : point(Vector3::Zero), vec(Vector3::Zero) {}
		Lay(const Vector3& p, const Vector3& v) : point(p), vec(v) {}
		~Lay() {}

		// �_��̍��W���擾
		//  �x�N�g���Ɋ|���Z����W��
		inline Vector3 getPoint(float t) const { return point + vec * t; }
	};

	// ����
	struct Segment : public Lay {
		Segment() : Lay(Vector3::Zero, Vector3::Zero) {}
		Segment(const Vector3& p, const Vector3& v) : Lay(p, v) {}

		// �I�_���擾
		inline Vector3 getEndPoint() const { return point + vec; }
	};

	// ��
	struct Sphere {
		Vector3 point;	// ���W
		float raduis;	// ���a
	};

	// �J�v�Z��
	struct Capsule {
		Vector3 begin;	// �����̎n�_
		Vector3 end;	// �����̏I�_
		float raduis;	// ���a
	};

	// �R�[��
	struct Cone {
		Transform transform;// ���W�E��]�E�g�k
		float range;		// ����
		float angle;		// �p�x
	};

	// AABB
	struct AABB {
		Vector3 p;	// ���S�_
		Vector3 hl;	// �e���̕ӂ̒����̔���
		AABB() {}
		AABB(const Vector3& p, const Vector3& hl) : p(p), hl(hl) {}

		// �ӂ̒������擾
		inline float lenX() const { return hl.x * 2.0f; };
		inline float lenY() const { return hl.y * 2.0f; };
		inline float lenZ() const { return hl.z * 2.0f; };

		inline float len(int i) { return *((&hl.x) + i) * 2.0f; }
	};
}