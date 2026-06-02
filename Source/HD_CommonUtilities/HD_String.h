#pragma once

#include "HD_SafeDelete.h"
#include "HD_StringUtils.h"

#include <cassert>
#include <cstring>

template<typename T>
class HD_Str
{
public:
	HD_Str();
	HD_Str(const T* aString);
	HD_Str(const HD_Str& aString);
	HD_Str(HD_Str&& aString);
	~HD_Str();

	const T* GetBuffer() const;
	T* GetBufferWritable();

	int GetLength() const;
	int GetCapacity() const;

	T GetCharAt(int aIndex) const;

	void Append(const T* aString);
	void Append(const HD_Str& aString);

	void Clear();

	HD_Str operator+(const T* aString) const;
	HD_Str operator+(const HD_Str& aString) const;

	HD_Str& operator=(const T* aString);
	HD_Str& operator=(const HD_Str& aString);
	HD_Str& operator=(HD_Str&& aString);

	bool operator==(const T* aString) const;
	bool operator==(const HD_Str& aString) const;
	bool operator!=(const T* aString) const;
	bool operator!=(const HD_Str& aString) const;

	bool operator<(const T* aString) const;
	bool operator<(const HD_Str& aString) const;
	bool operator>(const T* aString) const;
	bool operator>(const HD_Str& aString) const;

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
	: myData(nullptr)
	, myLength(0)
	, myCapacity(0)
{
}

template<typename T>
HD_Str<T>::HD_Str(const T* aString)
	: myData(nullptr)
	, myLength(0)
	, myCapacity(0)
{
	if (aString)
	{
		myLength = HD_Strlen(aString);
		myCapacity = myLength + 1;
		myData = new T[myCapacity] { 0 };
		memcpy(myData, aString, myLength * sizeof(T));
	}
}

template<typename T>
HD_Str<T>::HD_Str(const HD_Str& aString)
	: HD_Str(aString.GetBuffer())
{
}

template<typename T>
HD_Str<T>::HD_Str(HD_Str&& aString)
{
	myData = aString.myData;
	myLength = aString.myLength;
	myCapacity = aString.myCapacity;

	aString.myData = nullptr;
	aString.myLength = 0;
	aString.myCapacity = 0;
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
T* HD_Str<T>::GetBufferWritable()
{
	return myData;
}

template<typename T>
int HD_Str<T>::GetLength() const
{
	return myLength;
}

template<typename T>
int HD_Str<T>::GetCapacity() const
{
	return myCapacity;
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

	memcpy(myData + myLength, aString, length * sizeof(T));
	myLength += length;
}

template<typename T>
void HD_Str<T>::Append(const HD_Str& aString)
{
	Append(aString.GetBuffer());
}

template<typename T>
void HD_Str<T>::Clear()
{
	myLength = 0;
	memset(myData, 0, myCapacity * sizeof(T));
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
	if (!aString || (*aString) == 0)
	{
		Clear();
	}
	else
	{
		int length = HD_Strlen(aString);
		bool isCurrentBufferTooSmall = length + 1 > myCapacity;

		if (isCurrentBufferTooSmall)
		{
			Grow(length + 1);
		}

		myLength = length;
		memset(myData, 0, myCapacity * sizeof(T));
		memcpy(myData, aString, myLength * sizeof(T));
	}

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

	myData = aString.myData;
	myLength = aString.myLength;
	myCapacity = aString.myCapacity;

	aString.myData = nullptr;
	aString.myLength = 0;
	aString.myCapacity = 0;

	return *this;
}

template<typename T>
bool HD_Str<T>::operator==(const T* aString) const
{
	if (myData && aString)
	{
		return HD_Strcmp(myData, aString) == 0;
	}
	else if (!myData)
	{
		return (*aString) == 0;
	}
	else
	{
		return false;
	}
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
bool HD_Str<T>::operator<(const T* aString) const
{
	return HD_Strcmp(myData, aString) < 0;
}

template<typename T>
bool HD_Str<T>::operator<(const HD_Str& aString) const
{
	return (*this) < aString.myData;
}

template<typename T>
bool HD_Str<T>::operator>(const T* aString) const
{
	return HD_Strcmp(myData, aString) > 0;
}

template<typename T>
bool HD_Str<T>::operator>(const HD_Str& aString) const
{
	return (*this) > aString.myData;
}

template<typename T>
void HD_Str<T>::CheckLengthAndGrowIfNecessary(int anAdditionalLength)
{
	if (myLength + anAdditionalLength > myCapacity - 1)
	{
		int newLength = myLength + anAdditionalLength;
		int newCapacity = static_cast<int>(newLength * ourGrowFactor + 1);
		Grow(newCapacity);
	}
}

template<typename T>
void HD_Str<T>::Grow(int aNewCapacity)
{
	if (myData)
	{
		T* oldData = myData;
		myData = new T[aNewCapacity]{ 0 };
		memcpy(myData, oldData, myLength * sizeof(T));
		HD_SafeDeleteArray(oldData);
		myCapacity = aNewCapacity;
	}
	else
	{
		myData = new T[aNewCapacity]{ 0 };
		myCapacity = aNewCapacity;
	}
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
