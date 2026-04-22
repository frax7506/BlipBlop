#include "stdafx.h"
#include "HD_HybridString.h"

#include "HD_StringUtils.h"

HD_HybridString::HD_HybridString()
	: myData(nullptr)
{
}

HD_HybridString::HD_HybridString(const char* aString)
{
	int length = HD_Strlen(aString);
	myData = new char[length + 1] { 0 };
	memcpy(myData, aString, length);
}

HD_HybridString::HD_HybridString(const wchar_t* aString)
{
	int length = HD_Strlen(aString);
	myData = new char[(sizeof(wchar_t) * length) + 1] { 0 };
	memcpy(myData, aString, sizeof(wchar_t) * length);
}

HD_HybridString::~HD_HybridString()
{
	HD_SafeDeleteArray(myData);
}

const char* HD_HybridString::GetBuffer() const
{
	assert(!GetIsWide() && "HD_HybridString::GetBuffer was called on a wide instance.");
	return myData;
}

const wchar_t* HD_HybridString::GetWBuffer() const
{
	assert(GetIsWide() && "HD_HybridString::GetWBuffer was called on a non-wide instance.");
	return reinterpret_cast<const wchar_t*>(myData);
}
