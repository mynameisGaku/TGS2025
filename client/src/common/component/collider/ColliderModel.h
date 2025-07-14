#pragma once
#include "src/common/component/collider/ColliderBase.h"

/// <summary>
/// ���f���̌`���̂܂ܓ����蔻��
/// </summary>
class ColliderModel : public ColliderBase {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	// ������
	ColliderModel();
	~ColliderModel() {};

	//==========================================================================================
	// ���e��֐�

	void UpdateData() override;

	//==========================================================================================
	// ���Z�b�^�[

	inline void SetRefresh(bool _refresh) { refresh = _refresh; }

	//==========================================================================================
	// ���Q�b�^�[

	inline int Model() const { return hModel; }
	inline bool Refresh() const { return refresh; }

private:
	//==========================================================================================
	// �������o�ϐ�

	int hModel;		// ���f���̏��
	bool refresh;	// �ʒu�����X�V���邩
};
