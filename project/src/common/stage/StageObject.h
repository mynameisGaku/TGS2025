#pragma once
// ���p����
#include "src/util/object3D/Object3D.h"

// ���ėp
#include <string>

// ���ʂŕK�v�ȕ�
#include "src/common/stage/stageDefine.h"

using namespace StageDefine;

/// <summary>
/// �X�e�[�W�I�u�W�F�N�g�̏��
/// </summary>
class StageObject : public Object3D {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	StageObject(StageObjInfo info, const Transform& _transform, int _id, bool _collider);
	~StageObject();

	void Draw() override;

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ID��ݒ肷��
	/// </summary>
	inline void SetID(int _id) { id = _id; }

	/// <summary>
	/// �����̒l��ID�ɉ��Z����
	/// </summary>
	inline void AddingID(int value) { id += value; }

	/// <summary>
	/// �����蔻����@�\�����邩��ݒ肷��
	/// </summary>
	/// <param name="value">True�̏ꍇ�A�����蔻����s��</param>
	inline void SetIsCollider(bool value) { isCollider = value; }

	/// <summary>
	/// �����蔻��p���f���̕`���؂�ւ�
	/// </summary>
	/// <param name="value">True�̏ꍇ�A�`����s��</param>
	inline void SetIsHitModelDraw(bool value) { isHitModelDraw = value; }
	
	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ID���擾����
	/// </summary>
	inline int ID() const { return id; }

	/// <summary>
	/// �����蔻�肪�@�\���Ă��邩���擾����
	/// </summary>
	inline bool IsCollider() const { return isCollider; }
	inline bool& IsColliderPtr() { return isCollider; }

	/// <summary>
	/// �����蔻��p���f���̕`��؂�ւ����擾����
	/// </summary>
	inline bool IsHitModelDraw() const { return isHitModelDraw; }

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g�̏����擾����
	/// </summary>
	inline StageObjInfo Info() const { return info; }

private:
	StageObjInfo info;	// �X�e�[�W�I�u�W�F�N�g�̏��
	int id;				// ID
	bool isCollider;	// �����蔻��̉�
	bool isHitModelDraw;// �����蔻��p���f���̕`��؂�ւ�
};