#pragma once

template<int aCapacity>
class HD_DataBuffer
{
public:
	HD_DataBuffer();

	char* GetBuffer();
	int GetSize() const;

	template<typename T>
	void ReadFromBuffer(T& aDataToWriteTo, int aSizeOfDataToWriteTo);

	template<typename T>
	void WriteToBuffer(const T& aDataToReadFrom, int aSizeOfDataToReadFrom);

protected:
	char myData[aCapacity];
	int myReadWriteIndex;
};

template<int aCapacity>
HD_DataBuffer<aCapacity>::HD_DataBuffer()
	: myReadWriteIndex(0)
{
	memset(myData, 0, aCapacity);
}

template<int aCapacity>
char* HD_DataBuffer<aCapacity>::GetBuffer()
{
	return myData;
}

template<int aCapacity>
int HD_DataBuffer<aCapacity>::GetSize() const
{
	return myReadWriteIndex;
}

template<int aCapacity>
template<typename T>
void HD_DataBuffer<aCapacity>::ReadFromBuffer(T& aDataToWriteTo, int aSizeOfDataToWriteTo)
{
	memcpy(&aDataToWriteTo, myData + myReadWriteIndex, aSizeOfDataToWriteTo);
	myReadWriteIndex += aSizeOfDataToWriteTo;
}

template<int aCapacity>
template<typename T>
void HD_DataBuffer<aCapacity>::WriteToBuffer(const T& aDataToReadFrom, int aSizeOfDataToReadFrom)
{
	memcpy(myData + myReadWriteIndex, &aDataToReadFrom, aSizeOfDataToReadFrom);
	myReadWriteIndex += aSizeOfDataToReadFrom;
}
