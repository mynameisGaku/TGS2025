#pragma once
#include "src/util/ui/UI_Canvas.h"
#include "src/util/gauge/gauge.h"

class UI_CrossHair : public UI_Canvas {
public:
	UI_CrossHair(const RectTransform& trs);
	~UI_CrossHair();

	void Update() override;
	void Draw() override;

	void SetHandle_CrossHair(const std::string& path);
	void SetHandle_CrossHairFrame(const std::string& path);
	
	void SetHandle_CrossHairOutSide(const std::string& path);
	void SetHandle_CrossHairOutSideBack(const std::string& path);

	/// <summary>
	/// �X�N���[����ݒ肵�܂��B
	/// </summary>
	/// <param name="value">�Q�Ƃ���l�ւ̃|�C���^�B</param>
	/// <param name="min">�l�̍ŏ��l</param>
	/// <param name="max">�l�̍ő�l</param>
	/// <param name="scroll">�X�N���[���̎�ނ��w�肷�� Gauge::ScrollType �񋓑�</param>
	/// <param name="isDispMode">�\�����[�h��ύX����t���O�Btrue�F���Z���ɃX�N���[���Bfalse�F���Z���ɃX�N���[���B</param>
	void SetScroll(float* value, float min, float max, Gauge::ScrollType scroll, bool isDispMode = true);

private:
	Gauge gauge;
	Gauge::ScrollType scrollType;
	float* value;
	float valueMin;
	float valueMax;
	bool isDispMode;

	int hCrossHair;
	int hCrossHairFrame;

	int hCrossHairOutSide;
	int hCrossHairOutSideBack;
};