#pragma once

#include "HD_ArrayIterator.h"
#include "HD_Utilities.h"

#include <cassert>
#include <cstring>

template<typename T>
class HD_GrowingArray
{
public:
	typedef HD_ArrayIterator<T> Iterator;
	typedef HD_ArrayIterator<const T> ConstIterator;

	HD_GrowingArray();
	HD_GrowingArray(int aCapacity);
	HD_GrowingArray(const HD_GrowingArray& aGrowingArray);
	HD_GrowingArray(HD_GrowingArray&& aGrowingArray);
	~HD_GrowingArray();

	void PushBack(const T& aItem);
	void PushBack(T&& aItem);
	void InsertSorted(const T& aItem);
	void InsertSortedReverse(const T& aItem);

	void Remove(int aIndex);
	void RemoveCyclic(int aIndex);
	void RemoveAll();
	void Delete(int aIndex);
	void DeleteCyclic(int aIndex);
	void DeleteAll();

	int GetSize() const;
	bool GetIsEmpty() const;

	void Reserve(int aCapacity);
	void Resize(int aSize);

	void Swap(HD_GrowingArray& aOther);

	T& operator[](int aIndex);
	const T& operator[](int aIndex) const;

	HD_GrowingArray& operator=(const HD_GrowingArray& aGrowingArray);
	HD_GrowingArray& operator=(HD_GrowingArray&& aGrowingArray);

	T& GetFirst();
	T& GetLast();

	const T& GetFirst() const;
	const T& GetLast() const;

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

private:
	static constexpr float ourGrowFactor = 1.5;

	void CheckSizeAndGrowIfNecessary();
	void Grow(int aNewCapacity);
	void InsertAtIndex(const T& aItem, int aIndex);

	T* myData;
	int mySize;
	int myCapacity;
};

template<typename T>
HD_GrowingArray<T>::HD_GrowingArray()
	: mySize(0)
	, myCapacity(2)
{
	myData = new T[myCapacity];
}

template<typename T>
HD_GrowingArray<T>::HD_GrowingArray(int aCapacity)
	: mySize(0)
	, myCapacity(aCapacity)
{
	myData = new T[myCapacity];
}

template<typename T>
HD_GrowingArray<T>::HD_GrowingArray(const HD_GrowingArray& aGrowingArray)
{
	myData = new T[aGrowingArray.myCapacity];
	memcpy(myData, aGrowingArray.myData, sizeof(T) * aGrowingArray.mySize);
	mySize = aGrowingArray.mySize;
	myCapacity = aGrowingArray.myCapacity;
}

template<typename T>
HD_GrowingArray<T>::HD_GrowingArray(HD_GrowingArray&& aGrowingArray)
{
	myData = aGrowingArray.myData;
	aGrowingArray.myData = nullptr;
	mySize = aGrowingArray.mySize;
	myCapacity = aGrowingArray.myCapacity;
}

template<typename T>
HD_GrowingArray<T>::~HD_GrowingArray()
{
	HD_SafeDeleteArray(myData);
}

template<typename T>
void HD_GrowingArray<T>::PushBack(const T& aItem)
{
	CheckSizeAndGrowIfNecessary();

	myData[mySize] = aItem;
	mySize++;
}

template<typename T>
void HD_GrowingArray<T>::PushBack(T&& aItem)
{
	CheckSizeAndGrowIfNecessary();

	myData[mySize] = HD_Move(aItem);
	mySize++;
}

template<typename T>
void HD_GrowingArray<T>::InsertSorted(const T& aItem)
{
	if (GetIsEmpty())
	{
		PushBack(aItem);
	}
	else
	{
		int index = 0;
		while (index < mySize && aItem > myData[index])
			index++;

		InsertAtIndex(aItem, index);
	}
}

template<typename T>
void HD_GrowingArray<T>::InsertSortedReverse(const T& aItem)
{
	if (GetIsEmpty())
	{
		PushBack(aItem);
	}
	else
	{
		int index = 0;
		while (index < mySize && aItem < myData[index])
			index++;

		InsertAtIndex(aItem, index);
	}
}

template<typename T>
void HD_GrowingArray<T>::Remove(int aIndex)
{
	assert(!GetIsEmpty() && 0 <= aIndex && aIndex < mySize);

	if (aIndex < mySize - 1)
		memcpy(myData + aIndex, myData + (aIndex + 1), sizeof(T) * (mySize - 1 - aIndex));
	
	mySize--;
}

template<typename T>
void HD_GrowingArray<T>::RemoveCyclic(int aIndex)
{
	assert(!GetIsEmpty() && 0 <= aIndex && aIndex < mySize);

	if (aIndex < mySize - 1)
		myData[aIndex] = GetLast();

	mySize--;
}

template<typename T>
void HD_GrowingArray<T>::RemoveAll()
{
	mySize = 0;
}

template<typename T>
void HD_GrowingArray<T>::Delete(int aIndex)
{
	assert(!GetIsEmpty() && 0 <= aIndex && aIndex < mySize);

	HD_SafeDelete(myData[aIndex]);
	if (aIndex < mySize - 1)
		memcpy(myData + aIndex, myData + (aIndex + 1), sizeof(T) * (mySize - 1 - aIndex));

	mySize--;
}

template<typename T>
void HD_GrowingArray<T>::DeleteCyclic(int aIndex)
{
	assert(!GetIsEmpty() && 0 <= aIndex && aIndex < mySize);

	HD_SafeDelete(myData[aIndex]);
	if (aIndex < mySize - 1)
		myData[aIndex] = GetLast();

	mySize--;
}

template<typename T>
void HD_GrowingArray<T>::DeleteAll()
{
	for (int i = 0; i < mySize; i++)
		HD_SafeDelete(myData[i]);

	mySize = 0;
}

template<typename T>
int HD_GrowingArray<T>::GetSize() const
{
	return mySize;
}

template<typename T>
bool HD_GrowingArray<T>::GetIsEmpty() const
{
	return mySize == 0;
}

template<typename T>
void HD_GrowingArray<T>::Reserve(int aCapacity)
{
	if (aCapacity <= myCapacity)
		return;

	Grow(aCapacity);
}

template<typename T>
void HD_GrowingArray<T>::Resize(int aSize)
{
	if (aSize <= mySize)
		return;
	
	if (aSize > myCapacity)
		Grow(aSize);

	mySize = aSize;

	for (int i = 0; i < mySize; i++)
		myData[i] = T();
}

template<typename T>
void HD_GrowingArray<T>::Swap(HD_GrowingArray& aOther)
{
	T* tempData = myData;
	int tempSize = mySize;
	int tempCapacity = myCapacity;

	myData = aOther.myData;
	mySize = aOther.mySize;
	myCapacity = aOther.myCapacity;

	aOther.myData = tempData;
	aOther.mySize = tempSize;
	aOther.myCapacity = tempCapacity;
}

template<typename T>
T& HD_GrowingArray<T>::operator[](int aIndex)
{
	assert(0 <= aIndex && aIndex < mySize);
	return myData[aIndex];
}

template<typename T>
const T& HD_GrowingArray<T>::operator[](int aIndex) const
{
	assert(0 <= aIndex && aIndex < mySize);
	return myData[aIndex];
}

template<typename T>
HD_GrowingArray<T>& HD_GrowingArray<T>::operator=(const HD_GrowingArray& aGrowingArray)
{
	bool isCurrentBufferTooSmall = myCapacity < aGrowingArray.mySize;

	if (isCurrentBufferTooSmall)
	{
		HD_SafeDeleteArray(myData);
		myData = new T[aGrowingArray.myCapacity];
	}

	memcpy(myData, aGrowingArray.myData, sizeof(T) * aGrowingArray.mySize);
	mySize = aGrowingArray.mySize;
	myCapacity = aGrowingArray.myCapacity;

	return *this;
}

template<typename T>
HD_GrowingArray<T>& HD_GrowingArray<T>::operator=(HD_GrowingArray&& aGrowingArray)
{
	HD_SafeDeleteArray(myData);

	myData = aGrowingArray.myData;
	aGrowingArray.myData = nullptr;
	mySize = aGrowingArray.mySize;
	myCapacity = aGrowingArray.myCapacity;

	return *this;
}

template<typename T>
T& HD_GrowingArray<T>::GetFirst()
{
	assert(!GetIsEmpty());
	return myData[0];
}

template<typename T>
T& HD_GrowingArray<T>::GetLast()
{
	assert(!GetIsEmpty());
	return myData[mySize - 1];
}

template<typename T>
const T& HD_GrowingArray<T>::GetFirst() const
{
	assert(!GetIsEmpty());
	return myData[0];
}

template<typename T>
const T& HD_GrowingArray<T>::GetLast() const
{
	assert(!GetIsEmpty());
	return myData[mySize - 1];
}

template<typename T>
typename HD_GrowingArray<T>::Iterator HD_GrowingArray<T>::begin()
{
	return Iterator(myData);
}

template<typename T>
typename HD_GrowingArray<T>::Iterator HD_GrowingArray<T>::end()
{
	return Iterator(myData + mySize);
}

template<typename T>
typename HD_GrowingArray<T>::ConstIterator HD_GrowingArray<T>::begin() const
{
	return ConstIterator(myData);
}

template<typename T>
typename HD_GrowingArray<T>::ConstIterator HD_GrowingArray<T>::end() const
{
	return ConstIterator(myData + mySize);
}

template<typename T>
void HD_GrowingArray<T>::CheckSizeAndGrowIfNecessary()
{
	if (mySize + 1 > myCapacity)
	{
		int newCapacity = static_cast<int>(myCapacity * ourGrowFactor);
		assert(newCapacity > myCapacity && "HD_GrowingArray grew larger than int max.");
		Grow(newCapacity);
	}
}

template<typename T>
void HD_GrowingArray<T>::Grow(int aNewCapacity)
{
	T* oldData = myData;
	myData = new T[aNewCapacity];
	memcpy(myData, oldData, sizeof(T) * mySize);
	HD_SafeDeleteArray(oldData);
	myCapacity = aNewCapacity;
}

template<typename T>
void HD_GrowingArray<T>::InsertAtIndex(const T& aItem, int aIndex)
{
	CheckSizeAndGrowIfNecessary();

	memcpy(myData + (aIndex + 1), myData + aIndex, sizeof(T) * (mySize - aIndex));
	myData[aIndex] = aItem;
	mySize++;
}

template<typename T>
void HD_Swap(HD_GrowingArray<T>& aLhs, HD_GrowingArray<T>& aRhs)
{
	aLhs.Swap(aRhs);
}
