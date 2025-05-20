#pragma once

// Åûåpè≥å≥
#include "src/common/component/Component.h"

// Åûîƒóp
#include <DxLib.h>
#include <list>
#include <unordered_map>

class Object3D;

class MaterialColors {
public:
	COLOR_F difColor;
	COLOR_F spcColor;
	COLOR_F emiColor;
	COLOR_F ambColor;

	MaterialColors() :
		difColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f)),
		spcColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f)),
		emiColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f)),
		ambColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f))
	{
	}

	~MaterialColors() {}
};

class MaterialData {
public:
	int hModel;
	int materialIndex;
	std::unordered_map<int, MaterialColors> curMaterial;
	std::unordered_map<int, MaterialColors> defMaterial;

	MaterialData() :
		hModel(-1),
		materialIndex(0)
	{
	}

	MaterialData(int model, int index) :
		hModel(model),
		materialIndex(index)
	{
	}

	inline void Init() {

		if (hModel == -1)
			return;

		materialIndex = MV1GetMaterialNum(hModel);

		for (int i = 0; i < materialIndex; i++) {
			defMaterial[i].difColor = MV1GetMaterialDifColor(hModel, i);
			defMaterial[i].spcColor = MV1GetMaterialSpcColor(hModel, i);
			defMaterial[i].emiColor = MV1GetMaterialEmiColor(hModel, i);
			defMaterial[i].ambColor = MV1GetMaterialAmbColor(hModel, i);
		}
	}

	inline void Reset() {

		if (hModel == -1)
			return;

		if (materialIndex <= 0)
			materialIndex = MV1GetMaterialNum(hModel);

		for (int i = 0; i < materialIndex; i++) {
			curMaterial[i].difColor = defMaterial[i].difColor;
			curMaterial[i].spcColor = defMaterial[i].spcColor;
			curMaterial[i].emiColor = defMaterial[i].emiColor;
			curMaterial[i].ambColor = defMaterial[i].ambColor;
		}
	}

	inline void Apply() {

		if (hModel == -1)
			return;

		if (materialIndex <= 0)
			materialIndex = MV1GetMaterialNum(hModel);

		for (int i = 0; i < materialIndex; i++) {
			MV1SetMaterialDifColor(hModel, i, curMaterial[i].difColor);
			MV1SetMaterialSpcColor(hModel, i, curMaterial[i].spcColor);
			MV1SetMaterialEmiColor(hModel, i, curMaterial[i].emiColor);
			MV1SetMaterialAmbColor(hModel, i, curMaterial[i].ambColor);
		}
	}
};

class MaterialRenderer : public Component {
public:

	MaterialRenderer();
	~MaterialRenderer();

	void Init(Object3D* parent, std::list<int> anotherModels = {});
	void Update() override;
	void Draw() override;

	void SetMaterialDifColor(int materialIndex, COLOR_F color, int duration);
	void SetMaterialSpcColor(int materialIndex, COLOR_F color, int duration);
	void SetMaterialEmiColor(int materialIndex, COLOR_F color, int duration);
	void SetMaterialAmbColor(int materialIndex, COLOR_F color, int duration);

	void SetDuration(int duration);
	void ClearMaterials();

private:

	Object3D* parentObj;
	MaterialData parentMaterial;
	std::list<MaterialData> anotherMaterials;

	int duration;
	int durationMax;
};