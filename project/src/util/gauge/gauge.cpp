#include "src/util/gauge/gauge.h"

// 静的メンバ変数の初期化
int Gauge::currentId = 0;

Gauge::Gauge()
{
	id = GenerateId();  // IDを生成して設定
}

int Gauge::GenerateId()
{
	return currentId++;  // 新しいIDを生成
}

