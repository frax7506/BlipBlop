#pragma once

#include "HD_Math.h"
#include "HD_String.h"

#include <cassert>
#include <cstring>

template<int aSize>
class HD_Bitset
{
public:
	template<typename BitsetType>
	class BitRef
	{
	public:
		BitRef();
		BitRef(const BitRef& aOther);
		BitRef(BitsetType* aBitset, int aIndex);

		BitRef& operator=(const BitRef& aOther);
		BitRef& operator=(bool aValue);

		bool GetValue() const;
		operator bool() const;

	private:
		BitsetType* myBitset;
		int myIndex;
	};

	typedef BitRef<HD_Bitset> BitReference;
	typedef BitRef<const HD_Bitset> ConstBitReference;

public:
	static constexpr int GetNrOfBytesNeededForNrOfBits(int aNrOfBits) { return (aNrOfBits % 8) == 0 ? (aNrOfBits / 8) : (aNrOfBits / 8) + 1; }
	static constexpr int ourNrOfBytes = GetNrOfBytesNeededForNrOfBits(aSize);

	HD_Bitset();
	HD_Bitset(const HD_Bitset& aOther);
	HD_Bitset(unsigned long long aValue);

	void EnableAllBits();
	void DisableAllBits();
	void FlipAllBits();

	char* GetBuffer();
	const char* GetBuffer() const;

	const char* ToString() const;

	BitReference operator[](int aIndex);
	ConstBitReference operator[](int aIndex) const;

	HD_Bitset& operator&=(const HD_Bitset& aOther);
	HD_Bitset& operator|=(const HD_Bitset& aOther);
	HD_Bitset& operator^=(const HD_Bitset& aOther);

	HD_Bitset& operator<<=(int aValue);
	HD_Bitset& operator>>=(int aValue);

	HD_Bitset operator~() const;
	HD_Bitset operator<<(int aValue) const;
	HD_Bitset operator>>(int aValue) const;

private:

	char myBits[ourNrOfBytes];
};

template<int aSize>
HD_Bitset<aSize>::HD_Bitset()
{
	memset(&myBits, 0, ourNrOfBytes);
}

template<int aSize>
HD_Bitset<aSize>::HD_Bitset(const HD_Bitset& aOther)
{
	memcpy(&myBits, &aOther.myBits, ourNrOfBytes);
}

template<int aSize>
HD_Bitset<aSize>::HD_Bitset(unsigned long long aValue)
{
	int indexOfHighestBit = static_cast<int>(HD_Log2(static_cast<long double>(aValue)));
	assert(indexOfHighestBit <= (aSize - 1));
	int nrOfBytesToCopy = GetNrOfBytesNeededForNrOfBits(indexOfHighestBit + 1);

	memset(&myBits, 0, ourNrOfBytes);
	memcpy(&myBits, &aValue, nrOfBytesToCopy);
}

template<int aSize>
void HD_Bitset<aSize>::EnableAllBits()
{
	for (int i = 0; i < ourNrOfBytes; i++)
	{
		myBits[i] = static_cast<char>(0xFF);
	}
}

template<int aSize>
void HD_Bitset<aSize>::DisableAllBits()
{
	memset(myBits, 0, ourNrOfBytes);
}

template<int aSize>
void HD_Bitset<aSize>::FlipAllBits()
{
	for (int i = 0; i < ourNrOfBytes; i++)
	{
		myBits[i] = ~myBits[i];
	}
}

template<int aSize>
char* HD_Bitset<aSize>::GetBuffer()
{
	return myBits;
}

template<int aSize>
const char* HD_Bitset<aSize>::GetBuffer() const
{
	return myBits;
}

template<int aSize>
const char* HD_Bitset<aSize>::ToString() const
{
	HD_String string;
	for (int bitIndex = (ourNrOfBytes * 8) - 1; bitIndex >= 0; bitIndex--)
	{
		if ((*this)[bitIndex])
		{
			string.Append("1");
		}
		else
		{
			string.Append("0");
		}
	}

	return string.GetBuffer();
}

template<int aSize>
typename HD_Bitset<aSize>::BitReference HD_Bitset<aSize>::operator[](int aIndex)
{
	BitReference bitReference(this, aIndex);
	return bitReference;
}

template<int aSize>
typename HD_Bitset<aSize>::ConstBitReference HD_Bitset<aSize>::operator[](int aIndex) const
{
	ConstBitReference constBitReference(this, aIndex);
	return constBitReference;
}

template<int aSize>
HD_Bitset<aSize>& HD_Bitset<aSize>::operator&=(const HD_Bitset& aOther)
{
	for (int i = 0; i < ourNrOfBytes; i++)
	{
		myBits[i] &= aOther.myBits[i];
	}

	return *this;
}

template<int aSize>
HD_Bitset<aSize>& HD_Bitset<aSize>::operator|=(const HD_Bitset& aOther)
{
	for (int i = 0; i < ourNrOfBytes; i++)
	{
		myBits[i] |= aOther.myBits[i];
	}

	return *this;
}

template<int aSize>
HD_Bitset<aSize>& HD_Bitset<aSize>::operator^=(const HD_Bitset& aOther)
{
	for (int i = 0; i < ourNrOfBytes; i++)
	{
		myBits[i] ^= aOther.myBits[i];
	}

	return *this;
}

template<int aSize>
HD_Bitset<aSize>& HD_Bitset<aSize>::operator<<=(int aValue)
{
	for (int bitIndex = (ourNrOfBytes * 8) - 1; bitIndex >= 0; bitIndex--)
	{
		BitReference bitReferenceCurrent(this, bitIndex);

		if (bitIndex - aValue >= 0)
		{
			BitReference bitReferenceBitshift(this, bitIndex - aValue);
			bitReferenceCurrent = bitReferenceBitshift.GetValue();
		}
		else
		{
			bitReferenceCurrent = false;
		}
	}

	return *this;
}

template<int aSize>
HD_Bitset<aSize>& HD_Bitset<aSize>::operator>>=(int aValue)
{
	for (int bitIndex = 0; bitIndex < ourNrOfBytes * 8; bitIndex++)
	{
		BitReference bitReferenceCurrent(this, bitIndex);

		if (bitIndex + aValue < (ourNrOfBytes * 8))
		{
			BitReference bitReferenceBitshift(this, bitIndex + aValue);
			bitReferenceCurrent = bitReferenceBitshift.GetValue();
		}
		else
		{
			bitReferenceCurrent = false;
		}
	}

	return *this;
}

template<int aSize>
HD_Bitset<aSize> HD_Bitset<aSize>::operator~() const
{
	HD_Bitset result;

	for (int i = 0; i < ourNrOfBytes; i++)
	{
		result.myBits[i] = ~myBits[i];
	}

	return result;
}

template<int aSize>
HD_Bitset<aSize> HD_Bitset<aSize>::operator<<(int aValue) const
{
	HD_Bitset result = (*this);
	result <<= aValue;
	return result;
}

template<int aSize>
HD_Bitset<aSize> HD_Bitset<aSize>::operator>>(int aValue) const
{
	HD_Bitset result = (*this);
	result >>= aValue;
	return result;
}

template<int aSize>
template<typename BitsetType>
HD_Bitset<aSize>::BitRef<BitsetType>::BitRef()
	: myBitset(nullptr)
	, myIndex(0)
{
}

template<int aSize>
template<typename BitsetType>
HD_Bitset<aSize>::BitRef<BitsetType>::BitRef(const BitRef& aOther)
	: myBitset(aOther.myBitset)
	, myIndex(aOther.myIndex)
{
}

template<int aSize>
template<typename BitsetType>
HD_Bitset<aSize>::BitRef<BitsetType>::BitRef(BitsetType* aBitset, int aIndex)
	: myBitset(aBitset)
	, myIndex(aIndex)
{
}

template<int aSize>
template<typename BitsetType>
HD_Bitset<aSize>::BitRef<BitsetType>& HD_Bitset<aSize>::BitRef<BitsetType>::operator=(const BitRef& aOther)
{
	myBitset = aOther.myBitset;
	myIndex = aOther.myIndex;

	return *this;
}

template<int aSize>
template<typename BitsetType>
HD_Bitset<aSize>::BitRef<BitsetType>& HD_Bitset<aSize>::BitRef<BitsetType>::operator=(bool aValue)
{
	int byteIndex = myIndex / 8;
	int bitIndexInByte = myIndex % 8;
	char mask = static_cast<char>(HD_Pow(2, bitIndexInByte));

	if (aValue)
	{
		myBitset->myBits[byteIndex] |= mask;
	}
	else
	{
		myBitset->myBits[byteIndex] &= ~mask;
	}

	return *this;
}

template<int aSize>
template<typename BitsetType>
bool HD_Bitset<aSize>::BitRef<BitsetType>::GetValue() const
{
	int byteIndex = myIndex / 8;
	int bitIndexInByte = myIndex % 8;
	char mask = static_cast<char>(HD_Pow(2, bitIndexInByte));

	bool result = (myBitset->myBits[byteIndex] & mask) != 0;
	return result;
}

template<int aSize>
template<typename BitsetType>
HD_Bitset<aSize>::BitRef<BitsetType>::operator bool() const
{
	bool value = GetValue();
	return value;
}
