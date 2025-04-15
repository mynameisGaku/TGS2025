#pragma once

class GameObject;

/// <summary>
/// �R���|�[�l���g
/// </summary>
class Component {
public:
	GameObject* parent;	// �ۗL��

	Component() : parent(nullptr), isActive(true), destroyFlag(false) {};

	virtual ~Component() {};
	virtual void Update() {};
	virtual void Draw() {};

	/// <summary>
	/// �폜���s���t���O���I���ɂ���
	/// </summary>
	inline void DestroyMe() { destroyFlag = true; }

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �������Ă��邩��ݒ肷��
	/// </summary>
	inline void SetIsActive(bool value) { isActive = value; }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �폜���邩���擾����
	/// </summary>
	inline const bool IsDestroyFlag() const { return destroyFlag; }

	/// <summary>
	/// �������Ă��邩���擾����
	/// </summary>
	inline const bool IsActive() const { return isActive; }

	/// <summary>
	/// �ۗL�҂̃|�C���^�[���擾����
	/// </summary>
	/// <typeparam name="C">�ۗL�҂̃N���X</typeparam>
	/// <returns>�ۗL�҂̃|�C���^�[��Ԃ��B�N���X����v���Ȃ��ꍇ�Anullptr��Ԃ�</returns>
	template<class C>
	C* Parent() const { return dynamic_cast<C*>(parent); }

protected:
	bool isActive;		// �������Ă��邩
	bool destroyFlag;	// �폜���s����
};