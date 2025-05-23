#pragma once
#include "framework/myDxLib.h"
#include "src/util/file/FileUtil.h"
#include <string>

class FontInfo;

/// <summary>
/// �t�H���g�̓ǂݍ��݁E�Ǘ����s��
/// </summary>
namespace Font {

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// �t�H���g�̃f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�H���g���u����Ă���t�@�C���p�X</param>
	/// <param name="resourceName">�t�H���g�̃��\�[�X��</param>
	/// <param name="fontName">�t�H���g�̖��O</param>
	void Load(const std::string& filePath, const std::string& resourceName, const std::string& fontName);

	/// <summary>
	/// �w�肳�ꂽ�t�H���g��񂩂�t�H���g�n���h���𐶐�����
	/// </summary>
	/// <param name="info">�t�H���g���</param>
	/// <returns>�t�H���g�n���h��</returns>
	int CreateFontToHandle(FontInfo* info);

}

/// <summary>
/// �t�H���g�̏��
/// </summary>
class FontInfo {
public:
	int handle;		// �n���h��
	int size;		// �傫��
	int thick;		// ����
	int fontType;	// �t�H���g�̃^�C�v(DX_FONTTYPE_~~)
	int charSet;	// 
	int edgeSize;	// ���̑傫��
	int italic;		// �΂�
	int color;		// �F
	FileUtil::Folder strData;// �t�H���g�Ŏg�p���镶������

	FontInfo() :
		handle(-1),
		size(-1),
		thick(-1),
		fontType(-1),
		charSet(-1),
		edgeSize(-1),
		italic(0),
		color(GetColor(255, 255, 255))
	{
	}

	~FontInfo(){}

	inline void Init(const std::string& tag, const int& _size, const int& _color, const int& _fontType, const int& _edgeSize) {
		strData.tag = tag;
		size = _size;
		color = _color;
		fontType = _fontType;
		edgeSize = _edgeSize;

		Font::CreateFontToHandle(this);
	}
};