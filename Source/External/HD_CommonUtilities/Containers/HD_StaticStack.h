#pragma once

#include <cassert>

template<typename T, int aSize>
class HD_StaticStack
{
public:
	HD_StaticStack();

	void Push(const T& aData);
	void Pop();
	const T& Top();

	void PopN(int aN);
	void MoveN(HD_StaticStack& aMoveFrom, int aN);

	int GetNrOfPushedElements();

private:
	T myData[aSize];
	int myNrOfPushedElements;
};

template<typename T, int aSize>
HD_StaticStack<T, aSize>::HD_StaticStack()
	: myNrOfPushedElements(0)
{
	for (int i = 0; i < aSize; ++i)
		myData[i] = T();
}

template<typename T, int aSize>
void HD_StaticStack<T, aSize>::Push(const T& aData)
{
	assert(myNrOfPushedElements < aSize);
	myData[myNrOfPushedElements] = aData;
	myNrOfPushedElements++;
}

template<typename T, int aSize>
void HD_StaticStack<T, aSize>::Pop()
{
	assert(myNrOfPushedElements > 0);
	myNrOfPushedElements--;
}

template<typename T, int aSize>
const T& HD_StaticStack<T, aSize>::Top()
{
	assert(myNrOfPushedElements > 0);
	return myData[myNrOfPushedElements - 1];
}

template<typename T, int aSize>
void HD_StaticStack<T, aSize>::PopN(int aN)
{
	assert(myNrOfPushedElements >= aN);
	myNrOfPushedElements -= aN;
}

template<typename T, int aSize>
void HD_StaticStack<T, aSize>::MoveN(HD_StaticStack& aMoveFrom, int aN)
{
	int moveFromNrOfPushedElements = aMoveFrom.GetNrOfPushedElements();
	assert(myNrOfPushedElements + aN <= aSize && moveFromNrOfPushedElements >= aN);

	for (int i = 0; i < aN; i++)
	{
		Push(aMoveFrom.myData[moveFromNrOfPushedElements - aN + i]);
		aMoveFrom.Pop();
	}
}

template<typename T, int aSize>
int HD_StaticStack<T, aSize>::GetNrOfPushedElements()
{
	return myNrOfPushedElements;
}
