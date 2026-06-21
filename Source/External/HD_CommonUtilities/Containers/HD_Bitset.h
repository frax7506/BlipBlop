#pragma once

#include <cstring>

template<typename T>
class HD_Bitset
{
public:
	HD_Bitset();

	void EnableBits(T aBits);
	void DisableBits(T aBits);

	bool GetIsBitSet(T aBits) const;

	T GetBits() const;

private:
	T myBits;
};

template<typename T>
HD_Bitset<T>::HD_Bitset()
{
	memset(&myBits, 0, sizeof(T));
}

template<typename T>
void HD_Bitset<T>::EnableBits(T aBits)
{
	myBits |= aBits;
}

template<typename T>
void HD_Bitset<T>::DisableBits(T aBits)
{
	myBits &= ~aBits;
}

template<typename T>
bool HD_Bitset<T>::GetIsBitSet(T aBits) const
{
	return (myBits & aBits) != 0;
}

template<typename T>
T HD_Bitset<T>::GetBits() const
{
	return myBits;
}

typedef HD_Bitset<unsigned char> HD_Bitset8;
typedef HD_Bitset<unsigned short> HD_Bitset16;
typedef HD_Bitset<unsigned int> HD_Bitset32;
typedef HD_Bitset<unsigned long long> HD_Bitset64;
