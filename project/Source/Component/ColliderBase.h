#pragma once
// ���p����
#include "component.h"

// ���ėp
#include <list>

// ���ʂŕK�v�ȕ�
#include "collisionDefine.h"

class HittedData;
class Transform;

/// <summary>
/// �����蔻��̊��R���|�[�l���g
/// </summary>
class ColliderBase : public Component {
public:
	//==========================================================================================
	// ��public�ϐ�

	// ���g�̃g�����X�t�H�[��
	// holder�̃g�����X�t�H�[���������Őe�ɂȂ�
	Transform* transform;

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	ColliderBase();
	virtual ~ColliderBase();

	//==========================================================================================
	// ���e��֐�

	void BaseInit(const ColDefine::ColBaseParam& param);

	virtual void UpdateData();

	// ����̃^�O���ΏۂɊ܂܂�Ă��邩
	bool IsTarget(ColDefine::Tag otherTag);

	//==========================================================================================
	// ���Z�b�^�[

	// �����蔻��̌n����ݒ肷��
	inline void SetTag(ColDefine::Tag value) { tag = value; }
	
	// �����蔻����s������̓����蔻��̌n����ݒ肷��
	inline void SetTargetTags(const std::list<ColDefine::Tag>& value) { targetTags = value; }
	
	// �����蔻����s������ݒ肷��
	inline void SetIsActive(bool value) { isActive = value; }

	// �����蔻��̕`����s������ݒ肷��
	inline void SetDraw(bool value) { isDraw = value; }

	//==========================================================================================
	// ���Q�b�^�[
	inline const Transform Global() const { return *global; }

	// �����蔻��̌`���擾����
	inline const ColDefine::Shape Shape() const { return shape; }

	//�����蔻��̌n�����擾����
	inline const ColDefine::Tag Tag() const { return tag; }

	// �����蔻����s������̓����蔻��̌n�����擾����
	inline const std::list<ColDefine::Tag> TargetTags() const { return targetTags; }

	// �����蔻����s�������擾����
	inline const bool IsActive() const { return isActive; }

	// �����蔻��̕`����s�������擾����
	inline bool IsDraw() const { return isDraw; }

	// ��x����������Ȃ������蔻��ŁA���ɓ������������ۑ����������擾����
	inline HittedData* HittedDataPtr() const { return hittedData; }

protected:
	Transform* global;	// �����蔻�莞�̃g�����X�t�H�[��

	ColDefine::Shape shape;	// �����蔻��̌`
	ColDefine::Tag tag;		// �����蔻��̌n��
	std::list<ColDefine::Tag> targetTags;	// �����蔻����s������̓����蔻��B���̃��X�g�ɖ������̂Ƃ͓����蔻����s��Ȃ�

	bool isActive;	// �����蔻����s��
	bool isDraw;	// �����蔻��̉���

	HittedData* hittedData;	// ��x����������Ȃ������蔻��ŁA���ɓ������������ۑ�����B�A���q�b�g�ő��v�Ȃ�nullptr�ɂ���
};

/// <summary>
/// ��x����������Ȃ������蔻��ŁA���ɓ������������ۑ�����
/// </summary>
class HittedData {
public:
	//==========================================================================================
	// ���e��֐�

	/// <summary>
	/// ���ɓ������Ă��邩���ׂ�
	/// </summary>
	/// <param name="other">���ׂ�|�C���^</param>
	/// <returns>�������Ă�����true</returns>
	bool IsHitted(ColliderBase const* other);

	// �����������X�g�ǉ�
	inline void AddHitted(ColliderBase const* hit) { hitted.push_back(hit); }

	// �����������X�g�����Z�b�g
	inline void Reset() { hitted.clear(); }

private:
	//==========================================================================================
	// �������o�ϐ�

	// ���ɓ����蔻����s��������
	std::list<ColliderBase const*> hitted;
};