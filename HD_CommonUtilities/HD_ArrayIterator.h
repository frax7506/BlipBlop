#pragma once

template<typename T>
class HD_ArrayIterator
{
public:
	HD_ArrayIterator();
	HD_ArrayIterator(T* aPointer);
	HD_ArrayIterator(const HD_ArrayIterator& aIterator);

	HD_ArrayIterator& operator++();
	HD_ArrayIterator& operator--();
	HD_ArrayIterator operator++(int);
	HD_ArrayIterator operator--(int);
	bool operator==(const HD_ArrayIterator& aIterator) const;
	bool operator!=(const HD_ArrayIterator& aIterator) const;
	T& operator*() const;

private:
	T* myPointer;
};

template<typename T>
HD_ArrayIterator<T>::HD_ArrayIterator()
	: myPointer(nullptr)
{
}

template<typename T>
HD_ArrayIterator<T>::HD_ArrayIterator(T* aPointer)
	: myPointer(aPointer)
{
}

template<typename T>
HD_ArrayIterator<T>::HD_ArrayIterator(const HD_ArrayIterator& aIterator)
	: myPointer(aIterator.myPointer)
{
}

template<typename T>
HD_ArrayIterator<T>& HD_ArrayIterator<T>::operator++()
{
	++myPointer;
	return *this;
}

template<typename T>
HD_ArrayIterator<T>& HD_ArrayIterator<T>::operator--()
{
	--myPointer;
	return *this;
}

template<typename T>
HD_ArrayIterator<T> HD_ArrayIterator<T>::operator++(int)
{
	HD_ArrayIterator iterator = *this;
	++myPointer;
	return iterator;
}

template<typename T>
HD_ArrayIterator<T> HD_ArrayIterator<T>::operator--(int)
{
	HD_ArrayIterator iterator = *this;
	--myPointer;
	return iterator;
}

template<typename T>
bool HD_ArrayIterator<T>::operator==(const HD_ArrayIterator& aIterator) const
{
	return myPointer == aIterator.myPointer;
}

template<typename T>
bool HD_ArrayIterator<T>::operator!=(const HD_ArrayIterator& aIterator) const
{
	return !(*this == aIterator);
}

template<typename T>
T& HD_ArrayIterator<T>::operator*() const
{
	return *myPointer;
}
