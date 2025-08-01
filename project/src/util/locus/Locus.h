#pragma once
#include "src/util/object3D/Object3D.h"
#include <vector>

class Locus : public Object3D {
public:
	Locus();
	~Locus();
	void Update() override;
	void Draw() override;

	void SetGraph(std::string filename);
	void SetPosition(VECTOR root, VECTOR tip);

private:
	int hImage;
	static const int KEEP_NUM = 5; // 軌跡を取るフレーム数
	
	struct POSITION {
		int counter; // ここが呼ばれてからの経過時間
		VECTOR root;
		VECTOR tip;
	};
	std::vector<POSITION> buf;
	
	void MakeDiv(std::vector<VERTEX3D>& vs, VECTOR rPos[4], VECTOR tPos[4], float v1, float v2);
};
