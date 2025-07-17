#pragma once

#include <math.h>

class Asm
{
public:

	/// sqrt
	static inline float sqrt(float r)
	{
#if defined(_MSC_VER) && !defined(_WIN64)
		__asm
		{
			fld		r
			fsqrt
			fstp	r
		}

		return r;

#else
		return ::sqrtf(r);
#endif
	}

	/// sincos
	static inline void sincos(float r_, float* sin_, float* cos_)
	{
#if defined(_MSC_VER) && !defined(_WIN64)
		float ts, tc;

		__asm
		{
			fld		r_
			fsincos
			fstp	tc
			fstp	ts
		}

		*sin_ = ts;
		*cos_ = tc;
#else
		* sin_ = sinf(r_);
		*cos_ = cosf(r_);
#endif
	}
};
