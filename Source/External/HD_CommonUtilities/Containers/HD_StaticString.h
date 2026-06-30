#pragma once

#include "HD_StringUtils.h"

#include <cassert>
#include <cstring>

template<typename T, int aCapacity>
class HD_StaticStr
{
public:
	HD_StaticStr();
	HD_StaticStr(const T* aString);
	HD_StaticStr(const HD_StaticStr& aString);

	const T* GetBuffer() const;
	T* GetBufferWritable();

	int GetLength() const;
	int GetCapacity() const;

	T GetCharAt(int aIndex) const;

	void Append(const T* aString);
	void Append(const HD_StaticStr& aString);

	void Clear();

	HD_StaticStr& operator=(const T* aString);
	HD_StaticStr& operator=(const HD_StaticStr& aString);

private:
	T myString[aCapacity];
};

template<typename T, int aCapacity> HD_StaticStr<T, aCapacity> operator+(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2);
template<typename T, int aCapacity> HD_StaticStr<T, aCapacity> operator+(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2);
template<typename T, int aCapacity> HD_StaticStr<T, aCapacity> operator+(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2);

template<typename T, int aCapacity> bool operator==(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2);
template<typename T, int aCapacity> bool operator==(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2);
template<typename T, int aCapacity> bool operator==(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2);

template<typename T, int aCapacity> bool operator!=(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2);
template<typename T, int aCapacity> bool operator!=(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2);
template<typename T, int aCapacity> bool operator!=(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2);

template<typename T, int aCapacity> bool operator<(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2);
template<typename T, int aCapacity> bool operator<(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2);
template<typename T, int aCapacity> bool operator<(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2);

template<typename T, int aCapacity> bool operator>(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2);
template<typename T, int aCapacity> bool operator>(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2);
template<typename T, int aCapacity> bool operator>(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2);

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity>::HD_StaticStr()
{
	memset(myString, 0, aCapacity * sizeof(T));
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity>::HD_StaticStr(const T* aString)
{
	int length = HD_Strlen(aString);
	assert(length < aCapacity);
	memset(myString, 0, aCapacity * sizeof(T));
	memcpy(myString, aString, length * sizeof(T));
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity>::HD_StaticStr(const HD_StaticStr& aString)
	: HD_StaticStr(aString.GetBuffer())
{
}

template<typename T, int aCapacity>
const T* HD_StaticStr<T, aCapacity>::GetBuffer() const
{
	return myString;
}

template<typename T, int aCapacity>
T* HD_StaticStr<T, aCapacity>::GetBufferWritable()
{
	return myString;
}

template<typename T, int aCapacity>
int HD_StaticStr<T, aCapacity>::GetLength() const
{
	return HD_Strlen(myString);
}

template<typename T, int aCapacity>
int HD_StaticStr<T, aCapacity>::GetCapacity() const
{
	return aCapacity;
}

template<typename T, int aCapacity>
T HD_StaticStr<T, aCapacity>::GetCharAt(int aIndex) const
{
	assert(0 <= aIndex && aIndex < aCapacity - 1);
	return myString[aIndex];
}

template<typename T, int aCapacity>
void HD_StaticStr<T, aCapacity>::Append(const T* aString)
{
	int currentLength = GetLength();
	int additionalLength = HD_Strlen(aString);
	assert(currentLength + additionalLength < aCapacity);
	memcpy(myString + currentLength, aString, additionalLength * sizeof(T));
}

template<typename T, int aCapacity>
void HD_StaticStr<T, aCapacity>::Append(const HD_StaticStr& aString)
{
	Append(aString.GetBuffer());
}

template<typename T, int aCapacity>
void HD_StaticStr<T, aCapacity>::Clear()
{
	memset(myString, 0, aCapacity * sizeof(T));
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity>& HD_StaticStr<T, aCapacity>::operator=(const T* aString)
{
	int length = HD_Strlen(aString);
	assert(length < aCapacity);
	memset(myString, 0, aCapacity * sizeof(T));
	memcpy(myString, aString, length * sizeof(T));

	return *this;
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity>& HD_StaticStr<T, aCapacity>::operator=(const HD_StaticStr& aString)
{
	return (*this) = aString.GetBuffer();
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity> operator+(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	assert(aString1.GetLength() + aString2.GetLength() < aCapacity);

	HD_StaticStr<T, aCapacity> result = aString1;
	result.Append(aString2);
	return result;
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity> operator+(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2)
{
	assert(aString1.GetLength() + HD_Strlen(aString2) < aCapacity);

	HD_StaticStr<T, aCapacity> result = aString1;
	result.Append(aString2);
	return result;
}

template<typename T, int aCapacity>
HD_StaticStr<T, aCapacity> operator+(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	assert(HD_Strlen(aString1) + aString2.GetLength() < aCapacity);

	HD_StaticStr<T, aCapacity> result = aString1;
	result.Append(aString2);
	return result;
}

template<typename T, int aCapacity>
bool operator==(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return aString1 == aString2.GetBuffer();
}

template<typename T, int aCapacity>
bool operator==(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2)
{
	if (aString2)
	{
		return HD_Strcmp(aString1.GetBuffer(), aString2) == 0;
	}
	else
	{
		return aString1.GetLength() == 0;
	}
}

template<typename T, int aCapacity>
bool operator==(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return aString2 == aString1;
}

template<typename T, int aCapacity>
bool operator!=(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return !(aString1 == aString2);
}

template<typename T, int aCapacity>
bool operator!=(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2)
{
	return !(aString1 == aString2);
}

template<typename T, int aCapacity>
bool operator!=(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return !(aString1 == aString2);
}

template<typename T, int aCapacity>
bool operator<(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return HD_Strcmp(aString1.GetBuffer(), aString2.GetBuffer()) < 0;
}

template<typename T, int aCapacity>
bool operator<(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2)
{
	return HD_Strcmp(aString1.GetBuffer(), aString2) < 0;
}

template<typename T, int aCapacity>
bool operator<(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return HD_Strcmp(aString1, aString2.GetBuffer()) < 0;
}

template<typename T, int aCapacity>
bool operator>(const HD_StaticStr<T, aCapacity>& aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return HD_Strcmp(aString1.GetBuffer(), aString2.GetBuffer()) > 0;
}

template<typename T, int aCapacity>
bool operator>(const HD_StaticStr<T, aCapacity>& aString1, const T* aString2)
{
	return HD_Strcmp(aString1.GetBuffer(), aString2) > 0;
}

template<typename T, int aCapacity>
bool operator>(const T* aString1, const HD_StaticStr<T, aCapacity>& aString2)
{
	return HD_Strcmp(aString1, aString2.GetBuffer()) > 0;
}

template<int aCapacity> using HD_StaticString = HD_StaticStr<char, aCapacity>;
template<int aCapacity> using HD_StaticWString = HD_StaticStr<wchar_t, aCapacity>;
