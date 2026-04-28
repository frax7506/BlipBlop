#pragma once

#include "HD_String.h"

#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>

#define HD_FORMAT_MAX_BUFFER_SIZE 512

template<typename T>
class HD_Format
{
public:
	inline HD_Format(const T* aFormat, ...);

	inline const T* GetBuffer() const;

private:
	HD_Str<T> myFormattedString;
};

template<>
inline HD_Format<char>::HD_Format(const char* aFormat, ...)
	: myFormattedString(HD_FORMAT_MAX_BUFFER_SIZE)
{
	va_list args;
	va_start(args, aFormat);
	vsprintf_s(myFormattedString.GetWritableBuffer(), HD_FORMAT_MAX_BUFFER_SIZE, aFormat, args);
	va_end(args);
}

template<>
inline HD_Format<wchar_t>::HD_Format(const wchar_t* aFormat, ...)
	: myFormattedString(HD_FORMAT_MAX_BUFFER_SIZE)
{
	va_list args;
	va_start(args, aFormat);
	vswprintf_s(myFormattedString.GetWritableBuffer(), HD_FORMAT_MAX_BUFFER_SIZE, aFormat, args);
	va_end(args);
}

template<typename T>
inline const T* HD_Format<T>::GetBuffer() const
{
	return myFormattedString.GetBuffer();
}
