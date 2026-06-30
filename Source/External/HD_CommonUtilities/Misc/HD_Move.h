#pragma once

#include "HD_TypeTraits.h"

template<typename T> struct HD_RemoveReference { typedef T Type; };
template<typename T> struct HD_RemoveReference<T&> { typedef T Type; };
template<typename T> struct HD_RemoveReference<T&&> { typedef T Type; };

template<typename T>
using HD_RemoveReference_Type = typename HD_RemoveReference<T>::Type;

template<typename T> struct HD_Is_lValue_Reference : HD_FalseType {};
template<typename T> struct HD_Is_lValue_Reference<T&> : HD_TrueType {};

template<typename T>
constexpr bool HD_Is_lValue_Reference_Value = HD_Is_lValue_Reference<T>::Value;

template<typename T>
constexpr HD_RemoveReference_Type<T>&& HD_Move(T&& aArg) noexcept
{
	return static_cast<HD_RemoveReference_Type<T>&&>(aArg);
}

template<typename T>
constexpr T&& HD_Forward(HD_RemoveReference_Type<T>& aArg) noexcept
{
	return static_cast<T&&>(aArg);
}

template<typename T>
constexpr T&& HD_Forward(HD_RemoveReference_Type<T>&& aArg) noexcept
{
	static_assert(!HD_Is_lValue_Reference_Value<T>, "Can't forward an rvalue as an lvalue");
	return static_cast<T&&>(aArg);
}
