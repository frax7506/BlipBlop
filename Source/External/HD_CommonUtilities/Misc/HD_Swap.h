#pragma once

#include "HD_IsFundamental.h"

template<typename T>
void HD_Swap(T& aLhs, T& aRhs)
{
	static_assert(HD_IsFundamental<T>::Result);

	T temp = aLhs;
	aLhs = aRhs;
	aRhs = temp;
}
