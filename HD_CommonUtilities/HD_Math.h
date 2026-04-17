#pragma once

#include <math.h>

#define F_ONE_OVER_SQRT_TWO 0.7071067811f
#define F_SMALL 0.0001f

#define F_PI 3.1415926535f
#define F_PI_HALF (F_PI / 2.f)

#define DEG_TO_RAD(a) (a * (F_PI / 180.f))
#define RAD_TO_DEG(a) (a * (180.f / F_PI))

#define HD_Max(a, b) ((a) > (b) ? (a) : (b))
#define HD_Min(a, b) ((a) < (b) ? (a) : (b))

#define HD_Abs(a) ((a) < 0 ? (-1 * (a)) : (a))
#define HD_Sqrtf(arg) sqrtf(arg)
#define HD_Powf(a, b) powf(a, b)
#define HD_Pow(a, b) static_cast<int>(powf(static_cast<float>(a), static_cast<float>(b)))

#define HD_Sinf(a) sinf(a)
#define HD_Cosf(a) cosf(a)
#define HD_Tanf(a) tanf(a)

#define HD_ArcSinf(a) asinf(a)
#define HD_ArcCosf(a) acosf(a)
#define HD_ArcTanf(a) atanf(a)

namespace HD_Math
{
	inline bool AreFloatValuesClose(float aFirst, float aSecond)
	{
		float diff = HD_Abs(aFirst - aSecond);
		return diff < F_SMALL;
	}
}
