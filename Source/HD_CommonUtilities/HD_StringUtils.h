#pragma once

#include <cstring>
#include <wchar.h>

template<typename T>
inline int HD_Strlen(const T* aString);

template<typename T>
inline int HD_Strcmp(const T* aString1, const T* aString2);

template<>
inline int HD_Strlen(const char* aString)
{
	return static_cast<int>(strlen(aString));
}

template<>
inline int HD_Strlen(const wchar_t* aString)
{
	return static_cast<int>(wcslen(aString));
}

template<>
inline int HD_Strcmp(const char* aString1, const char* aString2)
{
	return strcmp(aString1, aString2);
}

template<>
inline int HD_Strcmp(const wchar_t* aString1, const wchar_t* aString2)
{
	return wcscmp(aString1, aString2);
}
