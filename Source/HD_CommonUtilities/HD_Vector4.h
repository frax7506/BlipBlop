#pragma once

template<typename T>
class HD_Vector4
{
public:
	HD_Vector4();
	HD_Vector4(T aX, T aY, T aZ, T aW);
	HD_Vector4(const HD_Vector4& aOther);

	void Set(T aX, T aY, T aZ, T aW);

	HD_Vector4 operator+(const HD_Vector4& aOther) const;
	HD_Vector4 operator-(const HD_Vector4& aOther) const;
	HD_Vector4 operator*(T aConstant) const;
	HD_Vector4 operator/(T aConstant) const;

	HD_Vector4& operator=(const HD_Vector4& aOther);
	HD_Vector4& operator+=(const HD_Vector4& aOther);
	HD_Vector4& operator-=(const HD_Vector4& aOther);
	HD_Vector4& operator*=(T aConstant);
	HD_Vector4& operator/=(T aConstant);

	T myX, myY, myZ, myW;
};

template<typename T>
HD_Vector4<T>::HD_Vector4()
	: myX()
	, myY()
	, myZ()
	, myW()
{
}

template<typename T>
HD_Vector4<T>::HD_Vector4(T aX, T aY, T aZ, T aW)
	: myX(aX)
	, myY(aY)
	, myZ(aZ)
	, myW(aW)
{
}

template<typename T>
HD_Vector4<T>::HD_Vector4(const HD_Vector4& aOther)
	: myX(aOther.myX)
	, myY(aOther.myY)
	, myZ(aOther.myZ)
	, myW(aOther.myW)
{
}

template<typename T>
void HD_Vector4<T>::Set(T aX, T aY, T aZ, T aW)
{
	myX = aX;
	myY = aY;
	myZ = aZ;
	myW = aW;
}

template<typename T>
HD_Vector4<T> HD_Vector4<T>::operator+(const HD_Vector4<T>& aOther) const
{
	return HD_Vector4(myX + aOther.myX, myY + aOther.myY, myZ + aOther.myZ, myW + aOther.myW);
}

template<typename T>
HD_Vector4<T> HD_Vector4<T>::operator-(const HD_Vector4<T>& aOther) const
{
	return HD_Vector4(myX - aOther.myX, myY - aOther.myY, myZ - aOther.myZ, myW - aOther.myW);
}

template<typename T>
HD_Vector4<T> HD_Vector4<T>::operator*(T aConstant) const
{
	return HD_Vector4(myX * aConstant, myY * aConstant, myZ * aConstant, myW * aConstant);
}

template<typename T>
HD_Vector4<T> HD_Vector4<T>::operator/(T aConstant) const
{
	return HD_Vector4(myX / aConstant, myY / aConstant, myZ / aConstant, myW / aConstant);
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator=(const HD_Vector4<T>& aOther)
{
	myX = aOther.myX;
	myY = aOther.myY;
	myZ = aOther.myZ;
	myW = aOther.myW;

	return *this;
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator+=(const HD_Vector4<T>& aOther)
{
	myX += aOther.myX;
	myY += aOther.myY;
	myZ += aOther.myZ;
	myW += aOther.myW;

	return *this;
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator-=(const HD_Vector4<T>& aOther)
{
	myX -= aOther.myX;
	myY -= aOther.myY;
	myZ -= aOther.myZ;
	myW -= aOther.myW;

	return *this;
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator*=(T aConstant)
{
	myX *= aConstant;
	myY *= aConstant;
	myZ *= aConstant;
	myW *= aConstant;

	return *this;
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator/=(T aConstant)
{
	myX /= aConstant;
	myY /= aConstant;
	myZ /= aConstant;
	myW /= aConstant;

	return *this;
}

// Global operator for doing aConstant * aVector.
template<typename T>
HD_Vector4<T> operator*(T aConstant, const HD_Vector4<T>& aVector)
{
	return aVector * aConstant;
}

typedef HD_Vector4<float> HD_Vector4f;
typedef HD_Vector4<int> HD_Vector4i;
typedef HD_Vector4<unsigned int> HD_Vector4ui;
