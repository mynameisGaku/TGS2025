#pragma once
#include "framework/gameObject.h"
#include "framework/myDxLib.h"
#include "src/util/transform/Transform.h"

/// <summary>
/// 3D�I�u�W�F�N�g�Ɋւ������N���X
/// <remarks>�g�����X�t�H�[���⃂�f���̏���ێ����Ă���</remarks>
/// </summary>
class Object3D : public GameObject {
public:
	//==========================================================================================
	// ��public�ϐ�

	Transform* transform;	// ���W�E��]�E�g�k�̏��

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Object3D();

	/// <summary>
	/// ������Tranform�ŏ��������s��
	/// </summary>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	Object3D(const Transform& transform);


	/// <summary>
	/// ������Tranform�ŏ��������s��
	/// </summary>
	/// <param name="model">�`�惂�f��</param>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	Object3D(const int& model, const Transform& transform);
	
	virtual ~Object3D();

	//==========================================================================================
	// ���e���

	virtual void Draw() override;

	/// <summary>
	/// ���W������`�悷��(���f�o�b�O�@�\)
	/// </summary>
	void DrawAixs();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���W�E��]�E�g�k�̏���ݒ肷��
	/// </summary>
	void SetTransform(const Transform& trs);

	/// <summary>
	/// ���f����ݒ肷��
	/// </summary>
	/// <param name="handle">���f���̃n���h��</param>
	void SetModel(const int& handle);

	/// <summary>
	/// ���f����`�悷�邩�ݒ肷��
	/// </summary>
	inline void SetModelDraw(const bool& value) { isModelDraw = value; }

	/// <summary>
	/// ���W������`�悷�邩�ݒ肷��
	/// </summary>
	inline void SetAxisDraw(const bool& value) { isAxisDraw = value; }

	inline void SetLocalMatrix(const MATRIX& mat) { localMatrix = mat; }

	//==========================================================================================
	// ���Q�b�^�[

	// ���f���f�[�^���擾����
	inline const int Model() const { return hModel; }

	// ���f����`�悷�邩�擾����
	inline bool IsModelDraw() const { return isModelDraw; }

	// ���W������`�悷�邩�擾����
	inline bool IsAxisDraw() const { return isAxisDraw; }

protected:
	//==========================================================================================
	// �������o�ϐ�

	int hModel;			// ���f���̃n���h��

	MATRIX localMatrix;

	bool isModelDraw;	// ���f���̕`��̗L��
	bool isAxisDraw;	// ���W�����̕`��̗L��

};