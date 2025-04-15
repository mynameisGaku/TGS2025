#include "gauge.h"

// Ã“Iƒƒ“ƒo•Ï”‚Ì‰Šú‰»
int Gauge::currentId = 0;

Gauge::Gauge()
{
	id = GenerateId();  // ID‚ğ¶¬‚µ‚Äİ’è
}

int Gauge::GenerateId()
{
	return currentId++;  // V‚µ‚¢ID‚ğ¶¬
}

