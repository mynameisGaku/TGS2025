#pragma once
#include <unordered_map>

class GameObject;
class StateBase;

/// <summary>
/// �X�e�[�g���Ǘ����āA�Ăяo���N���X��ύX����
/// </summary>
class StateManager {
public:
	StateManager(GameObject* parent);
	~StateManager();

	void Update();
	void Draw();
	
	/// <summary>
	/// �X�e�[�g��ǉ�����
	/// </summary>
	/// <param name="id">�X�e�[�g�̔��ʔԍ�</param>
	/// <param name="inst">�X�e�[�g�̃C���X�^���X</param>
	void AddInstance(int id, StateBase* inst);

	/// <summary>
	/// �X�e�[�g��ύX����
	/// </summary>
	/// <param name="newState">�X�e�[�g�̔��ʔԍ�</param>
	void ChangeState(int newState);
	
	//==================================================
	// ���Q�b�^�[

	/// <summary>
	/// �ۗL�҂̃|�C���^�[���擾����
	/// </summary>
	/// <returns>GameObject�̃|�C���^�[</returns>
	inline GameObject* Object() const { return parent; }

	/// <summary>
	/// �X�e�[�g�̃|�C���^�[���擾����
	/// </summary>
	/// <param name="id">�X�e�[�g�̔��ʔԍ�</param>
	/// <returns>�X�e�[�g�̃|�C���^�[�Bid����v���Ă��Ȃ��ꍇ�Anullptr</returns>
	inline StateBase* State(int id) { return stateInstances[id]; }

	/// <summary>
	/// ���݂̃X�e�[�g�̃|�C���^�[���擾����
	/// </summary>
	/// <returns>���ݎg�p���Ă���X�e�[�g�̃|�C���^�[</returns>
	inline StateBase* CurrentState() { return stateInstances[currentStateID]; }

	/// <summary>
	/// ���݂̃X�e�[�g�̔ԍ����擾����
	/// </summary>
	/// <returns>���ݎg�p���Ă���X�e�[�g�̔ԍ�</returns>
	inline int CurrentStateID() const { return currentStateID; }

	/// <summary>
	/// ��O�̃X�e�[�g�̔ԍ����擾����
	/// </summary>
	/// <returns>��O�̃X�e�[�g�̔ԍ�</returns>
	inline int PrevStateID() const { return prevStateID; }

	/// <summary>
	/// �ύX���\�񂳂�Ă���X�e�[�g�̔ԍ����擾����
	/// </summary>
	/// <returns>�ύX�\��̃X�e�[�g�̔ԍ�</returns>
	inline int NextStateID() const { return nextStateID; }

	inline const std::unordered_map<int, StateBase*> AllStateInstances() const { return stateInstances; }

private:
	GameObject* parent;
	int currentStateID;
	int prevStateID;
	int nextStateID;

	std::unordered_map<int, StateBase*> stateInstances;
};