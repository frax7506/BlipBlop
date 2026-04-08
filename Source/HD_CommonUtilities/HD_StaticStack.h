#pragma once

#include <cassert>
#include <cstring>

template<typename T, int N>
class HD_StaticStack
{
public:
	HD_StaticStack();

	void Push(const T& aData);
	void Pop();
	const T& Top();

	void PopN(int aN);
	void MoveN(HD_StaticStack& aMoveFrom, int aN);

	int Size();

private:
	T myData[N];
	int mySize;
};

template<typename T, int N>
HD_StaticStack<T, N>::HD_StaticStack()
	: mySize(0)
{
	for (int i = 0; i < N; ++i)
		myData[i] = T();
}

template<typename T, int N>
void HD_StaticStack<T, N>::Push(const T& aData)
{
	assert(mySize < N);
	myData[mySize] = aData;
	++mySize;
}

template<typename T, int N>
void HD_StaticStack<T, N>::Pop()
{
	assert(mySize > 0);
	--mySize;
}

template<typename T, int N>
const T& HD_StaticStack<T, N>::Top()
{
	assert(mySize > 0);
	return myData[mySize - 1];
}

template<typename T, int N>
void HD_StaticStack<T, N>::PopN(int aN)
{
	assert(mySize >= aN);
	mySize -= aN;
}

template<typename T, int N>
void HD_StaticStack<T, N>::MoveN(HD_StaticStack& aMoveFrom, int aN)
{
	assert(mySize + aN <= N && aMoveFrom.Size() >= aN);
	memcpy(myData + mySize, aMoveFrom.myData + aMoveFrom.mySize - aN, sizeof(T) * aN);
	mySize += aN;
	aMoveFrom.PopN(aN);
}

template<typename T, int N>
int HD_StaticStack<T, N>::Size()
{
	return mySize;
}
