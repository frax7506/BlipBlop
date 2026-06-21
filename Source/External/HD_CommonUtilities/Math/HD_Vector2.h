#pragma once

template<typename T>
class HD_Vector2
{
public:
	HD_Vector2();
	HD_Vector2(T aX, T aY);
	HD_Vector2(const HD_Vector2& aOther);

	void Set(T aX, T aY);

	T Length() const;
	T Length2() const;

	void Normalize();
	HD_Vector2 GetNormalized() const;

	T Dot(const HD_Vector2& aOther) const;

	HD_Vector2& operator=(const HD_Vector2& aOther);
	HD_Vector2& operator+=(const HD_Vector2& aOther);
	HD_Vector2& operator-=(const HD_Vector2& aOther);
	HD_Vector2& operator*=(T aScalar);
	HD_Vector2& operator/=(T aScalar);

	T myX, myY;

public:
	static const HD_Vector2 Zero;
	static const HD_Vector2 Up;
	static const HD_Vector2 Down;
	static const HD_Vector2 Left;
	static const HD_Vector2 Right;
};

template<typename T> HD_Vector2<T> operator+(const HD_Vector2<T>& aVector0, const HD_Vector2<T>& aVector1);
template<typename T> HD_Vector2<T> operator-(const HD_Vector2<T>& aVector0, const HD_Vector2<T>& aVector1);
template<typename T> HD_Vector2<T> operator*(const HD_Vector2<T>& aVector, T aScalar);
template<typename T> HD_Vector2<T> operator*(T aScalar, const HD_Vector2<T>& aVector);
template<typename T> HD_Vector2<T> operator/(const HD_Vector2<T>& aVector, T aScalar);

template<typename T>
HD_Vector2<T>::HD_Vector2()
	: myX()
	, myY()
{
}

template<typename T>
HD_Vector2<T>::HD_Vector2(T aX, T aY)
	: myX(aX)
	, myY(aY)
{
}

template<typename T>
HD_Vector2<T>::HD_Vector2(const HD_Vector2& aOther)
	: myX(aOther.myX)
	, myY(aOther.myY)
{
}

template<typename T>
void HD_Vector2<T>::Set(T aX, T aY)
{
	myX = aX;
	myY = aY;
}

template<typename T>
T HD_Vector2<T>::Length() const
{
	// Note: this will cause float impercisions if the class is used
	// with big values. So far it's not been an issue. Normally
	// this functions isn't used on non-float instances. When
	// HD_Vector2i is used then it's more used like a container than
	// a mathematical vector.

	return HD_Sqrt(myX * myX + myY * myY);
}

template<typename T>
T HD_Vector2<T>::Length2() const
{
	return myX * myX + myY * myY;
}

template<typename T>
void HD_Vector2<T>::Normalize()
{
	// Note: will not compile if used on non-floating point types
	// with warning level 4 and warnings treated as errors, due to
	// data loss in the float -> non-float conversion. But normalizing
	// a non-floating point type vector should be unusual anyway.

	T length = Length();
	(*this) /= length;
}

template<typename T>
HD_Vector2<T> HD_Vector2<T>::GetNormalized() const
{
	// Note: same note as Normalize.

	HD_Vector2<T> temp(*this);
	temp.Normalize();
	return temp;
}

template<typename T>
T HD_Vector2<T>::Dot(const HD_Vector2<T>& aOther) const
{
	return myX * aOther.myX + myY * aOther.myY;
}

template<typename T>
HD_Vector2<T>& HD_Vector2<T>::operator=(const HD_Vector2<T>& aOther)
{
	myX = aOther.myX;
	myY = aOther.myY;

	return *this;
}

template<typename T>
HD_Vector2<T>& HD_Vector2<T>::operator+=(const HD_Vector2<T>& aOther)
{
	myX += aOther.myX;
	myY += aOther.myY;

	return *this;
}

template<typename T>
HD_Vector2<T>& HD_Vector2<T>::operator-=(const HD_Vector2<T>& aOther)
{
	myX -= aOther.myX;
	myY -= aOther.myY;

	return *this;
}

template<typename T>
HD_Vector2<T>& HD_Vector2<T>::operator*=(T aScalar)
{
	myX *= aScalar;
	myY *= aScalar;

	return *this;
}

template<typename T>
HD_Vector2<T>& HD_Vector2<T>::operator/=(T aScalar)
{
	myX /= aScalar;
	myY /= aScalar;

	return *this;
}

template<typename T>
HD_Vector2<T> operator+(const HD_Vector2<T>& aVector0, const HD_Vector2<T>& aVector1)
{
	return { aVector0.myX + aVector1.myX, aVector0.myY + aVector1.myY };
}

template<typename T>
HD_Vector2<T> operator-(const HD_Vector2<T>& aVector0, const HD_Vector2<T>& aVector1)
{
	return { aVector0.myX - aVector1.myX, aVector0.myY - aVector1.myY };
}

template<typename T>
HD_Vector2<T> operator*(const HD_Vector2<T>& aVector, T aScalar)
{
	return { aVector.myX * aScalar, aVector.myY * aScalar };
}

template<typename T>
HD_Vector2<T> operator*(T aScalar, const HD_Vector2<T>& aVector)
{
	return aVector * aScalar;
}

template<typename T>
HD_Vector2<T> operator/(const HD_Vector2<T>& aVector, T aScalar)
{
	return { aVector.myX / aScalar, aVector.myY / aScalar };
}

typedef HD_Vector2<float> HD_Vector2f;
typedef HD_Vector2<int> HD_Vector2i;
typedef HD_Vector2<unsigned int> HD_Vector2ui;

template <> const HD_Vector2<float> HD_Vector2<float>::Zero = { 0.f, 0.f };
template <> const HD_Vector2<float> HD_Vector2<float>::Up = { 0.f, 1.f };
template <> const HD_Vector2<float> HD_Vector2<float>::Down = { 0.f, -1.f };
template <> const HD_Vector2<float> HD_Vector2<float>::Left = { -1.f, 0.f };
template <> const HD_Vector2<float> HD_Vector2<float>::Right = { 1.f, 0.f };
