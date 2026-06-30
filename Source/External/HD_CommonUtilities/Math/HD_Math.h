#pragma once

#include <math.h>

#define F_SQRT_TWO 1.4142135623f
#define F_ONE_OVER_SQRT_TWO 0.7071067811f
#define F_SMALL 0.0001f

#define F_PI 3.1415926535f
#define F_PI_HALF (F_PI / 2.f)

#define DEG_TO_RAD(a) (a * (F_PI / 180.f))
#define RAD_TO_DEG(a) (a * (180.f / F_PI))

#define HD_Max(a, b) ((a) > (b) ? (a) : (b))
#define HD_Min(a, b) ((a) < (b) ? (a) : (b))
#define HD_Abs(a) ((a) < 0 ? (-1 * (a)) : (a))

#define HD_ARE_FLOAT_VALUES_CLOSE(a, b) (HD_Abs(a - b) < F_SMALL)

template<typename T> inline T HD_Sqrt(T aValue);
template<typename T> inline T HD_Pow(T aBase, T aExponent);

template<typename T> inline T HD_Log2(T aValue);

template<typename T> inline T HD_Sin(T aAngleInRadians);
template<typename T> inline T HD_Cos(T aAngleInRadians);
template<typename T> inline T HD_Tan(T aAngleInRadians);

template<typename T> inline T HD_ArcSin(T aAngleInRadians);
template<typename T> inline T HD_ArcCos(T aAngleInRadians);
template<typename T> inline T HD_ArcTan(T aAngleInRadians);
template<typename T> inline T HD_ArcTan2(T aAngleYInRadians, T aAngleXInRadians);

template<typename T> inline T HD_Remap(T aValue, T aOldMin, T aOldMax, T aNewMin, T aNewMax);
template<typename T> inline T HD_RemapClamped(T aValue, T aOldMin, T aOldMax, T aNewMin, T aNewMax);

template<>
inline float HD_Sqrt(float aValue)
{
	return sqrtf(aValue);
}

template<>
inline double HD_Sqrt(double aValue)
{
	return sqrt(aValue);
}

template<>
inline long double HD_Sqrt(long double aValue)
{
	return sqrtl(aValue);
}

template<>
inline int HD_Pow(int aBase, int aExponent)
{
	return static_cast<int>(powf(static_cast<float>(aBase), static_cast<float>(aExponent)));
}

template<>
inline float HD_Pow(float aBase, float aExponent)
{
	return powf(aBase, aExponent);
}

template<>
inline double HD_Pow(double aBase, double aExponent)
{
	return pow(aBase, aExponent);
}

template<>
inline long double HD_Pow(long double aBase, long double aExponent)
{
	return powl(aBase, aExponent);
}

template<>
inline float HD_Log2(float aValue)
{
	return log2f(aValue);
}

template<>
inline double HD_Log2(double aValue)
{
	return log2(aValue);
}

template<>
inline long double HD_Log2(long double aValue)
{
	return log2l(aValue);
}

template<>
inline float HD_Sin(float aAngleInRadians)
{
	return sinf(aAngleInRadians);
}

template<>
inline double HD_Sin(double aAngleInRadians)
{
	return sin(aAngleInRadians);
}

template<>
inline long double HD_Sin(long double aAngleInRadians)
{
	return sinl(aAngleInRadians);
}

template<>
inline float HD_Cos(float aAngleInRadians)
{
	return cosf(aAngleInRadians);
}

template<>
inline double HD_Cos(double aAngleInRadians)
{
	return cos(aAngleInRadians);
}

template<>
inline long double HD_Cos(long double aAngleInRadians)
{
	return cosl(aAngleInRadians);
}

template<>
inline float HD_Tan(float aAngleInRadians)
{
	return tanf(aAngleInRadians);
}

template<>
inline double HD_Tan(double aAngleInRadians)
{
	return tan(aAngleInRadians);
}

template<>
inline long double HD_Tan(long double aAngleInRadians)
{
	return tanl(aAngleInRadians);
}

template<>
inline float HD_ArcSin(float aAngleInRadians)
{
	return asinf(aAngleInRadians);
}

template<>
inline double HD_ArcSin(double aAngleInRadians)
{
	return asin(aAngleInRadians);
}

template<>
inline long double HD_ArcSin(long double aAngleInRadians)
{
	return asinl(aAngleInRadians);
}

template<>
inline float HD_ArcCos(float aAngleInRadians)
{
	return acosf(aAngleInRadians);
}

template<>
inline double HD_ArcCos(double aAngleInRadians)
{
	return acos(aAngleInRadians);
}

template<>
inline long double HD_ArcCos(long double aAngleInRadians)
{
	return acosl(aAngleInRadians);
}

template<>
inline float HD_ArcTan(float aAngleInRadians)
{
	return atanf(aAngleInRadians);
}

template<>
inline double HD_ArcTan(double aAngleInRadians)
{
	return atan(aAngleInRadians);
}

template<>
inline long double HD_ArcTan(long double aAngleInRadians)
{
	return atanl(aAngleInRadians);
}

template<>
inline float HD_ArcTan2(float aAngleYInRadians, float aAngleXInRadians)
{
	return atan2f(aAngleYInRadians, aAngleXInRadians);
}

template<>
inline double HD_ArcTan2(double aAngleYInRadians, double aAngleXInRadians)
{
	return atan2(aAngleYInRadians, aAngleXInRadians);
}

template<>
inline long double HD_ArcTan2(long double aAngleYInRadians, long double aAngleXInRadians)
{
	return atan2l(aAngleYInRadians, aAngleXInRadians);
}

template<typename T>
inline T HD_Remap(T aValue, T aOldMin, T aOldMax, T aNewMin, T aNewMax)
{
	return (((aValue - aOldMin) / (aOldMax - aOldMin)) * (aNewMax - aNewMin)) + aNewMin;
}

template<typename T>
inline T HD_RemapClamped(T aValue, T aOldMin, T aOldMax, T aNewMin, T aNewMax)
{
	if (aValue < aOldMin)
	{
		return aNewMin;
	}

	if (aValue > aOldMax)
	{
		return aNewMax;
	}

	return HD_Remap(aValue, aOldMin, aOldMax, aNewMin, aNewMax);
}
