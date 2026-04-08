#pragma once

#include "HD_Math.h"

template<typename T>
class HD_Vector3
{
public:
	HD_Vector3();
	HD_Vector3(T aX, T aY, T aZ);
	HD_Vector3(const HD_Vector3& aOther);

	void Set(T aX, T aY, T aZ);

	float Length() const;
	T Length2() const;

	void Normalize();
	HD_Vector3 GetNormalized() const;

	float Dot(const HD_Vector3& aOther) const;
	HD_Vector3 Cross(const HD_Vector3& aOther) const;

	HD_Vector3& operator=(const HD_Vector3& aOther);
	HD_Vector3 operator+(const HD_Vector3& aOther) const;
	HD_Vector3 operator-(const HD_Vector3& aOther) const;
	HD_Vector3 operator*(T aConstant) const;
	HD_Vector3 operator/(T aConstant) const;

	HD_Vector3& operator+=(const HD_Vector3& aOther);
	HD_Vector3& operator-=(const HD_Vector3& aOther);
	HD_Vector3& operator*=(T aConstant);
	HD_Vector3& operator/=(T aConstant);

	T myX, myY, myZ;

public:
	static const HD_Vector3 Zero;
	static const HD_Vector3 Up;
	static const HD_Vector3 Down;
	static const HD_Vector3 Left;
	static const HD_Vector3 Right;
	static const HD_Vector3 Forward;
	static const HD_Vector3 Back;
};

template<typename T>
HD_Vector3<T>::HD_Vector3()
	: myX()
	, myY()
	, myZ()
{
}

template<typename T>
HD_Vector3<T>::HD_Vector3(T aX, T aY, T aZ)
	: myX(aX)
	, myY(aY)
	, myZ(aZ)
{
}

template<typename T>
HD_Vector3<T>::HD_Vector3(const HD_Vector3& aOther)
	: myX(aOther.myX)
	, myY(aOther.myY)
	, myZ(aOther.myZ)
{
}

template<typename T>
void HD_Vector3<T>::Set(T aX, T aY, T aZ)
{
	myX = aX;
	myY = aY;
	myZ = aZ;
}

template<typename T>
float HD_Vector3<T>::Length() const
{
	// Note: this will cause float impercisions if the class is used
	// with big values. So far it's not been an issue. Normally
	// this functions isn't used on non-float instances. When
	// HD_Vector3i is used then it's more used like a container than
	// a mathematical vector.

	float x = static_cast<float>(myX);
	float y = static_cast<float>(myY);
	float z = static_cast<float>(myZ);
	return HD_Sqrtf(x * x + y * y + z * z);
}

template<typename T>
T HD_Vector3<T>::Length2() const
{
	return myX * myX + myY * myY + myZ * myZ;
}

template<typename T>
void HD_Vector3<T>::Normalize()
{
	// Note: will not compile if used on non-floating point types
	// with warning level 4 and warnings treated as errors, due to
	// data loss in the float -> non-float conversion. But normalizing
	// a non-floating point type vector should be unusual anyway.

	float length = Length();
	(*this) /= length;
}

template<typename T>
HD_Vector3<T> HD_Vector3<T>::GetNormalized() const
{
	// Note: same note as Normalize.

	HD_Vector3<T> temp(*this);
	temp.Normalize();
	return temp;
}

template<typename T>
float HD_Vector3<T>::Dot(const HD_Vector3<T>& aOther) const
{
	return myX * aOther.myX + myY * aOther.myY + myZ * aOther.myZ;
}

template<typename T>
HD_Vector3<T> HD_Vector3<T>::Cross(const HD_Vector3<T>& aOther) const
{
	HD_Vector3 result(
		myY* aOther.myZ - aOther.myY * myZ,
		myZ* aOther.myX - aOther.myZ * myX,
		myX* aOther.myY - aOther.myX * myY
	);

	return result;
}

template<typename T>
HD_Vector3<T>& HD_Vector3<T>::operator=(const HD_Vector3<T>& aOther)
{
	myX = aOther.myX;
	myY = aOther.myY;
	myZ = aOther.myZ;

	return *this;
}

template<typename T>
HD_Vector3<T> HD_Vector3<T>::operator+(const HD_Vector3<T>& aOther) const
{
	return HD_Vector3(myX + aOther.myX, myY + aOther.myY, myZ + aOther.myZ);
}

template<typename T>
HD_Vector3<T> HD_Vector3<T>::operator-(const HD_Vector3<T>& aOther) const
{
	return HD_Vector3(myX - aOther.myX, myY - aOther.myY, myZ - aOther.myZ);
}

template<typename T>
HD_Vector3<T> HD_Vector3<T>::operator*(T aConstant) const
{
	return HD_Vector3(myX * aConstant, myY * aConstant, myZ * aConstant);
}

template<typename T>
HD_Vector3<T> HD_Vector3<T>::operator/(T aConstant) const
{
	return HD_Vector3(myX / aConstant, myY / aConstant, myZ / aConstant);
}

template<typename T>
HD_Vector3<T>& HD_Vector3<T>::operator+=(const HD_Vector3<T>& aOther)
{
	myX += aOther.myX;
	myY += aOther.myY;
	myZ += aOther.myZ;

	return *this;
}

template<typename T>
HD_Vector3<T>& HD_Vector3<T>::operator-=(const HD_Vector3<T>& aOther)
{
	myX -= aOther.myX;
	myY -= aOther.myY;
	myZ -= aOther.myZ;

	return *this;
}

template<typename T>
HD_Vector3<T>& HD_Vector3<T>::operator*=(T aConstant)
{
	myX *= aConstant;
	myY *= aConstant;
	myZ *= aConstant;

	return *this;
}

template<typename T>
HD_Vector3<T>& HD_Vector3<T>::operator/=(T aConstant)
{
	myX /= aConstant;
	myY /= aConstant;
	myZ /= aConstant;

	return *this;
}

// Global operator for doing aConstant * aVector.
template<typename T>
HD_Vector3<T> operator*(T aConstant, const HD_Vector3<T>& aVector)
{
	return aVector * aConstant;
}

typedef HD_Vector3<float> HD_Vector3f;
typedef HD_Vector3<int> HD_Vector3i;

template <> const HD_Vector3<float> HD_Vector3<float>::Zero = { 0.f, 0.f, 0.f };
template <> const HD_Vector3<float> HD_Vector3<float>::Up = { 0.f, 1.f, 0.f };
template <> const HD_Vector3<float> HD_Vector3<float>::Down = { 0.f, -1.f, 0.f };
template <> const HD_Vector3<float> HD_Vector3<float>::Left = { -1.f, 0.f, 0.f };
template <> const HD_Vector3<float> HD_Vector3<float>::Right = { 1.f, 0.f, 0.f };
template <> const HD_Vector3<float> HD_Vector3<float>::Forward = { 0.f, 0.f, 1.f };
template <> const HD_Vector3<float> HD_Vector3<float>::Back = { 0.f, 0.f, -1.f };
