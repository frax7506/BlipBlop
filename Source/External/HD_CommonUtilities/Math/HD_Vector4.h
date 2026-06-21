#pragma once

template<typename T>
class HD_Matrix4x4;

template<typename T>
class HD_Vector4
{
public:
	HD_Vector4();
	HD_Vector4(T aX, T aY, T aZ, T aW);
	HD_Vector4(const HD_Vector4& aOther);

	void Set(T aX, T aY, T aZ, T aW);

	HD_Vector4& operator=(const HD_Vector4& aOther);
	HD_Vector4& operator+=(const HD_Vector4& aOther);
	HD_Vector4& operator-=(const HD_Vector4& aOther);
	HD_Vector4& operator*=(const HD_Matrix4x4<T>& aMatrix);
	HD_Vector4& operator*=(T aScalar);
	HD_Vector4& operator/=(T aScalar);

	T myX, myY, myZ, myW;
};

template<typename T> HD_Vector4<T> operator+(const HD_Vector4<T>& aVector0, const HD_Vector4<T>& aVector1);
template<typename T> HD_Vector4<T> operator-(const HD_Vector4<T>& aVector0, const HD_Vector4<T>& aVector1);
template<typename T> HD_Vector4<T> operator*(const HD_Vector4<T>& aVector, T aScalar);
template<typename T> HD_Vector4<T> operator*(T aScalar, const HD_Vector4<T>& aVector);
template<typename T> HD_Vector4<T> operator/(const HD_Vector4<T>& aVector, T aScalar);

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
HD_Vector4<T>& HD_Vector4<T>::operator*=(const HD_Matrix4x4<T>& aMatrix)
{
	(*this) = (*this) * aMatrix;
	return *this;
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator*=(T aScalar)
{
	myX *= aScalar;
	myY *= aScalar;
	myZ *= aScalar;
	myW *= aScalar;

	return *this;
}

template<typename T>
HD_Vector4<T>& HD_Vector4<T>::operator/=(T aScalar)
{
	myX /= aScalar;
	myY /= aScalar;
	myZ /= aScalar;
	myW /= aScalar;

	return *this;
}

template<typename T>
HD_Vector4<T> operator+(const HD_Vector4<T>& aVector0, const HD_Vector4<T>& aVector1)
{
	return { aVector0.myX + aVector1.myX, aVector0.myY + aVector1.myY, aVector0.myZ + aVector1.myZ, aVector0.myW + aVector1.myW };
}

template<typename T>
HD_Vector4<T> operator-(const HD_Vector4<T>& aVector0, const HD_Vector4<T>& aVector1)
{
	return { aVector0.myX - aVector1.myX, aVector0.myY - aVector1.myY, aVector0.myZ - aVector1.myZ, aVector0.myW - aVector1.myW };
}

template<typename T>
HD_Vector4<T> operator*(const HD_Vector4<T>& aVector, T aScalar)
{
	return { aVector.myX * aScalar, aVector.myY * aScalar, aVector.myZ * aScalar, aVector.myW * aScalar };
}

template<typename T>
HD_Vector4<T> operator*(T aScalar, const HD_Vector4<T>& aVector)
{
	return aVector * aScalar;
}

template<typename T>
HD_Vector4<T> operator/(const HD_Vector4<T>& aVector, T aScalar)
{
	return { aVector.myX / aScalar, aVector.myY / aScalar, aVector.myZ / aScalar, aVector.myW / aScalar };
}

typedef HD_Vector4<float> HD_Vector4f;
typedef HD_Vector4<int> HD_Vector4i;
typedef HD_Vector4<unsigned int> HD_Vector4ui;
