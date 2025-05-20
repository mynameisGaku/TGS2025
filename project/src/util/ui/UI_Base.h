//#pragma once
//#include "../Library/gameObject.h"
//#include "../Source/Util/Utils.h"
//#include "../Source/Util/Vector3.h"
//#include "../Source/Util/Vector2.h"
//#include "gauge.h"
//
//#include <math.h>
//#include <algorithm>
//#include <list>
//#include <string>
//
////�摜�̋P�x�Ɋւ���ϐ����܂Ƃ߂����́B�֐��ǉ��\
//
///// <summary>
///// UI�Ɏg�p����摜�̋P�x��\���N���X
///// </summary>
///// <author> G.F </author>
//class UI_Bright {
//public:
//	inline UI_Bright(float r = 255, float g = 255, float b = 255, float a = 255) {
//		red = r;
//		green = g;
//		blue = b;
//		alpha = a;
//	}
//
//	float red;
//	float green;
//	float blue;
//	float alpha;
//};
//
///// <summary>
///// UI�Ɏg�p����摜�f�[�^�N���X
///// </summary>
///// <author> G.F </author>
//class UI_Graph {
//public:
//	inline UI_Graph() {
//		handle = -1;
//		size_x = 0;
//		size_y = 0;
//		cell_x = 0;
//		cell_y = 0;
//		center_x = 0;
//		center_y = 0;
//		alpha = 255;
//		ext = 1;
//		angle = 0;
//		bright = UI_Bright();
//		pos = V2::ZERO;
//	}
//	int handle;
//	int size_x;
//	int size_y;
//	int cell_x;
//	int cell_y;
//	float center_x;
//	float center_y;
//	float alpha;
//	float ext;
//	float angle;
//	UI_Bright bright;
//	Vector2 pos;
//};
//
///// <summary>
///// �r���{�[�h�Q�[�W�p�̃p�����[�^�N���X
///// </summary>
///// <author> G.F </author>
//class BillGaugeParam {
//public:
//	float x1, y1, x2, y2, x3, y3, x4, y4;
//	float u1, v1, u2, v2, u3, v3, u4, v4;
//
//	inline BillGaugeParam(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
//		float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
//	{
//		this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2; this->x3 = x3; this->y3 = y3; this->x4 = x4; this->y4 = y4;
//		this->u1 = u1; this->v1 = v1; this->u2 = u2; this->v2 = v2; this->u3 = u3; this->v3 = v3; this->u4 = u4; this->v4 = v4;
//	}
//
//	inline void SetUV(float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
//	{
//		this->u1 = u1; this->v1 = v1; this->u2 = u2; this->v2 = v2; this->u3 = u3; this->v3 = v3; this->u4 = u4; this->v4 = v4;
//	}
//	
//	inline void SetPos(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
//	{
//		this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2; this->x3 = x3; this->y3 = y3; this->x4 = x4; this->y4 = y4;
//	}
//
//};
//namespace
//{
//	const int VERT_INDEX = 4;
//}
//
///// <summary>
///// �r���{�[�h�ɂ��Q�[�W�`��N���X
///// </summary>
///// <author> G.F </author>
//class BillGauge
//{
//public:
//	BillGauge()
//	{
//		for (int i = 0; i < VERT_INDEX; i++)
//		{
//			m_vert[i].pos = Vector3::Zero;				//���� 0, 1
//			m_vert[i].norm = Vector3::Zero;
//			m_vert[i].dif = GetColorU8(255, 255, 255, 255);
//			m_vert[i].spc = GetColorU8(255, 255, 255, 255);
//			m_vert[i].u = 0.0f;
//			m_vert[i].v = 0.0f;
//			m_vert[i].sv = 0.0f;
//			m_vert[i].su = 0.0f;
//		}
//		m_polygonNum = 2;
//		m_graphHandle = -1;
//		m_pos = Vector3::Zero;
//		m_pRawVertexNorm = VGet(0, 0, 1);
//	}
//	VERTEX3D m_vert[VERT_INDEX];             //�r���{�[�h�`��p���_���W  ���̃N���X�̓Q�[�W�ׂ̈̃N���X�Ȃ̂Ŋ�{���_��4�B
//	int m_polygonNum;             //�|���S����   ��{�Q��
//	int m_graphHandle;            //�r���{�[�h�ɓ\��摜
//	Vector3 m_pos;               //�r���{�[�h�̍��W
//	Vector3 m_pRawVertexNorm;   //�I���W�i���̖@������ێ�����
//};
//
///// <summary>
///// UI�̃x�[�X�ƂȂ�N���X�B
///// </summary>
///// <author> G.F </author>
//class UI_Base
//{
//public:
//	UI_Base() { Initialize(); }
//	virtual ~UI_Base() {}
//
//	virtual void Start() = 0;
//	virtual void Update() = 0;
//	virtual void Draw() = 0;
//
//	/// <summary>
//	/// �S�Ă�UI�h���N���X�ɋ��ʂ��鏉�����֐��B
//	/// ����𑖂点�Ƃ��΂Ƃ肠�����������ł���B
//	/// </summary>
//	void Initialize();
//
//	/// <summary>
//	/// �V�[���̃|�C���^���Z�b�g����
//	/// </summary>	
//	inline void SetSceneInfo(SceneBase* _scene) { scene = _scene; }
//
//protected:
//
//	//�V�[���|�C���^
//	SceneBase* scene;
//
//	//�摜�f�[�^
//	UI_Graph graph;
//
//	//�r���{�[�h�f�[�^
//	BillGauge billgauge;
//
//	//�Q�[�W
//	Gauge gauge;
//
//	//�h�ꂪ�\�񂳂�Ă��邩�H
//	bool isShake;
//	//�g�k���\�񂳂�Ă��邩�H
//	bool isBound;
//
//	/*---�f�o�b�O---*/
//	bool isShow;	//�\������H
//
//	//�����擾
//	int GetDigitCount(int number)
//	{
//		if (number == 0) return 1;
//		int digits = 0;
//		while (number != 0) {
//			number /= 10;
//			digits++;
//		}
//		return digits;
//	}
//
//
//public:
//
//	////////////////////////////////////////////////////////////////////////////////////////////
//	/*			�Q�[�W�`��			*/
//
//	// ���_�Q�[�W��`��
//	// Pos: �Q�[�W�̈ʒu���������W
//	// cx: �Q�[�W�̒��S���W��X�l (0.0f�`1.0f) ��]�̌��_
//	// cy: �Q�[�W�̒��S���W��Y�l (0.0f�`1.0f) ��]�̌��_
//	// Size: �Q�[�W�̃T�C�Y
//	// MaxValue: �Q�[�W�̍ő�l
//	// CurrentValue: ���݂̒l
//	// Angle: �Q�[�W�̌X���i���W�A���P�ʁj
//	// GrHandle: �`�悷��摜�̃n���h��
//	// TransFlag: �摜�̓����x��L���ɂ��邩�ǂ����iTRUE: �L��, FALSE: �����j
//	// ReverseFlagX: �摜�����E���]���邩�ǂ����itrue: �L��, false: �����j
//	// ReverseFlagY: �摜���㉺���]���邩�ǂ����itrue: �L��, false: �����j
//	void DrawHorizontalGauge(Vector2 Pos, float cx, float cy, float Size, float MaxValue, float CurrentValue, float Angle, int GrHandle, int TransFlag, bool ReverseFlagX = false, bool ReverseFlagY = false);
//
//	// ���s�l�ӌ`�̃Q�[�W��`�� (�摜��Ή�)
//	// Pos: �Q�[�W�̈ʒu���������W
//	// cx: �Q�[�W�̒��S���W��X�l (0.0f�`1.0f)
//	// cy: �Q�[�W�̒��S���W��Y�l (0.0f�`1.0f)
//	// Size: �Q�[�W�̃T�C�Y
//	// MaxValue: �Q�[�W�̍ő�l
//	// CurrentValue: ���݂̒l
//	// Angle: �Q�[�W�̌X���i���W�A���P�ʁj
//	// Color: �Q�[�W�̐F
//	// FillFlag: �Q�[�W��h��Ԃ����ǂ����iTRUE: �h��Ԃ�, FALSE: �g���̂݁j
//	// ReverseFlagX: �Q�[�W�����E���]���邩�ǂ����itrue: �L��, false: �����j
//	// ReverseFlagY: �Q�[�W���㉺���]���邩�ǂ����itrue: �L��, false: �����j
//	void DrawParallelGauge(Vector2 Pos, float cx, float cy, float Width, float Height, float MaxValue, float CurrentValue, float Angle, int Color, int FillFlag, bool ReverseFlagX = false, bool ReverseFlagY = false);
//
//	/// <summary>
//	/// �r���{�[�h�ŃQ�[�W��`�悷��
//	/// </summary>
//	/// <param name="_pos">�`���̍��W</param>
//	/// <param name="left_down">�����̒��_���W</param>
//	/// <param name="left_up">����̒��_���W</param>
//	/// <param name="right_down">�E���̒��_���W</param>
//	/// <param name="right_up">�E��̒��_���W</param>
//	/// <param name="uv_pos_left">���ʂ��猩���r���{�[�h�̍�����UV���W</param>
//	/// <param name="uv_pos_right">���ʂ��猩���r���{�[�h�̉E����UV���W</param>
//	/// <param name="current">�Q�[�W�ɓK�p������l</param>
//	/// <param name="graph">�摜�n���h��	-1�ŉ摜���g�p���Ȃ�</param>
//	/// <param name="transflag">���߂����邩�H </param>
//	void DrawBillGauge3D(Vector3 _pos, BillGaugeParam param, float current, float currentMax, int graph, bool transflag);
//
//	/*		�摜�Ɋւ���֐�		*/
//
//	/*----------	�Z�b�^�[	----------*/
//	// �g�p����摜���Z�b�g����(���łɉ摜�T�C�Y�Ɖ摜�̒������W�������Ŋi�[)
//	const void SetGraph(int _graph) { graph.handle = _graph; GetGraphSize(graph.handle, &graph.size_x, &graph.size_y); graph.center_x = (float)graph.size_x / 2; graph.center_y = (float)graph.size_y / 2; }
//	// �g�p����摜�Ƃ��̕�����(x,y)���w�肷��B
//	const void SetGraph(int _graph, int cell_x, int cell_y) { graph.handle = _graph; graph.cell_x = cell_x; graph.cell_y = cell_y; }
//	// ���W��ݒ�
//	const void SetPosition(Vector2 pos) { graph.pos = pos; }
//	// �g�嗦��ݒ�
//	const void SetExtendRate(float ext) { graph.ext = ext; }
//	// ��]��ݒ�
//	const void SetAngle(float angle) { graph.angle = angle; }
//	// �P�x��ݒ�
//	const void SetBright(float r, float g, float b) { graph.bright.red = r, graph.bright.green = g, graph.bright.blue = b; }
//
//	/*-----------	�Q�b�^�[	-----------*/
//	// �摜���擾
//	const int GetGraph() const { return graph.handle; }
//	// �摜X���̑傫�����擾
//	const int GetGraphSize_X() const { return graph.size_x; }
//	// �摜Y���̑傫�����擾
//	const int GetGraphSize_Y() const { return graph.size_y; }
//	// �摜X���̑傫�����擾(�g�嗦�K�p)
//	const int GetGraphSize_X_EXT() const { return (int)(graph.size_x * graph.ext); }
//	// �摜Y���̑傫�����擾(�g�嗦�K�p)
//	const int GetGraphSize_Y_EXT() const { return (int)(graph.size_y * graph.ext); }
//	// X���Z��1�}�X�̑傫�����擾(�s�N�Z��)
//	const int GetCellSize_X() const { return graph.size_x / graph.cell_x; }
//	// Y���Z��1�}�X�̑傫�����擾(�s�N�Z��)
//	const int GetCellSize_Y() const { return graph.size_y / graph.cell_y; }
//	// �摜�̉�]���擾
//	const float GetAngle() const { return graph.angle; }
//	// �摜�̊g�嗦���擾
//	const float GetExtend() const { return graph.ext; }
//	// �摜�̍��W���擾
//	const Vector2 GetPosision() const { return graph.pos; }
//	// �摜�̋P�x���擾
//	const UI_Bright GetBright() const { return graph.bright; }
//
//	/*---�f�o�b�O---*/
//	void SetShowFlag(bool flag) { isShow = flag; }
//};