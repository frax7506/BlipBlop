#pragma once

// * This class is intended to have minimal life span, which is why its data is in a raw array
//		and not in a string object.
// * The intended use is for instance HD_Format("NPC name: %s", npc.GetName()).GetBuffer(),
//		using the GetBuffer method at the end. Saving a HD_Format object as a state doesn't make
//		much sense and in that case HD_String should be used instead, which can be initialized
//		from the buffer of a temporary HD_Format object.

#include "HD_String.h"

#include <cstdarg>
#include <cstring>
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
	T myFormattedString[HD_FORMAT_MAX_BUFFER_SIZE];
};

template<>
inline HD_Format<char>::HD_Format(const char* aFormat, ...)
{
	memset(myFormattedString, 0, HD_FORMAT_MAX_BUFFER_SIZE);

	va_list args;
	va_start(args, aFormat);
	vsprintf_s(myFormattedString, HD_FORMAT_MAX_BUFFER_SIZE, aFormat, args);
	va_end(args);
}

template<>
inline HD_Format<wchar_t>::HD_Format(const wchar_t* aFormat, ...)
{
	memset(myFormattedString, 0, HD_FORMAT_MAX_BUFFER_SIZE);

	va_list args;
	va_start(args, aFormat);
	vswprintf_s(myFormattedString, HD_FORMAT_MAX_BUFFER_SIZE, aFormat, args);
	va_end(args);
}

template<typename T>
inline const T* HD_Format<T>::GetBuffer() const
{
	return myFormattedString;
}
