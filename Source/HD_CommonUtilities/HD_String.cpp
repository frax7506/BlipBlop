#include "stdafx.h"
#include "HD_String.h"

HD_String::HD_String()
	: myData(nullptr)
	, myLength(0)
	, myCapacity(0)
{
}

HD_String::HD_String(const char* aString)
{
	myLength = HD_Strlen(aString);
	myCapacity = myLength + 1;
	myData = new char[myCapacity] { 0 };
	memcpy(myData, aString, myLength);
}

HD_String::HD_String(const HD_String& aString)
	: HD_String(aString.GetBuffer())
{
}

HD_String::HD_String(HD_String&& aString)
{
	myLength = aString.myLength;
	myCapacity = aString.myCapacity;
	myData = aString.myData;
	aString.myData = nullptr;
}

HD_String::~HD_String()
{
	HD_SafeDeleteArray(myData);
}

const char* HD_String::GetBuffer() const
{
	return myData;
}

int HD_String::GetLength() const
{
	return myLength;
}

char HD_String::GetCharAt(int aIndex) const
{
	assert(0 <= aIndex && aIndex < myLength);
	return myData[aIndex];
}

void HD_String::Append(const char* aString)
{
	int length = HD_Strlen(aString);
	CheckLengthAndGrowIfNecessary(length);

	memcpy(myData + myLength, aString, length);
	myLength += length;
}

void HD_String::Append(const HD_String& aString)
{
	Append(aString.GetBuffer());
}

HD_String HD_String::operator+(const char* aString) const
{
	HD_String result(*this);
	result.Append(aString);
	return result;
}

HD_String HD_String::operator+(const HD_String& aString) const
{
	return (*this) + aString.GetBuffer();
}

HD_String& HD_String::operator=(const char* aString)
{
	int length = HD_Strlen(aString);
	bool isCurrentBufferTooSmall = myCapacity < length + 1;

	if (isCurrentBufferTooSmall)
	{
		HD_SafeDeleteArray(myData);
		myCapacity = length + 1;
		myData = new char[myCapacity] { 0 };
	}

	myLength = length;
	memset(myData, 0, myCapacity);
	memcpy(myData, aString, myLength);

	return *this;
}

HD_String& HD_String::operator=(const HD_String& aString)
{
	*this = aString.GetBuffer();
	return *this;
}

HD_String& HD_String::operator=(HD_String&& aString)
{
	myLength = aString.myLength;
	myCapacity = aString.myCapacity;
	myData = aString.myData;
	aString.myData = nullptr;

	return *this;
}

bool HD_String::operator==(const char* aString) const
{
	return HD_Strcmp(myData, aString) == 0;
}

bool HD_String::operator==(const HD_String& aString) const
{
	return (*this) == aString.GetBuffer();
}

bool HD_String::operator!=(const char* aString) const
{
	return !((*this) == aString);
}

bool HD_String::operator!=(const HD_String& aString) const
{
	return !((*this) == aString.GetBuffer());
}

void HD_String::CheckLengthAndGrowIfNecessary(int anAdditionalLength)
{
	if (myLength + anAdditionalLength > myCapacity - 1)
	{
		int newLength = myLength + anAdditionalLength;
		int newCapacity = static_cast<int>(newLength * ourGrowFactor + 1);
		assert(newCapacity > myCapacity && "HD_String grew larger than int max.");
		Grow(newCapacity);
	}
}

void HD_String::Grow(int aNewCapacity)
{
	char* oldData = myData;
	myData = new char[aNewCapacity] { 0 };
	memcpy(myData, oldData, myLength);
	HD_SafeDeleteArray(oldData);
	myCapacity = aNewCapacity;
}

HD_String operator+(const char* aString1, const HD_String& aString2)
{
	HD_String result(aString1);
	result.Append(aString2);
	return result;
}
