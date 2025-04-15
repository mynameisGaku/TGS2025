#pragma once

// ���p����
#include "Component.h"

// ���ėp
#include "../Util/Vector3.h"
#include <list>
#include <string>
#include <assert.h>

class Object3D;

/// <summary>
/// �U���̏������N���X
/// </summary>
class Shake : public Component {
public:
	//================================================================================
	// ���񋓌^

	// �U���̎��
	enum class Type {
		tNone = -1,
		tVertical,	// �c�h��
		tHorizontal,// ���h��
		tDepth,		// ���h��
		tAll,		// �c+��+�� �h��
	};

	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Shake();
	~Shake();
	
	//================================================================================
	// ���e��֐�

	void Init(Object3D* parent);
	void Update() override;
	void Draw() override;

	void IntervalUpdate();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �U����ݒ肷��
	/// </summary>
	/// <param name="type">�U���̎��</param>
	/// <param name="power">�U���̋���</param>
	/// <param name="duration">���ʎ���</param>
	void SetParam(std::list<Type> type, float power, Vector3 interval, float duration);

	void ApplyShakeTrs(std::string frameName, float magnification);
	void ClearShakeTrs(std::string frameName);

	//================================================================================
	// ���Q�b�^�[

	// �U���̎�ނ��擾����
	inline std::list<Type> GetTypes() const { return types; }
	
	// �U���̋���
	inline Vector3 Power() const { return power; }
	
	// �U���̋���(�����l)
	inline Vector3 InitPower() const { return initPower; }
	
	// �U���̋���(�s��)
	inline MATRIX Matrix() const { return MGetTranslate(power); }

	// ���ʎ���
	inline float Duration() const { return duration; }

	// ���ʎ���(�����l)
	inline float InitDuration() const { return initDuration; }
	
	// ���ʎ��Ԃ�0..1�Ŏ擾����
	inline float CurrentRatio() const {
		if (initDuration == 0)
			assert(0 && "�돜�Z�ł�");
		return duration / initDuration;
	}

	// �U�����I�����������擾����
	inline bool IsFinish() const { return (duration <= 0.0f); }

private:
	Object3D* parentObj;

	std::list<Type> types;	// �U���̎��

	Vector3 power;		// �U���̋���
	Vector3 initPower;	// �U���̋���(����l)

	Vector3 interval;		// �U���̌�����ύX����Ԋu
	Vector3 initInterval;	// �U���̌�����ύX����Ԋu(����l)
	Vector3 direction;		// �U���̌���

	float duration;		// ���ʎ���
	float initDuration;	// ���ʎ���(����l)
};