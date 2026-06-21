#pragma once

#include "HD_ArrayIterator.h"
#include "HD_Move.h"
#include "HD_SafeDelete.h"

#include <cassert>
#include <initializer_list>

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
	HD_GrowingArray(std::initializer_list<T> aInitializerList);
	~HD_GrowingArray();

	T* GetData();
	const T* GetData() const;

	void PushBack(const T& aItem);
	void PushBack(T&& aItem);

	template<typename... Args>
	void EmplaceBack(Args&&... args);

	void InsertSorted(const T& aItem);
	void InsertSortedReverse(const T& aItem);

	void Remove(int aIndex);
	void RemoveCyclic(int aIndex);
	void RemoveAll();

	int GetSize() const;
	bool GetIsEmpty() const;

	void Reserve(int aCapacity);
	void Resize(int aSize);

	void Swap(HD_GrowingArray& aOther);

	T& operator[](int aIndex);
	const T& operator[](int aIndex) const;

	HD_GrowingArray& operator=(const HD_GrowingArray& aGrowingArray);
	HD_GrowingArray& operator=(HD_GrowingArray&& aGrowingArray);
	HD_GrowingArray& operator=(std::initializer_list<T> aInitializerList);

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
	: myData(nullptr)
	, mySize(0)
	, myCapacity(0)
{
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
	: myData(nullptr)
	, mySize(0)
	, myCapacity(0)
{
	Reserve(aGrowingArray.mySize);

	for (int i = 0; i < aGrowingArray.GetSize(); i++)
	{
		PushBack(aGrowingArray[i]);
	}
}

template<typename T>
HD_GrowingArray<T>::HD_GrowingArray(HD_GrowingArray&& aGrowingArray)
{
	myData = aGrowingArray.myData;
	mySize = aGrowingArray.mySize;
	myCapacity = aGrowingArray.myCapacity;

	aGrowingArray.myData = nullptr;
	aGrowingArray.mySize = 0;
	aGrowingArray.myCapacity = 0;
}

template<typename T>
HD_GrowingArray<T>::HD_GrowingArray(std::initializer_list<T> aInitializerList)
	: myData(nullptr)
	, mySize(0)
	, myCapacity(0)
{
	int initializerListSize = static_cast<int>(aInitializerList.size());
	Reserve(initializerListSize);

	for (int i = 0; i < initializerListSize; i++)
	{
		PushBack(*(aInitializerList.begin() + i));
	}
}

template<typename T>
HD_GrowingArray<T>::~HD_GrowingArray()
{
	HD_SafeDeleteArray(myData);
}

template<typename T>
T* HD_GrowingArray<T>::GetData()
{
	return myData;
}

template<typename T>
const T* HD_GrowingArray<T>::GetData() const
{
	return myData;
}

template<typename T>
void HD_GrowingArray<T>::PushBack(const T& aItem)
{
	EmplaceBack(aItem);
}

template<typename T>
void HD_GrowingArray<T>::PushBack(T&& aItem)
{
	EmplaceBack(HD_Move(aItem));
}

template<typename T>
template<typename... Args>
void HD_GrowingArray<T>::EmplaceBack(Args&&... args)
{
	CheckSizeAndGrowIfNecessary();

	myData[mySize].~T();
	new (myData + mySize) T(HD_Forward<Args>(args)...);
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
	{
		for (int i = aIndex; i < mySize - 1; i++)
			myData[i] = HD_Move(myData[i + 1]);
	}

	mySize--;
}

template<typename T>
void HD_GrowingArray<T>::RemoveCyclic(int aIndex)
{
	assert(!GetIsEmpty() && 0 <= aIndex && aIndex < mySize);

	if (aIndex < mySize - 1)
		myData[aIndex] = HD_Move(GetLast());

	mySize--;
}

template<typename T>
void HD_GrowingArray<T>::RemoveAll()
{
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
	RemoveAll();
	Reserve(aGrowingArray.mySize);

	for (int i = 0; i < aGrowingArray.GetSize(); i++)
	{
		PushBack(aGrowingArray[i]);
	}

	return *this;
}

template<typename T>
HD_GrowingArray<T>& HD_GrowingArray<T>::operator=(HD_GrowingArray&& aGrowingArray)
{
	HD_SafeDeleteArray(myData);

	myData = aGrowingArray.myData;
	mySize = aGrowingArray.mySize;
	myCapacity = aGrowingArray.myCapacity;

	aGrowingArray.myData = nullptr;
	aGrowingArray.mySize = 0;
	aGrowingArray.myCapacity = 0;

	return *this;
}

template<typename T>
HD_GrowingArray<T>& HD_GrowingArray<T>::operator=(std::initializer_list<T> aInitializerList)
{
	RemoveAll();

	int initializerListSize = static_cast<int>(aInitializerList.size());
	Reserve(initializerListSize);

	for (int i = 0; i < initializerListSize; i++)
	{
		PushBack(*(aInitializerList.begin() + i));
	}

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
		int newCapacity = myCapacity == 0 ? 2 : static_cast<int>(myCapacity * ourGrowFactor);
		Grow(newCapacity);
	}
}

template<typename T>
void HD_GrowingArray<T>::Grow(int aNewCapacity)
{
	T* oldData = myData;
	myData = new T[aNewCapacity];

	for (int i = 0; i < mySize; i++)
		myData[i] = oldData[i];

	HD_SafeDeleteArray(oldData);
	myCapacity = aNewCapacity;
}

template<typename T>
void HD_GrowingArray<T>::InsertAtIndex(const T& aItem, int aIndex)
{
	CheckSizeAndGrowIfNecessary();
	mySize++;

	for (int i = mySize - 1; i > aIndex; i--)
	{
		myData[i] = HD_Move(myData[i - 1]);
	}

	myData[aIndex] = aItem;
}

template<typename T>
void HD_Swap(HD_GrowingArray<T>& aLhs, HD_GrowingArray<T>& aRhs)
{
	aLhs.Swap(aRhs);
}
