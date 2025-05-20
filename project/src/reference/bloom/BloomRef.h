#pragma once

/// <summary>
/// �{�[����Json�p�����[�^��ǂݍ���
/// </summary>
/// <author>�����h�l</author>
class BloomRef
{
private:
	const std::string PATH = "data/Json/Bloom/Bloom.json";
	const std::string FILEKEY = "Bloom";

	static BloomRef* instance;
public:
	struct Parameter
	{
		int DownScale;	// �t�B���^��������Ƃ��̏k���x�A�傫���قǏ������y���i1..8���炢�j
		int MinBrightness;	// ���P�x�𔲂��o���Ƃ��̍ŏ����邳�i0..255�j
		int GaussianRate;	// �K�E�X�ڂ����̋����i0..1000���炢�j
		int AddRate;	// ���Z���鋭���i0..255�j
	};

	static BloomRef* Inst()
	{
		if (instance == nullptr)
		{
			instance = new BloomRef;
		}
		return instance;
	}

	// " ���t�@�����X��� "
	// ���𕶎��񌟍������璅���Ƃ���ŉ�����Ă܂��B(�����炭���C�����[�v�̂�������)
	void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
		instance = nullptr;
	}

	void Load();

	Parameter Param;	// �u���[���̃p�����[�^
};

#define BLOOM_REF (*BloomRef::Inst())

// Json�ǂݍ��ݗp
#include <vendor/nlohmann/json_fwd.hpp>

void to_json(nlohmann::json& j, const BloomRef::Parameter& param);
void from_json(const nlohmann::json& j, BloomRef::Parameter& param);

