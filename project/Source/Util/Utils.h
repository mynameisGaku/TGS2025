#pragma once
#include <DxLib.h>
#include <assert.h>
#include <string>
#include <list>
#include <vector>

#define _OX_EPSILON_ 0.000001f // �덷

namespace Math {

	//==================================================
	// �����w�֘A

	// �~����
	static const float PI = 3.1415927f;

	// �~������2�{
	static const float PI_TW = PI * 2.0f;

	// �~������2��
	static const float PI_SQ = PI * PI;

	// �d��(2D)
	static const float GRAVITY_2D = 9.8f / 20.0f;

	// �d��(3D)
	static const float GRAVITY_3D = -GRAVITY_2D;

	// �x���@���ʓx�@(���W�A��)�ɕϊ�
	// ��FDegToRad(180) = 3.14...
	template<typename T = float>
	inline float DegToRad(T deg) { return static_cast<float>(deg) * PI / 180.0f; }

	// �ʓx�@(���W�A��)��x���@�ɕϊ�
	// ��FRadToDeg(3.14) = 180
	template<typename T = float>
	inline float RadToDeg(T rad) { return static_cast<float>(rad) * 180.0f / PI; }

	// �����ɂ���
	template<typename T = float>
	inline float Half(T value) { return static_cast<float>(value) / 2; }

	// �l���ɂ���
	template<typename T = float>
	inline float Quarter(T value) { return static_cast<float>(value) / 4; }

	/// <summary>
	/// �m���̌v�Z
	/// </summary>
	/// <param name="probability">�m��(���S����)</param>
	/// <returns>���������ꍇ�ATrue</returns>
	inline bool Probability(int probability) { return GetRand(99) < probability; }

	/// <summary>
	/// �w�肵���ŏ��l�ƍő�l�̊Ԃɐ�������
	/// </summary>
	/// <param name="v">���ݒl</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>���ݒl�ɐ��������������l</returns>
	template<typename T = float>
	inline T Clamp(T v, T min, T max) {
		if (v < min)
			return min;
		else if (v > max)
			return max;
		return v;
	}

	inline float RateDecrease(float start, float rate) {

		if (rate >= 1.0f || rate <= 0.0f)
			return start;

		float value = start;
		float ans = 0.0f;
		bool isLoop = true;

		while (isLoop) {
			value *= rate;
			ans += value;

			if (value <= _OX_EPSILON_)
				isLoop = false;
		}

		return ans;
	}
}

namespace String {

	/// <summary>
	/// �ꕶ�����ۑ�����
	/// </summary>
	struct Letter {
		std::string str;	// ����
		float x;	// �`����W(X��)
		float y;	// �`����W(Y��)

		Letter() : Letter("", 0.0f, 0.0f) {}

		Letter(const std::string& s, float _x, float _y) :
			str(s),
			x(_x),
			y(_y)
		{
		};
	};

	/// <summary>
	/// �t�H���_�p�X�̏�������
	/// </summary>
	struct Folder {
		std::string filePath;		// �t�@�C���p�X
		std::string resourceName;	// ���\�[�X��
		std::string tag;			// �^�O

		Folder() :
			filePath(""),
			resourceName(""),
			tag("")
		{
		}

		/// <summary>
		/// �t�H���_�p�X�̏���ݒ肷��
		/// </summary>
		/// <param name="_filePath">�t�@�C���p�X</param>
		/// <param name="resource">���\�[�X��</param>
		/// <param name="_tag">�^�O</param>
		Folder(const std::string& _filePath, const std::string& resource, const std::string& _tag) :
			filePath(_filePath),
			resourceName(resource),
			tag(_tag)
		{
		}

		/// <summary>
		/// �w�肳�ꂽ�t�@�C���p�X�ƈ�v���Ă��邩
		/// </summary>
		/// <param name="file">�t�@�C���p�X</param>
		/// <returns>��v���Ă���ꍇtrue</returns>
		inline bool IsFilePath(const std::string& file) const { return filePath == file; }

		/// <summary>
		/// �w�肳�ꂽ���\�[�X���ƈ�v���Ă��邩
		/// </summary>
		/// <param name="resource">���\�[�X��</param>
		/// <returns>��v���Ă���ꍇtrue</returns>
		inline bool IsResourceName(const std::string& resource) const { return resourceName == resource; }

		/// <summary>
		/// �w�肳�ꂽ�^�O�ƈ�v���Ă��邩
		/// </summary>
		/// <param name="_tag">�t�@�C���p�X</param>
		/// <returns>��v���Ă���ꍇtrue</returns>
		inline bool IsTag(const std::string& _tag) const { return tag == _tag; }
	};
}

namespace Screen {

	//==================================================
	// ���X�N���[���֘A

	// ��ʂ̉���
	static const int WIDTH = 1280;

	// ��ʂ̉���(float)
	static const float WIDTH_F = 1280.0f;

	// ��ʂ̉����̔���
	static const float WIDTH_HALF = Math::Half(WIDTH);

	// ��ʂ̏c��
	static const int HEIGHT = 720;

	// ��ʂ̏c��(float)
	static const float HEIGHT_F = 720.0f;

	// ��ʂ̏c���̔���
	static const float HEIGHT_HALF = Math::Half(HEIGHT);
}

namespace Function {

	/// <summary>
	/// �|�C���^�[���폜����Bnull�`�F�b�N�ƁA�폜���nullptr�������Ă����B
	/// </summary>
	/// <param name="p">�폜�������|�C���^�[</param>
	template<class C>
	inline void DeletePointer(C*& p) {
		if (p != nullptr) {
			delete p;
			p = nullptr;
		}
	}

	/// <summary>
	/// �z��̃|�C���^�[���폜����Bnull�`�F�b�N�ƁA�폜���nullptr�������Ă����B
	/// </summary>
	/// <param name="p">�폜�������|�C���^�[</param>
	template<class C>
	inline void DeletePointerArray(C*& p) {
		if (p != nullptr) {
			delete[] p;
			p = nullptr;
		}
	}

	/// <summary>
	/// �z��̃|�C���^�[���폜����Bnull�`�F�b�N�ƁA�폜���nullptr�������Ă����B
	/// </summary>
	/// <param name="p">�폜�������|�C���^�[</param>
	template<class C>
	inline void DeletePointerList(std::list<C*>& p) {
		if (p.empty())
			return;

		for (auto itr = p.begin(); itr != p.end();) {
			delete* itr;
			itr = p.erase(itr);
		}

		p.clear();
	}

	/// <summary>
	/// �z��̃|�C���^�[���폜����Bnull�`�F�b�N�ƁA�폜���nullptr�������Ă����B
	/// </summary>
	/// <param name="p">�폜�������|�C���^�[</param>
	template<class C>
	inline void DeletePointerVector(std::vector<C*>& p) {
		if (p.empty())
			return;

		for (auto itr = p.begin(); itr != p.end();) {
			delete* itr;
			itr = p.erase(itr);
		}

		p.clear();
	}

	/// <summary>
	/// �p�x�����X�ɕω������鏈���B
	/// ���݂̒l���I�_�p�x�ɂȂ�܂ŁA��]���x�����Z��������B
	/// </summary>
	/// <param name="current">���݂̒l</param>
	/// <param name="terminus">�I�_�p�x</param>
	/// <param name="rotSpeed">��]���x</param>
	template<typename T = float>
	inline float RotAngle(T current, T terminus, T rotSpeed) {

		float ans = current;	// �ŏI�I�Ȍ��ʂ�ێ�����
		float diffAngle = terminus - current;// �p�x�̍���

		// �}�C�i�X�Ȃ����񂵂ăv���X�̕����ɂ���
		if (diffAngle < 0)
			diffAngle += Math::PI_TW;

		float angleReverseDif = -diffAngle + Math::PI_TW;	// �t��]�̑傫��(�l�̓v���X)

		// ���v���̕����߂��ꍇ
		if (diffAngle < angleReverseDif) {
			if (diffAngle < rotSpeed)		// �p�x�̍��قƉ�]���x���r
				ans = terminus;
			else
				ans += rotSpeed;
		}
		// �����v���̕����߂��ꍇ
		else {
			if (angleReverseDif < rotSpeed)// �p�x�̍��قƉ�]���x���r
				ans = terminus;
			else
				ans -= rotSpeed;
		}

		return ans;
	}

	/// <summary>
	/// �t�H�[�}�b�g���ꂽ�������std::string�^�ɕϊ�����
	/// </summary>
	inline std::string FormatToString(const char* format, ...) {

		char buffer[1024];  // �o�b�t�@�T�C�Y�͕K�v�ɉ����Ē���

		va_list valist;
		va_start(valist, format);
		vsprintf_s(buffer, format, valist);  // �o�b�t�@�Ƀt�H�[�}�b�g���ꂽ���������������
		va_end(valist);

		return static_cast<std::string>(buffer);
	}

	/// <summary>
	/// �l�̌ܓ����s��
	/// </summary>
	/// <param name="value">�l�̌ܓ����s���l</param>
	/// <param name="decimalPoint">�����_���ʂ܂Ŏl�̌ܓ����邩</param>
	/// <returns>�l�̌ܓ����s�����l���擾����</returns>
	inline float Rounding(float value, int decimalPoint) {

		float result = 0.0f;

		result = value * static_cast<float>(pow(10, decimalPoint - 1)); //�l�̌ܓ��������l��10��(n-1)��{����B
		result = roundf(result); //�����_�ȉ����l�̌ܓ�����B
		result /= static_cast<float>(pow(10, decimalPoint - 1)); //10��(n-1)��Ŋ���B
		return result;
	}

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <param name="value">�������擾����l</param>
	inline int Digit(int value) {

		int digit = 0;

		if (value == 0)
			return 1;

		while (value != 0) {
			value /= 10;
			digit++;
		}

		return digit;
	}

	/// <summary>
	/// ���ݒl����I�_�܂ł��Ԃ���B
	/// </summary>
	/// <param name="current">���ݒl</param>
	/// <param name="terminus">�I�_</param>
	/// <param name="rate">�{��</param>
	inline void Interpolation(float* current, float terminus, float rate) {

		const float diff = terminus - *current;
		if (diff == 0.0f)
			return;

		*current += diff * rate;

		bool isPositive = (diff > 0.0f);

		if (isPositive) {
			if (*current >= terminus)
				*current = terminus;
		}
		else {
			if (*current <= terminus)
				*current = terminus;
		}

	}
};