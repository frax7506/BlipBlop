#pragma once

#include <cassert>

// HD_CircularArray
// * If overflow is enabled and PushBack is called on a full array it will start
//   to overwrite the oldest items.
// * The GetFirstAndRemove function gets the oldest item and removes it, which means
//   that PushBack and GetFirstAndRemove can be used to make this array a FIFO queue.

enum class OverflowBehaviour
{
	Enable,
	Disable,
};

template<typename T, int aCapacity>
class HD_CircularArray
{
public:
	HD_CircularArray();

	void PushBack(const T& aItem);
	void GetFirstAndRemove(T& outItem);

	void Clear();

	int GetSize() const;
	int GetCapacity() const;
	bool IsEmpty() const;

	void SetOverflowBehaviour(OverflowBehaviour aOverflowBehaviour);

	T& operator[](int aIndex);
	const T& operator[](int aIndex) const;

private:
	int myFirstIndex;
	int myWriteIndex;
	int mySize;

	OverflowBehaviour myOverflowBehaviour;
	T myData[aCapacity];
};

template<typename T, int aCapacity>
HD_CircularArray<T, aCapacity>::HD_CircularArray()
	: myFirstIndex(0)
	, myWriteIndex(0)
	, mySize(0)
{
	myOverflowBehaviour = OverflowBehaviour::Enable;
	memset(myData, 0, sizeof(T) * aCapacity);
}

template<typename T, int aCapacity>
void HD_CircularArray<T, aCapacity>::PushBack(const T& aItem)
{
	assert(myOverflowBehaviour != OverflowBehaviour::Disable || mySize + 1 <= aCapacity);

	memcpy(myData + myWriteIndex, &aItem, sizeof(T));

	if (myWriteIndex == myFirstIndex && mySize > 0)
	{
		myWriteIndex = (myWriteIndex + 1) % aCapacity;
		myFirstIndex = (myFirstIndex + 1) % aCapacity;
	}
	else
	{
		myWriteIndex = (myWriteIndex + 1) % aCapacity;
		++mySize;
	}
}

template<typename T, int aCapacity>
void HD_CircularArray<T, aCapacity>::GetFirstAndRemove(T& outItem)
{
	assert(!IsEmpty());

	memcpy(&outItem, myData + myFirstIndex, sizeof(T));
	myFirstIndex = (myFirstIndex + 1) % aCapacity;
	mySize--;
}

template<typename T, int aCapacity>
void HD_CircularArray<T, aCapacity>::Clear()
{
	myFirstIndex = 0;
	myWriteIndex = 0;
	mySize = 0;
}

template<typename T, int aCapacity>
int HD_CircularArray<T, aCapacity>::GetSize() const
{
	return mySize;
}

template<typename T, int aCapacity>
int HD_CircularArray<T, aCapacity>::GetCapacity() const
{
	return aCapacity;
}

template<typename T, int aCapacity>
bool HD_CircularArray<T, aCapacity>::IsEmpty() const
{
	return mySize == 0;
}

template<typename T, int aCapacity>
void HD_CircularArray<T, aCapacity>::SetOverflowBehaviour(OverflowBehaviour aOverflowBehaviour)
{
	myOverflowBehaviour = aOverflowBehaviour;
}

template<typename T, int aCapacity>
T& HD_CircularArray<T, aCapacity>::operator[](int aIndex)
{
	assert(aIndex < GetSize());

	return myData[(myFirstIndex + aIndex) % aCapacity];
}

template<typename T, int aCapacity>
const T& HD_CircularArray<T, aCapacity>::operator[](int aIndex) const
{
	assert(aIndex < GetSize());

	return myData[(myFirstIndex + aIndex) % aCapacity];
}
