#pragma once

// Inspired by Google's flat_hash_set: https://www.youtube.com/watch?v=ncHmEUmJZf4
	// * Uses open addressing to handle collissions. Key-values are stored directly
	//		in the data array, minimizing the amount of indirections. This table
	//		uses linear probing.
	// * Metadata is stored at the beginning of the array(1b for each element),
	//		containing information about the slots' states. This means that even
	//		though 8b keys are stored in the array, the whole array can be scanned
	//		quickly and cache-friendly using this metadata.
	// * Removing entries creates tombstones. These slots can be reused on insert,
	//		but are skipped during find. This means that removing many elements and then
	//		trying to find an element can be expensive. If this is an issue then HD_Map
	//		should be used instead, which is an AVL-tree structure.
	// * Overall this map favors speed on insert and find, at the cost of memory.

// Future work:
	// * I don't fully understand how the map in the cppcon talk manages to function
	//		without storing keys or hash codes together with the values. Finding an
	//		element with a given key means we can find a probe sequence, but how
	//		do we find the correct element in that sequence with only a key/hash if
	//		only values are stored in the array? Only storing values would be more
	//		optimized, mainly in terms of memory but perhaps also in speed if more
	//		values fit in one cache line.
	// * If we want to store something together with the value, then storing the 
	//		hash code would be nice. That would mean that we wouldn't need to re-
	//		calculate the hash code when we rehash. However, while iterating the map
	//		I want to be able to extract the key, and if I store hash codes instead
	//		then I don't know how to reverse that to a key-value (if that's possible).
	// * Right now I'm calculating the hash code a bit too often. For example, when
	//		I insert a value the hash code is computed at least twice, and three
	//		times if the insert triggers a rehash. It should only have to be
	//		calculated once per insert.

#include "HD_Hash.h"
#include "HD_Pair.h"
#include "HD_Utilities.h"

template<typename K, typename V>
using KeyValuePair = HD_Pair<K, V>;

typedef char ControlByte_t;

template<typename KeyValuePair_t>
class HD_HashMapIterator
{
public:
	HD_HashMapIterator();
	HD_HashMapIterator(const ControlByte_t* aControlBytes, KeyValuePair_t* aKeyValuePairs, int aIndex, int aHashMapCapacity);
	HD_HashMapIterator(const HD_HashMapIterator& aIterator);

	HD_HashMapIterator& operator++();
	HD_HashMapIterator& operator--();
	HD_HashMapIterator operator++(int);
	HD_HashMapIterator operator--(int);
	HD_HashMapIterator& operator+=(int aIncrement);
	HD_HashMapIterator& operator-=(int aDecrement);
	bool operator==(const HD_HashMapIterator& aIterator) const;
	bool operator!=(const HD_HashMapIterator& aIterator) const;
	KeyValuePair_t& operator*() const;
	KeyValuePair_t* operator->() const;

private:
	const ControlByte_t* myControlBytes;
	KeyValuePair_t* myKeyValuePairs;

	int myIndex;
	int myHashMapCapacity;
};

template<typename K, typename V>
class HD_HashMap
{
public:
	typedef HD_HashMapIterator<KeyValuePair<K, V>> Iterator;
	typedef HD_HashMapIterator<const KeyValuePair<K, V>> ConstIterator;
	friend class Iterator;
	friend class ConstIterator;

	HD_HashMap();
	HD_HashMap(const HD_HashMap& aHashMap);
	HD_HashMap(HD_HashMap&& aHashMap);
	~HD_HashMap();

	const V* GetIfExists(const K& aKey) const;

	V& operator[](const K& aKey);

	HD_HashMap& operator=(const HD_HashMap& aHashMap);
	HD_HashMap& operator=(HD_HashMap&& aHashMap);

	void Remove(const K& aKey);

	void Clear();

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

private:
	enum ControlByte : ControlByte_t
	{
		ControlByte_Empty = 0b00000000,
		ControlByte_Deleted = 0b01111111,
		// ControlByte_Full = 0b1xxxxxxx
	};

	static constexpr float ourMaximumLoadFactor = 0.5f;
	static constexpr float ourGrowFactor = 2.f;

	void InitWithCapacity(int aCapacity);

	void InsertKeyValueAtIndex(const K& aKey, const V& aValue, int aIndex);

	void Rehash();

	// Finds the index where an element is, or would be.
	int GetSlotIndexForKey(const K& aKey) const;

	int GetFirstSlotIndex() const;

	bool GetIsSlotFullAtIndex(int aIndex) const;

	size_t GetLevel1Hash(size_t aHash) const { return aHash >> 7; }
	ControlByte_t GetLevel2Hash(size_t aHash) const { return aHash & 0x7F; }

	char* myData;
	ControlByte_t* myControlBytes;
	KeyValuePair<K, V>* myKeyValuePairs;
	int mySize;
	int myCapacity;
};

template<typename K, typename V>
HD_HashMap<K, V>::HD_HashMap()
	: myData(nullptr)
	, myControlBytes(nullptr)
	, myKeyValuePairs(nullptr)
	, mySize(0)
	, myCapacity(0)
{
	InitWithCapacity(16);
}

template<typename K, typename V>
HD_HashMap<K, V>::HD_HashMap(const HD_HashMap& aHashMap)
	: myData(nullptr)
	, myControlBytes(nullptr)
	, myKeyValuePairs(nullptr)
	, mySize(0)
	, myCapacity(0)
{
	InitWithCapacity(aHashMap.myCapacity);

	for (auto it = aHashMap.begin(); it != aHashMap.end(); it++)
	{
		const K& key = it->myFirst;
		const V& value = it->mySecond;

		int index = GetSlotIndexForKey(key);
		InsertKeyValueAtIndex(key, value, index);
		mySize++;
	}
}

template<typename K, typename V>
HD_HashMap<K, V>::HD_HashMap(HD_HashMap&& aHashMap)
	: myData(nullptr)
	, myControlBytes(nullptr)
	, myKeyValuePairs(nullptr)
	, mySize(aHashMap.mySize)
	, myCapacity(aHashMap.myCapacity)
{
	myData = aHashMap.myData;
	myControlBytes = myData;
	myKeyValuePairs = reinterpret_cast<KeyValuePair<K, V>*>(myControlBytes + myCapacity);

	aHashMap.myData = nullptr;
}

template<typename K, typename V>
HD_HashMap<K, V>::~HD_HashMap()
{
	HD_SafeDeleteArray(myData);
}

template<typename K, typename V>
const V* HD_HashMap<K, V>::GetIfExists(const K& aKey) const
{
	int index = GetSlotIndexForKey(aKey);
	bool isFull = GetIsSlotFullAtIndex(index);

	if (isFull)
	{
		return &myKeyValuePairs[index].mySecond;
	}

	return nullptr;
}

template<typename K, typename V>
V& HD_HashMap<K, V>::operator[](const K& aKey)
{
	int index = GetSlotIndexForKey(aKey);
	bool isFull = GetIsSlotFullAtIndex(index);

	if (isFull)
	{
		return myKeyValuePairs[index].mySecond;
	}

	float newLoadFactor = static_cast<float>(mySize + 1) / myCapacity;
	if (newLoadFactor > ourMaximumLoadFactor)
	{
		Rehash();
		index = GetSlotIndexForKey(aKey);
	}

	if (myControlBytes[index] == ControlByte_Empty)
	{
		mySize++;
	}

	InsertKeyValueAtIndex(aKey, V(), index);
	return myKeyValuePairs[index].mySecond;
}

template<typename K, typename V>
HD_HashMap<K, V>& HD_HashMap<K, V>::operator=(const HD_HashMap& aHashMap)
{
	Clear();

	bool isCapacitySmaller = myCapacity < aHashMap.myCapacity;

	if (isCapacitySmaller)
	{
		HD_SafeDeleteArray(myData);
		InitWithCapacity(aHashMap.myCapacity);
	}

	for (auto it = aHashMap.begin(); it != aHashMap.end(); it++)
	{
		const K& key = it->myFirst;
		const V& value = it->mySecond;

		int index = GetSlotIndexForKey(key);
		InsertKeyValueAtIndex(key, value, index);
		mySize++;
	}

	return *this;
}

template<typename K, typename V>
HD_HashMap<K, V>& HD_HashMap<K, V>::operator=(HD_HashMap&& aHashMap)
{
	mySize = aHashMap.mySize;
	myCapacity = aHashMap.myCapacity;

	myData = aHashMap.myData;
	myControlBytes = myData;
	myKeyValuePairs = reinterpret_cast<KeyValuePair<K, V>*>(myControlBytes + myCapacity);

	aHashMap.myData = nullptr;

	return *this;
}

template<typename K, typename V>
void HD_HashMap<K, V>::Remove(const K& aKey)
{
	int index = GetSlotIndexForKey(aKey);
	bool isFull = GetIsSlotFullAtIndex(index);

	if (isFull)
	{
		myControlBytes[index] = ControlByte_Deleted;
	}
}

template<typename K, typename V>
void HD_HashMap<K, V>::Clear()
{
	memset(myControlBytes, 0, sizeof(ControlByte_t) * myCapacity);
	mySize = 0;
}

template<typename K, typename V>
typename HD_HashMap<K, V>::Iterator HD_HashMap<K, V>::begin()
{
	int firstIndex = GetFirstSlotIndex();
	return Iterator(myControlBytes, myKeyValuePairs, firstIndex, myCapacity);
}

template<typename K, typename V>
typename HD_HashMap<K, V>::Iterator HD_HashMap<K, V>::end()
{
	return Iterator(myControlBytes, myKeyValuePairs, myCapacity, myCapacity);
}

template<typename K, typename V>
typename HD_HashMap<K, V>::ConstIterator HD_HashMap<K, V>::begin() const
{
	int firstIndex = GetFirstSlotIndex();
	return ConstIterator(myControlBytes, myKeyValuePairs, firstIndex, myCapacity);
}

template<typename K, typename V>
typename HD_HashMap<K, V>::ConstIterator HD_HashMap<K, V>::end() const
{
	return ConstIterator(myControlBytes, myKeyValuePairs, myCapacity, myCapacity);
}

template<typename K, typename V>
void HD_HashMap<K, V>::InitWithCapacity(int aCapacity)
{
	myCapacity = aCapacity;
	mySize = 0;

	myData = new char[myCapacity + sizeof(KeyValuePair<size_t, V>) * myCapacity] { 0 };
	myControlBytes = myData;
	myKeyValuePairs = reinterpret_cast<KeyValuePair<K, V>*>(myControlBytes + myCapacity);
}

template<typename K, typename V>
void HD_HashMap<K, V>::InsertKeyValueAtIndex(const K& aKey, const V& aValue, int aIndex)
{
	size_t hashCode = HD_Hash(aKey);
	myControlBytes[aIndex] = GetLevel2Hash(hashCode) | 0b10000000;
	myKeyValuePairs[aIndex].myFirst = aKey;
	myKeyValuePairs[aIndex].mySecond = aValue;
}

template<typename K, typename V>
void HD_HashMap<K, V>::Rehash()
{
	HD_HashMap temp = HD_Move(*this);
	InitWithCapacity(static_cast<int>(myCapacity * ourGrowFactor));
	(*this) = temp;
}

template<typename K, typename V>
int HD_HashMap<K, V>::GetSlotIndexForKey(const K& aKey) const
{
	size_t hashCode = HD_Hash(aKey);
	int index = GetLevel1Hash(hashCode) % myCapacity;

	while (true)
	{
		bool isSlotEmpty = myControlBytes[index] == ControlByte_Empty;
		bool isLevel2HashSame = GetLevel2Hash(myControlBytes[index]) == GetLevel2Hash(hashCode);

		if (isSlotEmpty || (isLevel2HashSame && myKeyValuePairs[index].myFirst == aKey))
		{
			return index;
		}

		index++;
	}
}

template<typename K, typename V>
int HD_HashMap<K, V>::GetFirstSlotIndex() const
{
	for (int i = 0; i < myCapacity; i++)
	{
		if (GetIsSlotFullAtIndex(i))
		{
			return i;
		}
	}

	return -1;
}

template<typename K, typename V>
bool HD_HashMap<K, V>::GetIsSlotFullAtIndex(int aIndex) const
{
	return myControlBytes[aIndex] & 0b10000000;
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>::HD_HashMapIterator()
	: myControlBytes(nullptr)
	, myKeyValuePairs(nullptr)
	, myIndex(0)
	, myHashMapCapacity(0)
{
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>::HD_HashMapIterator(const ControlByte_t* aControlBytes, KeyValuePair_t* aKeyValuePairs, int aIndex, int aHashMapCapacity)
	: myControlBytes(aControlBytes)
	, myKeyValuePairs(aKeyValuePairs)
	, myIndex(aIndex)
	, myHashMapCapacity(aHashMapCapacity)
{
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>::HD_HashMapIterator(const HD_HashMapIterator& aIterator)
	: myControlBytes(aIterator.myControlBytes)
	, myKeyValuePairs(aIterator.myKeyValuePairs)
	, myIndex(aIterator.myIndex)
	, myHashMapCapacity(aIterator.myHashMapCapacity)
{
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>& HD_HashMapIterator<KeyValuePair_t>::operator++()
{
	do
	{
		myIndex++;
	}
	while (myIndex < myHashMapCapacity && !(myControlBytes[myIndex] & 0b10000000));

	return *this;
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>& HD_HashMapIterator<KeyValuePair_t>::operator--()
{
	do
	{
		myIndex--;
	}
	while (myIndex > 0 && !(myControlBytes[myIndex] & 0b10000000));

	return *this;
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t> HD_HashMapIterator<KeyValuePair_t>::operator++(int)
{
	HD_HashMapIterator iterator = *this;
	++(*this);
	return iterator;
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t> HD_HashMapIterator<KeyValuePair_t>::operator--(int)
{
	HD_HashMapIterator iterator = *this;
	--(*this);
	return iterator;
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>& HD_HashMapIterator<KeyValuePair_t>::operator+=(int aIncrement)
{
	for (int i = 0; i < aIncrement; ++i)
		++(*this);

	return *this;
}

template<typename KeyValuePair_t>
HD_HashMapIterator<KeyValuePair_t>& HD_HashMapIterator<KeyValuePair_t>::operator-=(int aDecrement)
{
	for (int i = 0; i < aDecrement; ++i)
		--(*this);

	return *this;
}

template<typename KeyValuePair_t>
bool HD_HashMapIterator<KeyValuePair_t>::operator==(const HD_HashMapIterator& aIterator) const
{
	return myControlBytes == aIterator.myControlBytes && myIndex == aIterator.myIndex;
}

template<typename KeyValuePair_t>
bool HD_HashMapIterator<KeyValuePair_t>::operator!=(const HD_HashMapIterator& aIterator) const
{
	return !(*this == aIterator);
}

template<typename KeyValuePair_t>
KeyValuePair_t& HD_HashMapIterator<KeyValuePair_t>::operator*() const
{
	return myKeyValuePairs[myIndex];
}

template<typename KeyValuePair_t>
KeyValuePair_t* HD_HashMapIterator<KeyValuePair_t>::operator->() const
{
	return myKeyValuePairs + myIndex;
}
