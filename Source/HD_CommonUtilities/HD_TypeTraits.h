#pragma once

template<typename T, T v>
struct HD_Integral_Constant
{
	static constexpr T Value = v;
};

typedef HD_Integral_Constant<bool, true> HD_TrueType;
typedef HD_Integral_Constant<bool, false> HD_FalseType;
