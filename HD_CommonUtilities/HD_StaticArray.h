#pragma once

#include "HD_ArrayIterator.h"

#include <cassert>
#include <cstring>

template<typename T, int N>
class HD_StaticArray
{
public:
	typedef HD_ArrayIterator<T> Iterator;
	typedef HD_ArrayIterator<const T> ConstIterator;

	HD_StaticArray();
	HD_StaticArray(const HD_StaticArray& aStaticArray);

	int Size() const;

	T& operator[](int aIndex);
	const T& operator[](int aIndex) const;
	HD_StaticArray& operator=(const HD_StaticArray& aStaticArray);

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

private:
	T myData[N];
};

template<typename T, int N>
HD_StaticArray<T, N>::HD_StaticArray()
{
	for (int i = 0; i < N; ++i)
		myData[i] = T();
}

template<typename T, int N>
HD_StaticArray<T, N>::HD_StaticArray(const HD_StaticArray& aStaticArray)
{
	memcpy(myData, aStaticArray.myData, sizeof(T) * N);
}

template<typename T, int N>
int HD_StaticArray<T, N>::Size() const
{
	return N;
}

template<typename T, int N>
T& HD_StaticArray<T, N>::operator[](int aIndex)
{
	assert(0 <= aIndex && aIndex < N);
	return myData[aIndex];
}

template<typename T, int N>
const T& HD_StaticArray<T, N>::operator[](int aIndex) const
{
	assert(0 <= aIndex && aIndex < N);
	return myData[aIndex];
}

template<typename T, int N>
HD_StaticArray<T, N>& HD_StaticArray<T, N>::operator=(const HD_StaticArray& aStaticArray)
{
	memcpy(myData, aStaticArray.myData, sizeof(T) * N);
	return *this;
}

template<typename T, int N>
typename HD_StaticArray<T, N>::Iterator HD_StaticArray<T, N>::begin()
{
	return Iterator(myData);
}

template<typename T, int N>
typename HD_StaticArray<T, N>::Iterator HD_StaticArray<T, N>::end()
{
	return Iterator(myData + N);
}

template<typename T, int N>
typename HD_StaticArray<T, N>::ConstIterator HD_StaticArray<T, N>::begin() const
{
	return ConstIterator(myData);
}

template<typename T, int N>
typename HD_StaticArray<T, N>::ConstIterator HD_StaticArray<T, N>::end() const
{
	return ConstIterator(myData + N);
}
