#pragma once

#include "HD_Math.h"
#include "HD_Swap.h"

#include <cassert>
#include <cstring>
#include <initializer_list>

template<typename T>
class HD_Vector2;

template<typename T>
class HD_Vector3;

template<typename T>
class HD_Matrix4x4;

template<typename T>
class HD_Matrix3x3
{
public:
	HD_Matrix3x3();
	HD_Matrix3x3(const HD_Matrix3x3& aMatrix);
	HD_Matrix3x3(const HD_Matrix4x4<T>& aMatrix);
	HD_Matrix3x3(std::initializer_list<T> aInitializerList);

	HD_Matrix3x3& operator=(const HD_Matrix3x3& aMatrix);
	HD_Matrix3x3& operator=(const HD_Matrix4x4<T>& aMatrix);
	HD_Matrix3x3& operator=(std::initializer_list<T> aInitializerList);

	T& operator()(int aRow, int aCol);
	const T& operator()(int aRow, int aCol) const;

	HD_Matrix3x3& operator+=(const HD_Matrix3x3& aMatrix);
	HD_Matrix3x3& operator-=(const HD_Matrix3x3& aMatrix);
	HD_Matrix3x3& operator*=(const HD_Matrix3x3& aMatrix);

	void ScaleInX(T aScalar);
	void ScaleInY(T aScalar);

	bool operator==(const HD_Matrix3x3& aMatrix) const;
	bool operator!=(const HD_Matrix3x3& aMatrix) const;

	HD_Vector2<T> GetRightVector() const;
	HD_Vector2<T> GetUpVector() const;

	T GetScaleInX() const;
	T GetScaleInY() const;
	HD_Vector2<T> GetScaleInXY() const;

	T GetRotation() const;

	T GetPositionX() const;
	T GetPositionY() const;
	HD_Vector2<T> GetPosition() const;

	HD_Matrix3x3 GetTranspose() const;
	HD_Matrix3x3 GetFastInverse() const;
	HD_Matrix3x3 Get2x2() const;

	T m11, m12, m13, m21, m22, m23, m31, m32, m33;

public:
	static HD_Matrix3x3 CreateScale(T aScaleInX, T aScaleInY);
	static HD_Matrix3x3 CreateRotation(T aAngleInRadians);
	static HD_Matrix3x3 CreateTranslation(T aX, T aY);

	static const HD_Matrix3x3 Identity;
};

template<typename T> HD_Matrix3x3<T> operator+(const HD_Matrix3x3<T>& aMatrix0, const HD_Matrix3x3<T>& aMatrix1);
template<typename T> HD_Matrix3x3<T> operator-(const HD_Matrix3x3<T>& aMatrix0, const HD_Matrix3x3<T>& aMatrix1);
template<typename T> HD_Matrix3x3<T> operator*(const HD_Matrix3x3<T>& aMatrix0, const HD_Matrix3x3<T>& aMatrix1);
template<typename T> HD_Matrix3x3<T> operator*(const HD_Matrix3x3<T>& aMatrix, T aScalar);
template<typename T> HD_Matrix3x3<T> operator*(T aScalar, const HD_Matrix3x3<T>& aMatrix);
template<typename T> HD_Vector3<T> operator*(const HD_Vector3<T>& aVector, const HD_Matrix3x3<T>& aMatrix);

template<typename T>
HD_Matrix3x3<T>::HD_Matrix3x3()
	: m11(), m12(), m13(), m21(), m22(), m23(), m31(), m32(), m33()
{
}

template<typename T>
HD_Matrix3x3<T>::HD_Matrix3x3(const HD_Matrix3x3& aMatrix)
{
	(*this) = aMatrix;
}

template<typename T>
HD_Matrix3x3<T>::HD_Matrix3x3(const HD_Matrix4x4<T>& aMatrix)
{
	(*this) = aMatrix;
}

template<typename T>
HD_Matrix3x3<T>::HD_Matrix3x3(std::initializer_list<T> aInitializerList)
{
	(*this) = aInitializerList;
}

template<typename T>
HD_Matrix3x3<T>& HD_Matrix3x3<T>::operator=(const HD_Matrix3x3& aMatrix)
{
	memcpy(this, &aMatrix, 3 * 3 * sizeof(T));
	return *this;
}

template<typename T>
HD_Matrix3x3<T>& HD_Matrix3x3<T>::operator=(const HD_Matrix4x4<T>& aMatrix)
{
	memcpy(&m11, &aMatrix.m11, 3 * sizeof(T));
	memcpy(&m21, &aMatrix.m21, 3 * sizeof(T));
	memcpy(&m31, &aMatrix.m31, 3 * sizeof(T));
	return *this;
}

template<typename T>
HD_Matrix3x3<T>& HD_Matrix3x3<T>::operator=(std::initializer_list<T> aInitializerList)
{
	assert(aInitializerList.size() == 9);
	memcpy(this, aInitializerList.begin(), 3 * 3 * sizeof(T));
	return *this;
}

template<typename T>
T& HD_Matrix3x3<T>::operator()(int aRow, int aCol)
{
	assert(1 <= aRow && aRow <= 3 && 1 <= aCol && aCol <= 3);
	return *(&m11 + ((aRow - 1) * 3) + (aCol - 1));
}

template<typename T>
const T& HD_Matrix3x3<T>::operator()(int aRow, int aCol) const
{
	assert(1 <= aRow && aRow <= 3 && 1 <= aCol && aCol <= 3);
	return *(&m11 + ((aRow - 1) * 3) + (aCol - 1));
}

template<typename T>
HD_Matrix3x3<T>& HD_Matrix3x3<T>::operator+=(const HD_Matrix3x3& aMatrix)
{
	m11 += aMatrix.m11;
	m12 += aMatrix.m12;
	m13 += aMatrix.m13;
	m21 += aMatrix.m21;
	m22 += aMatrix.m22;
	m23 += aMatrix.m23;
	m31 += aMatrix.m31;
	m32 += aMatrix.m32;
	m33 += aMatrix.m33;

	return *this;
}

template<typename T>
HD_Matrix3x3<T>& HD_Matrix3x3<T>::operator-=(const HD_Matrix3x3& aMatrix)
{
	m11 -= aMatrix.m11;
	m12 -= aMatrix.m12;
	m13 -= aMatrix.m13;
	m21 -= aMatrix.m21;
	m22 -= aMatrix.m22;
	m23 -= aMatrix.m23;
	m31 -= aMatrix.m31;
	m32 -= aMatrix.m32;
	m33 -= aMatrix.m33;

	return *this;
}

template<typename T>
HD_Matrix3x3<T>& HD_Matrix3x3<T>::operator*=(const HD_Matrix3x3& aMatrix)
{
	(*this) = (*this) * aMatrix;
	return *this;
}

template<typename T>
void HD_Matrix3x3<T>::ScaleInX(T aScalar)
{
	m11 *= aScalar;
	m12 *= aScalar;
}

template<typename T>
void HD_Matrix3x3<T>::ScaleInY(T aScalar)
{
	m21 *= aScalar;
	m22 *= aScalar;
}

template<typename T>
bool HD_Matrix3x3<T>::operator==(const HD_Matrix3x3& aMatrix) const
{
	return	HD_ARE_FLOAT_VALUES_CLOSE(m11, aMatrix.m11) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m12, aMatrix.m12) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m13, aMatrix.m13) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m21, aMatrix.m21) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m22, aMatrix.m22) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m23, aMatrix.m23) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m31, aMatrix.m31) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m32, aMatrix.m32) &&
			HD_ARE_FLOAT_VALUES_CLOSE(m33, aMatrix.m33);
}

template<typename T>
bool HD_Matrix3x3<T>::operator!=(const HD_Matrix3x3& aMatrix) const
{
	return !((*this) == aMatrix);
}

template<typename T>
HD_Vector2<T> HD_Matrix3x3<T>::GetRightVector() const
{
	return { m11, m12 };
}

template<typename T>
HD_Vector2<T> HD_Matrix3x3<T>::GetUpVector() const
{
	return { m21, m22 };
}

template<typename T>
T HD_Matrix3x3<T>::GetScaleInX() const
{
	T scaleInX = GetRightVector().Length();
	return scaleInX;
}

template<typename T>
T HD_Matrix3x3<T>::GetScaleInY() const
{
	T scaleInY = GetUpVector().Length();
	return scaleInY;
}

template<typename T>
HD_Vector2<T> HD_Matrix3x3<T>::GetScaleInXY() const
{
	T scaleInX = GetScaleInX();
	T scaleInY = GetScaleInY();

	return { scaleInX, scaleInY };
}

template<typename T>
T HD_Matrix3x3<T>::GetRotation() const
{
	HD_Vector2<T> rightVector = GetRightVector();
	return HD_ArcTan2(rightVector.myY, rightVector.myX);
}

template<typename T>
T HD_Matrix3x3<T>::GetPositionX() const
{
	return m31;
}

template<typename T>
T HD_Matrix3x3<T>::GetPositionY() const
{
	return m32;
}

template<typename T>
HD_Vector2<T> HD_Matrix3x3<T>::GetPosition() const
{
	return { m31, m32 };
}

template<typename T>
HD_Matrix3x3<T> HD_Matrix3x3<T>::GetTranspose() const
{
	HD_Matrix3x3 transpose = *this;

	HD_Swap(transpose.m21, transpose.m12);
	HD_Swap(transpose.m31, transpose.m13);
	HD_Swap(transpose.m32, transpose.m23);

	return transpose;
}

template<typename T>
HD_Matrix3x3<T> HD_Matrix3x3<T>::GetFastInverse() const
{
	// Assumes that this matrix is composed as an SRT matrix (scale, rotation, translation), which
	// means the inverse is the inverse of each operation in reverse order: T^-1 * R^-1 * S^-1.

	HD_Matrix3x3 translationInverse = CreateTranslation(-1 * m31, -1 * m32);
	HD_Matrix3x3 scaleInverse = CreateScale(1 / GetScaleInX(), 1 / GetScaleInY());

	// At this point rotationInverse still holds a scaling as well. It's removed by normalizing its
	// right and up vectors. Multiplying by scaleInverse doesn't work since CreateScale assumes no
	// rotation.
	HD_Matrix3x3 rotationInverse = Get2x2().GetTranspose();
	reinterpret_cast<HD_Vector2<T>*>(&rotationInverse.m11)->Normalize();
	reinterpret_cast<HD_Vector2<T>*>(&rotationInverse.m21)->Normalize();

	HD_Matrix3x3 inverse = translationInverse * rotationInverse * scaleInverse;
	return inverse;
}

template<typename T>
HD_Matrix3x3<T> HD_Matrix3x3<T>::Get2x2() const
{
	HD_Matrix3x3 result =
	{
		m11, m12, 0,
		m21, m22, 0,
		0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> HD_Matrix3x3<T>::CreateScale(T aScaleInX, T aScaleInY)
{
	HD_Matrix3x3 result =
	{
		aScaleInX, 0, 0,
		0, aScaleInY, 0,
		0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> HD_Matrix3x3<T>::CreateRotation(T aAngleInRadians)
{
	HD_Matrix3x3 result =
	{
		HD_Cos(aAngleInRadians), HD_Sin(aAngleInRadians), 0,
		-1 * HD_Sin(aAngleInRadians), HD_Cos(aAngleInRadians), 0,
		0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> HD_Matrix3x3<T>::CreateTranslation(T aX, T aY)
{
	HD_Matrix3x3 result = Identity;
	result.m31 = aX;
	result.m32 = aY;

	return result;
}

template<typename T>
HD_Matrix3x3<T> operator+(const HD_Matrix3x3<T>& aMatrix0, const HD_Matrix3x3<T>& aMatrix1)
{
	HD_Matrix3x3 result =
	{
		aMatrix0.m11 + aMatrix1.m11, aMatrix0.m12 + aMatrix1.m12, aMatrix0.m13 + aMatrix1.m13,
		aMatrix0.m21 + aMatrix1.m21, aMatrix0.m22 + aMatrix1.m22, aMatrix0.m23 + aMatrix1.m23,
		aMatrix0.m31 + aMatrix1.m31, aMatrix0.m32 + aMatrix1.m32, aMatrix0.m33 + aMatrix1.m33
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> operator-(const HD_Matrix3x3<T>& aMatrix0, const HD_Matrix3x3<T>& aMatrix1)
{
	HD_Matrix3x3 result =
	{
		aMatrix0.m11 - aMatrix1.m11, aMatrix0.m12 - aMatrix1.m12, aMatrix0.m13 - aMatrix1.m13,
		aMatrix0.m21 - aMatrix1.m21, aMatrix0.m22 - aMatrix1.m22, aMatrix0.m23 - aMatrix1.m23,
		aMatrix0.m31 - aMatrix1.m31, aMatrix0.m32 - aMatrix1.m32, aMatrix0.m33 - aMatrix1.m33
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> operator*(const HD_Matrix3x3<T>& aMatrix0, const HD_Matrix3x3<T>& aMatrix1)
{
	HD_Matrix3x3 result =
	{
		aMatrix0.m11 * aMatrix1.m11 + aMatrix0.m12 * aMatrix1.m21 + aMatrix0.m13 * aMatrix1.m31,
		aMatrix0.m11 * aMatrix1.m12 + aMatrix0.m12 * aMatrix1.m22 + aMatrix0.m13 * aMatrix1.m32,
		aMatrix0.m11 * aMatrix1.m13 + aMatrix0.m12 * aMatrix1.m23 + aMatrix0.m13 * aMatrix1.m33,

		aMatrix0.m21 * aMatrix1.m11 + aMatrix0.m22 * aMatrix1.m21 + aMatrix0.m23 * aMatrix1.m31,
		aMatrix0.m21 * aMatrix1.m12 + aMatrix0.m22 * aMatrix1.m22 + aMatrix0.m23 * aMatrix1.m32,
		aMatrix0.m21 * aMatrix1.m13 + aMatrix0.m22 * aMatrix1.m23 + aMatrix0.m23 * aMatrix1.m33,

		aMatrix0.m31 * aMatrix1.m11 + aMatrix0.m32 * aMatrix1.m21 + aMatrix0.m33 * aMatrix1.m31,
		aMatrix0.m31 * aMatrix1.m12 + aMatrix0.m32 * aMatrix1.m22 + aMatrix0.m33 * aMatrix1.m32,
		aMatrix0.m31 * aMatrix1.m13 + aMatrix0.m32 * aMatrix1.m23 + aMatrix0.m33 * aMatrix1.m33
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> operator*(const HD_Matrix3x3<T>& aMatrix, T aScalar)
{
	HD_Matrix3x3 result =
	{
		aMatrix.m11 * aScalar, aMatrix.m12 * aScalar, aMatrix.m13 * aScalar,
		aMatrix.m21 * aScalar, aMatrix.m22 * aScalar, aMatrix.m23 * aScalar,
		aMatrix.m31 * aScalar, aMatrix.m32 * aScalar, aMatrix.m33 * aScalar
	};

	return result;
}

template<typename T>
HD_Matrix3x3<T> operator*(T aScalar, const HD_Matrix3x3<T>& aMatrix)
{
	return aMatrix * aScalar;
}

template<typename T>
HD_Vector3<T> operator*(const HD_Vector3<T>& aVector, const HD_Matrix3x3<T>& aMatrix)
{
	HD_Vector3 result
	(
		aVector.myX * aMatrix.m11 + aVector.myY * aMatrix.m21 + aVector.myZ * aMatrix.m31,
		aVector.myX * aMatrix.m12 + aVector.myY * aMatrix.m22 + aVector.myZ * aMatrix.m32,
		aVector.myX * aMatrix.m13 + aVector.myY * aMatrix.m23 + aVector.myZ * aMatrix.m33
	);

	return result;
}

typedef HD_Matrix3x3<float> HD_Matrix3x3f;
typedef HD_Matrix3x3<double> HD_Matrix3x3d;

template <> const HD_Matrix3x3<float> HD_Matrix3x3<float>::Identity = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
