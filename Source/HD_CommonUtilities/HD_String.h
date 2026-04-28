#pragma once

#include "HD_StringUtils.h"
#include "HD_Utilities.h"

#include <cassert>
#include <cstring>

template<typename T>
class HD_Str
{
public:
	HD_Str();
	HD_Str(int aInitialCapacity);
	HD_Str(const T* aString);
	HD_Str(const HD_Str& aString);
	HD_Str(HD_Str&& aString);
	~HD_Str();

	const T* GetBuffer() const;
	T* GetWritableBuffer();

	int GetLength() const;
	T GetCharAt(int aIndex) const;

	void Append(const T* aString);
	void Append(const HD_Str& aString);

	HD_Str operator+(const T* aString) const;
	HD_Str operator+(const HD_Str& aString) const;

	HD_Str& operator=(const T* aString);
	HD_Str& operator=(const HD_Str& aString);
	HD_Str& operator=(HD_Str&& aString);

	bool operator==(const T* aString) const;
	bool operator==(const HD_Str& aString) const;
	bool operator!=(const T* aString) const;
	bool operator!=(const HD_Str& aString) const;

private:
	static constexpr float ourGrowFactor = 1.5f;

	void CheckLengthAndGrowIfNecessary(int anAdditionalLength);
	void Grow(int aNewCapacity);

	T* myData;
	int myLength;
	int myCapacity;
};

// Global operator for doing <string literal> + aString.
template<typename T>
HD_Str<T> operator+(const T* aString1, const HD_Str<T>& aString2);

template<typename T>
HD_Str<T>::HD_Str()
	: myLength(0)
	, myCapacity(1)
{
	myData = new T[myCapacity] { 0 };
}

template<typename T>
HD_Str<T>::HD_Str(int aInitialCapacity)
	: myLength(0)
	, myCapacity(aInitialCapacity)
{
	myData = new T[myCapacity]{ 0 };
}

template<typename T>
HD_Str<T>::HD_Str(const T* aString)
{
	myLength = HD_Strlen(aString);
	myCapacity = myLength + 1;
	myData = new T[myCapacity] { 0 };
	memcpy(myData, aString, sizeof(T) * myLength);
}

template<typename T>
HD_Str<T>::HD_Str(const HD_Str& aString)
	: HD_Str(aString.GetBuffer())
{
}

template<typename T>
HD_Str<T>::HD_Str(HD_Str&& aString)
{
	myLength = aString.myLength;
	myCapacity = aString.myCapacity;
	myData = aString.myData;
	aString.myData = nullptr;
}

template<typename T>
HD_Str<T>::~HD_Str()
{
	HD_SafeDeleteArray(myData);
}

template<typename T>
const T* HD_Str<T>::GetBuffer() const
{
	return myData;
}

template<typename T>
T* HD_Str<T>::GetWritableBuffer()
{
	return myData;
}

template<typename T>
int HD_Str<T>::GetLength() const
{
	return myLength;
}

template<typename T>
T HD_Str<T>::GetCharAt(int aIndex) const
{
	assert(0 <= aIndex && aIndex < myLength);
	return myData[aIndex];
}

template<typename T>
void HD_Str<T>::Append(const T* aString)
{
	int length = HD_Strlen(aString);
	CheckLengthAndGrowIfNecessary(length);

	memcpy(myData + myLength, aString, sizeof(T) * length);
	myLength += length;
}

template<typename T>
void HD_Str<T>::Append(const HD_Str& aString)
{
	Append(aString.GetBuffer());
}

template<typename T>
HD_Str<T> HD_Str<T>::operator+(const T* aString) const
{
	HD_Str result(*this);
	result.Append(aString);
	return result;
}

template<typename T>
HD_Str<T> HD_Str<T>::operator+(const HD_Str& aString) const
{
	return (*this) + aString.GetBuffer();
}

template<typename T>
HD_Str<T>& HD_Str<T>::operator=(const T* aString)
{
	int length = HD_Strlen(aString);
	bool isCurrentBufferTooSmall = myCapacity < length + 1;

	if (isCurrentBufferTooSmall)
	{
		HD_SafeDeleteArray(myData);
		myCapacity = length + 1;
		myData = new T[myCapacity] { 0 };
	}

	myLength = length;
	memset(myData, 0, sizeof(T) * myCapacity);
	memcpy(myData, aString, sizeof(T) * myLength);

	return *this;
}

template<typename T>
HD_Str<T>& HD_Str<T>::operator=(const HD_Str& aString)
{
	*this = aString.GetBuffer();
	return *this;
}

template<typename T>
HD_Str<T>& HD_Str<T>::operator=(HD_Str&& aString)
{
	HD_SafeDeleteArray(myData);

	myLength = aString.myLength;
	myCapacity = aString.myCapacity;
	myData = aString.myData;
	aString.myData = nullptr;

	return *this;
}

template<typename T>
bool HD_Str<T>::operator==(const T* aString) const
{
	return HD_Strcmp(myData, aString) == 0;
}

template<typename T>
bool HD_Str<T>::operator==(const HD_Str& aString) const
{
	return (*this) == aString.GetBuffer();
}

template<typename T>
bool HD_Str<T>::operator!=(const T* aString) const
{
	return !((*this) == aString);
}

template<typename T>
bool HD_Str<T>::operator!=(const HD_Str& aString) const
{
	return !((*this) == aString.GetBuffer());
}

template<typename T>
void HD_Str<T>::CheckLengthAndGrowIfNecessary(int anAdditionalLength)
{
	if (myLength + anAdditionalLength > myCapacity - 1)
	{
		int newLength = myLength + anAdditionalLength;
		int newCapacity = static_cast<int>(newLength * ourGrowFactor + 1);
		assert(newCapacity > myCapacity && "HD_Str grew larger than int max.");
		Grow(newCapacity);
	}
}

template<typename T>
void HD_Str<T>::Grow(int aNewCapacity)
{
	T* oldData = myData;
	myData = new T[aNewCapacity] { 0 };
	memcpy(myData, oldData, sizeof(T) * myLength);
	HD_SafeDeleteArray(oldData);
	myCapacity = aNewCapacity;
}

template<typename T>
HD_Str<T> operator+(const T* aString1, const HD_Str<T>& aString2)
{
	HD_Str result(aString1);
	result.Append(aString2);
	return result;
}

typedef HD_Str<char> HD_String;
typedef HD_Str<wchar_t> HD_WString;
