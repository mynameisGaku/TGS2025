#include "MaterialRenderer.h"

// ”Ä—p
#include "../Object3D.h"

MaterialRenderer::MaterialRenderer() {

	parentObj = nullptr;

	duration = 0;
	durationMax = 0;
}

MaterialRenderer::~MaterialRenderer() {

	anotherMaterials.clear();
}

void MaterialRenderer::Init(Object3D* pObj, std::list<int> anotherModels) {

	parentObj = pObj;
	parentMaterial.hModel = pObj->Model();
	parentMaterial.Init();

	for (auto& itr : anotherModels) {
		anotherMaterials.push_back(MaterialData(itr, MV1GetMaterialNum(itr)));
		anotherMaterials.back().Init();
	}
}

void MaterialRenderer::Update() {

	if (durationMax > 0) {
		duration++;
		if (duration >= durationMax) {
			SetDuration(0);
			ClearMaterials();
		}
	}
}

void MaterialRenderer::Draw() {

}

void MaterialRenderer::ClearMaterials() {

	parentMaterial.Reset();
	parentMaterial.Apply();

	for (auto& itr : anotherMaterials) {
		itr.Reset();
		itr.Apply();
	}
}

void MaterialRenderer::SetMaterialDifColor(int index, COLOR_F color, int duration) {

	parentMaterial.curMaterial[index].difColor = color;
	MV1SetMaterialDifColor(parentMaterial.hModel, index, color);

	SetDuration(duration);

	for (auto& itr : anotherMaterials) {
		for (int i = 0; i < itr.materialIndex; i++) {
			itr.curMaterial[i].difColor = color;
			MV1SetMaterialDifColor(itr.hModel, i, color);
		}
	}
}

void MaterialRenderer::SetMaterialSpcColor(int index, COLOR_F color, int duration) {

	parentMaterial.curMaterial[index].spcColor = color;
	MV1SetMaterialSpcColor(parentMaterial.hModel, index, color);

	SetDuration(duration);

	for (auto& itr : anotherMaterials) {
		for (int i = 0; i < itr.materialIndex; i++) {
			itr.curMaterial[i].spcColor = color;
			MV1SetMaterialSpcColor(itr.hModel, i, color);
		}
	}
}

void MaterialRenderer::SetMaterialEmiColor(int index, COLOR_F color, int duration) {

	parentMaterial.curMaterial[index].emiColor = color;
	MV1SetMaterialEmiColor(parentMaterial.hModel, index, color);

	SetDuration(duration);

	for (auto& itr : anotherMaterials) {
		for (int i = 0; i < itr.materialIndex; i++) {
			itr.curMaterial[i].emiColor = color;
			MV1SetMaterialEmiColor(itr.hModel, i, color);
		}
	}
}

void MaterialRenderer::SetMaterialAmbColor(int index, COLOR_F color, int duration) {

	parentMaterial.curMaterial[index].ambColor = color;
	MV1SetMaterialAmbColor(parentMaterial.hModel, index, color);

	SetDuration(duration);

	for (auto& itr : anotherMaterials) {
		for (int i = 0; i < itr.materialIndex; i++) {
			itr.curMaterial[i].ambColor = color;
			MV1SetMaterialAmbColor(itr.hModel, i, color);
		}
	}
}

void MaterialRenderer::SetDuration(int duration) {

	this->duration = 0;
	durationMax = duration;
}
