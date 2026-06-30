#pragma once

#include "HD_Swap.h"

#include <cassert>
#include <cstring>
#include <initializer_list>

template<typename T>
class HD_Vector3;

template<typename T>
class HD_Vector4;

template<typename T>
class HD_Matrix3x3;

template<typename T>
class HD_Matrix4x4
{
public:
	HD_Matrix4x4();
	HD_Matrix4x4(const HD_Matrix4x4& aMatrix);
	HD_Matrix4x4(const HD_Matrix3x3<T>& aMatrix);
	HD_Matrix4x4(std::initializer_list<T> aInitializerList);

	HD_Matrix4x4& operator=(const HD_Matrix4x4& aMatrix);
	HD_Matrix4x4& operator=(const HD_Matrix3x3<T>& aMatrix);
	HD_Matrix4x4& operator=(std::initializer_list<T> aInitializerList);

	HD_Matrix4x4& operator+=(const HD_Matrix4x4& aMatrix);
	HD_Matrix4x4& operator-=(const HD_Matrix4x4& aMatrix);
	HD_Matrix4x4& operator*=(const HD_Matrix4x4& aMatrix);

	void SetScaleInX(T aScalar);
	void SetScaleInY(T aScalar);
	void SetScaleInZ(T aScalar);

	void SetRotationAroundX(T aAngleInRadians);
	void SetRotationAroundY(T aAngleInRadians);
	void SetRotationAroundZ(T aAngleInRadians);

	void SetPositionX(T aValue);
	void SetPositionY(T aValue);
	void SetPositionZ(T aValue);
	void SetPosition(const HD_Vector3<T>& aPosition);

	T& operator()(int aRow, int aCol);
	const T& operator()(int aRow, int aCol) const;

	HD_Vector3<T> GetRightVector() const;
	HD_Vector3<T> GetUpVector() const;
	HD_Vector3<T> GetForwardVector() const;

	T GetScaleInX() const;
	T GetScaleInY() const;
	T GetScaleInZ() const;
	HD_Vector3<T> GetScaleInXYZ() const;

	T GetRotationAroundX() const;
	T GetRotationAroundY() const;
	T GetRotationAroundZ() const;
	HD_Vector3<T> GetRotationInXYZ() const;
	HD_Vector3<T> GetRotationInHPB() const;

	T GetPositionX() const;
	T GetPositionY() const;
	T GetPositionZ() const;
	HD_Vector3<T> GetPosition() const;

	HD_Matrix4x4 GetTranspose() const;
	HD_Matrix4x4 GetFastInverse() const;
	HD_Matrix4x4 Get3x3() const;

	T m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;

public:
	static HD_Matrix4x4 CreateScale(T aScaleInX, T aScaleInY, T aScaleInZ);
	static HD_Matrix4x4 CreateRotationAroundX(T aAngleInRadians);
	static HD_Matrix4x4 CreateRotationAroundY(T aAngleInRadians);
	static HD_Matrix4x4 CreateRotationAroundZ(T aAngleInRadians);
	static HD_Matrix4x4 CreateTranslation(T aX, T aY, T aZ);

	static const HD_Matrix4x4 Identity;
};

template<typename T> HD_Matrix4x4<T> operator+(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1);
template<typename T> HD_Matrix4x4<T> operator-(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1);
template<typename T> HD_Matrix4x4<T> operator*(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1);
template<typename T> HD_Matrix4x4<T> operator*(const HD_Matrix4x4<T>& aMatrix, T aScalar);
template<typename T> HD_Matrix4x4<T> operator*(T aScalar, const HD_Matrix4x4<T>& aMatrix);
template<typename T> HD_Vector4<T> operator*(const HD_Vector4<T>& aVector, const HD_Matrix4x4<T>& aMatrix);
template<typename T> bool operator==(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1);
template<typename T> bool operator!=(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1);

template<typename T>
HD_Matrix4x4<T>::HD_Matrix4x4()
{
	(*this) = Identity;
}

template<typename T>
HD_Matrix4x4<T>::HD_Matrix4x4(const HD_Matrix4x4& aMatrix)
{
	(*this) = aMatrix;
}

template<typename T>
HD_Matrix4x4<T>::HD_Matrix4x4(const HD_Matrix3x3<T>& aMatrix)
	: m14(), m24(), m34(), m41(), m42(), m43(), m44(1)
{
	(*this) = aMatrix;
}

template<typename T>
HD_Matrix4x4<T>::HD_Matrix4x4(std::initializer_list<T> aInitializerList)
{
	(*this) = aInitializerList;
}

template<typename T>
HD_Matrix4x4<T>& HD_Matrix4x4<T>::operator=(const HD_Matrix4x4& aMatrix)
{
	memcpy(&m11, &aMatrix, 4 * 4 * sizeof(T));
	return *this;
}

template<typename T>
HD_Matrix4x4<T>& HD_Matrix4x4<T>::operator=(const HD_Matrix3x3<T>& aMatrix)
{
	memcpy(&m11, &aMatrix.m11, 3 * sizeof(T));
	memcpy(&m21, &aMatrix.m21, 3 * sizeof(T));
	memcpy(&m31, &aMatrix.m31, 3 * sizeof(T));
	return *this;
}

template<typename T>
HD_Matrix4x4<T>& HD_Matrix4x4<T>::operator=(std::initializer_list<T> aInitializerList)
{
	assert(aInitializerList.size() == 16);
	memcpy(&m11, aInitializerList.begin(), 4 * 4 * sizeof(T));
	return *this;
}

template<typename T>
HD_Matrix4x4<T>& HD_Matrix4x4<T>::operator+=(const HD_Matrix4x4& aMatrix)
{
	m11 += aMatrix.m11;
	m12 += aMatrix.m12;
	m13 += aMatrix.m13;
	m14 += aMatrix.m14;

	m21 += aMatrix.m21;
	m22 += aMatrix.m22;
	m23 += aMatrix.m23;
	m24 += aMatrix.m24;

	m31 += aMatrix.m31;
	m32 += aMatrix.m32;
	m33 += aMatrix.m33;
	m34 += aMatrix.m34;

	m41 += aMatrix.m41;
	m42 += aMatrix.m42;
	m43 += aMatrix.m43;
	m44 += aMatrix.m44;

	return *this;
}

template<typename T>
HD_Matrix4x4<T>& HD_Matrix4x4<T>::operator-=(const HD_Matrix4x4& aMatrix)
{
	m11 -= aMatrix.m11;
	m12 -= aMatrix.m12;
	m13 -= aMatrix.m13;
	m14 -= aMatrix.m14;

	m21 -= aMatrix.m21;
	m22 -= aMatrix.m22;
	m23 -= aMatrix.m23;
	m24 -= aMatrix.m24;

	m31 -= aMatrix.m31;
	m32 -= aMatrix.m32;
	m33 -= aMatrix.m33;
	m34 -= aMatrix.m34;

	m41 -= aMatrix.m41;
	m42 -= aMatrix.m42;
	m43 -= aMatrix.m43;
	m44 -= aMatrix.m44;

	return *this;
}

template<typename T>
HD_Matrix4x4<T>& HD_Matrix4x4<T>::operator*=(const HD_Matrix4x4& aMatrix)
{
	HD_Matrix4x4 result;

	result.m11 = m11 * aMatrix.m11 + m12 * aMatrix.m21 + m13 * aMatrix.m31 + m14 * aMatrix.m41;
	result.m12 = m11 * aMatrix.m12 + m12 * aMatrix.m22 + m13 * aMatrix.m32 + m14 * aMatrix.m42;
	result.m13 = m11 * aMatrix.m13 + m12 * aMatrix.m23 + m13 * aMatrix.m33 + m14 * aMatrix.m43;
	result.m14 = m11 * aMatrix.m14 + m12 * aMatrix.m24 + m13 * aMatrix.m34 + m14 * aMatrix.m44;

	result.m21 = m21 * aMatrix.m11 + m22 * aMatrix.m21 + m23 * aMatrix.m31 + m24 * aMatrix.m41;
	result.m22 = m21 * aMatrix.m12 + m22 * aMatrix.m22 + m23 * aMatrix.m32 + m24 * aMatrix.m42;
	result.m23 = m21 * aMatrix.m13 + m22 * aMatrix.m23 + m23 * aMatrix.m33 + m24 * aMatrix.m43;
	result.m24 = m21 * aMatrix.m14 + m22 * aMatrix.m24 + m23 * aMatrix.m34 + m24 * aMatrix.m44;

	result.m31 = m31 * aMatrix.m11 + m32 * aMatrix.m21 + m33 * aMatrix.m31 + m34 * aMatrix.m41;
	result.m32 = m31 * aMatrix.m12 + m32 * aMatrix.m22 + m33 * aMatrix.m32 + m34 * aMatrix.m42;
	result.m33 = m31 * aMatrix.m13 + m32 * aMatrix.m23 + m33 * aMatrix.m33 + m34 * aMatrix.m43;
	result.m34 = m31 * aMatrix.m14 + m32 * aMatrix.m24 + m33 * aMatrix.m34 + m34 * aMatrix.m44;

	result.m41 = m41 * aMatrix.m11 + m42 * aMatrix.m21 + m43 * aMatrix.m31 + m44 * aMatrix.m41;
	result.m42 = m41 * aMatrix.m12 + m42 * aMatrix.m22 + m43 * aMatrix.m32 + m44 * aMatrix.m42;
	result.m43 = m41 * aMatrix.m13 + m42 * aMatrix.m23 + m43 * aMatrix.m33 + m44 * aMatrix.m43;
	result.m44 = m41 * aMatrix.m14 + m42 * aMatrix.m24 + m43 * aMatrix.m34 + m44 * aMatrix.m44;

	(*this) = result;

	return *this;
}

template<typename T>
void HD_Matrix4x4<T>::SetScaleInX(T aScalar)
{
	HD_Vector3<T> rightVector = GetRightVector();
	rightVector.SetLength(aScalar);
	m11 = rightVector.myX;
	m12 = rightVector.myY;
	m13 = rightVector.myZ;
}

template<typename T>
void HD_Matrix4x4<T>::SetScaleInY(T aScalar)
{
	HD_Vector3<T> upVector = GetUpVector();
	upVector.SetLength(aScalar);
	m21 = upVector.myX;
	m22 = upVector.myY;
	m23 = upVector.myZ;
}

template<typename T>
void HD_Matrix4x4<T>::SetScaleInZ(T aScalar)
{
	HD_Vector3<T> forwardVector = GetForwardVector();
	forwardVector.SetLength(aScalar);
	m31 = forwardVector.myX;
	m32 = forwardVector.myY;
	m33 = forwardVector.myZ;
}

template<typename T>
void HD_Matrix4x4<T>::SetRotationAroundX(T aAngleInRadians)
{
	T currentRotationAroundX = GetRotationAroundX();
	T angleDiff = aAngleInRadians - currentRotationAroundX;

	HD_Matrix4x4 rotation = CreateRotationAroundX(angleDiff);
	(*this) *= rotation;
}

template<typename T>
void HD_Matrix4x4<T>::SetRotationAroundY(T aAngleInRadians)
{
	T currentRotationAroundY = GetRotationAroundY();
	T angleDiff = aAngleInRadians - currentRotationAroundY;

	HD_Matrix4x4 rotation = CreateRotationAroundY(angleDiff);
	(*this) *= rotation;
}

template<typename T>
void HD_Matrix4x4<T>::SetRotationAroundZ(T aAngleInRadians)
{
	T currentRotationAroundZ = GetRotationAroundZ();
	T angleDiff = aAngleInRadians - currentRotationAroundZ;

	HD_Matrix4x4 rotation = CreateRotationAroundZ(angleDiff);
	(*this) *= rotation;
}

template<typename T>
void HD_Matrix4x4<T>::SetPositionX(T aValue)
{
	m41 = aValue;
}

template<typename T>
void HD_Matrix4x4<T>::SetPositionY(T aValue)
{
	m42 = aValue;
}

template<typename T>
void HD_Matrix4x4<T>::SetPositionZ(T aValue)
{
	m43 = aValue;
}

template<typename T>
void HD_Matrix4x4<T>::SetPosition(const HD_Vector3<T>& aPosition)
{
	m41 = aPosition.myX;
	m42 = aPosition.myY;
	m43 = aPosition.myZ;
}

template<typename T>
T& HD_Matrix4x4<T>::operator()(int aRow, int aCol)
{
	assert(1 <= aRow && aRow <= 4 && 1 <= aCol && aCol <= 4);
	return *(&m11 + ((aRow - 1) * 4) + (aCol - 1));
}

template<typename T>
const T& HD_Matrix4x4<T>::operator()(int aRow, int aCol) const
{
	assert(1 <= aRow && aRow <= 4 && 1 <= aCol && aCol <= 4);
	return *(&m11 + ((aRow - 1) * 4) + (aCol - 1));
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetRightVector() const
{
	return { m11, m12, m13 };
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetUpVector() const
{
	return { m21, m22, m23 };
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetForwardVector() const
{
	return { m31, m32, m33 };
}

template<typename T>
T HD_Matrix4x4<T>::GetScaleInX() const
{
	T scaleInX = GetRightVector().Length();
	return scaleInX;
}

template<typename T>
T HD_Matrix4x4<T>::GetScaleInY() const
{
	T scaleInY = GetUpVector().Length();
	return scaleInY;
}

template<typename T>
T HD_Matrix4x4<T>::GetScaleInZ() const
{
	T scaleInZ = GetForwardVector().Length();
	return scaleInZ;
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetScaleInXYZ() const
{
	T scaleInX = GetScaleInX();
	T scaleInY = GetScaleInY();
	T scaleInZ = GetScaleInZ();

	return { scaleInX, scaleInY, scaleInZ };
}

template<typename T>
T HD_Matrix4x4<T>::GetRotationAroundX() const
{
	HD_Vector3<T> xyz = GetRotationInXYZ();
	return xyz.myX;
}

template<typename T>
T HD_Matrix4x4<T>::GetRotationAroundY() const
{
	HD_Vector3<T> xyz = GetRotationInXYZ();
	return xyz.myY;
}

template<typename T>
T HD_Matrix4x4<T>::GetRotationAroundZ() const
{
	HD_Vector3<T> xyz = GetRotationInXYZ();
	return xyz.myZ;
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetRotationInXYZ() const
{
	HD_Vector3<T> hpb = GetRotationInHPB();
	HD_Vector3<T> xyz(hpb.myY, hpb.myX, hpb.myZ);
	return xyz;
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetRotationInHPB() const
{
	// From https://gamemath.com/book/orient.html#matrix_to_euler

	T heading = 0;
	T pitch = 0;
	T bank = 0;

	// Extract pitch from m32, being careful for domain errors with
	// asin(). We could have values slightly out of range due to
	// floating point arithmetic.

	T sinOfPitch = -1 * m32;
	if (sinOfPitch <= -1)
	{
		pitch = F_PI_HALF;
	}
	else if (sinOfPitch >= 1)
	{
		pitch = -1 * F_PI_HALF;
	}
	else
	{
		pitch = HD_ArcSin(sinOfPitch);
	}

	// Check for the Gimbal lock case, giving a slight tolerance
	// for numerical imprecision.

	if (HD_Abs(sinOfPitch) > 0.9999)
	{
		// We are looking straight up or down.
		// Set bank to zero and just set heading.
		bank = 0;
		heading = HD_ArcTan2(-1 * m13, m11);
	}
	else
	{
		// Compute heading from m13 and m33
		heading = HD_ArcTan2(m31, m33);

		// Compute bank from m21 and m22
		bank = HD_ArcTan2(m12, m22);
	}

	return { heading, pitch, bank };
}

template<typename T>
T HD_Matrix4x4<T>::GetPositionX() const
{
	return m41;
}

template<typename T>
T HD_Matrix4x4<T>::GetPositionY() const
{
	return m42;
}

template<typename T>
T HD_Matrix4x4<T>::GetPositionZ() const
{
	return m43;
}

template<typename T>
HD_Vector3<T> HD_Matrix4x4<T>::GetPosition() const
{
	return { m41, m42, m43 };
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::GetTranspose() const
{
	HD_Matrix4x4 transpose = *this;

	HD_Swap(transpose.m21, transpose.m12);
	HD_Swap(transpose.m31, transpose.m13);
	HD_Swap(transpose.m32, transpose.m23);
	HD_Swap(transpose.m41, transpose.m14);
	HD_Swap(transpose.m42, transpose.m24);
	HD_Swap(transpose.m43, transpose.m34);

	return transpose;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::GetFastInverse() const
{
	// Assumes that this matrix is composed as an SRT matrix (scale, rotation, translation), which
	// means the inverse is the inverse of each operation in reverse order: T^-1 * R^-1 * S^-1.

	HD_Matrix4x4 translationInverse = CreateTranslation(-1 * m41, -1 * m42, -1 * m43);
	HD_Matrix4x4 scaleInverse = CreateScale(1 / GetScaleInX(), 1 / GetScaleInY(), 1 / GetScaleInZ());

	// At this point rotationInverse still holds a scaling as well. It's removed by normalizing its
	// right, up and forward vectors. Multiplying by scaleInverse doesn't work since CreateScale assumes no
	// rotation.
	HD_Matrix4x4 rotationInverse = Get3x3().GetTranspose();
	reinterpret_cast<HD_Vector3<T>*>(&rotationInverse.m11)->Normalize();
	reinterpret_cast<HD_Vector3<T>*>(&rotationInverse.m21)->Normalize();
	reinterpret_cast<HD_Vector3<T>*>(&rotationInverse.m31)->Normalize();

	HD_Matrix4x4 inverse = translationInverse * rotationInverse * scaleInverse;
	return inverse;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::Get3x3() const
{
	HD_Matrix4x4 result =
	{
		m11, m12, m13, 0,
		m21, m22, m23, 0,
		m31, m32, m33, 0,
		0, 0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::CreateScale(T aScaleInX, T aScaleInY, T aScaleInZ)
{
	HD_Matrix4x4 result =
	{
		aScaleInX, 0, 0, 0,
		0, aScaleInY, 0, 0,
		0, 0, aScaleInZ, 0,
		0, 0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
{
	HD_Matrix4x4 result =
	{
		1, 0, 0, 0,
		0, HD_Cos(aAngleInRadians), HD_Sin(aAngleInRadians), 0,
		0, -1 * HD_Sin(aAngleInRadians), HD_Cos(aAngleInRadians), 0,
		0, 0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
{
	HD_Matrix4x4 result =
	{
		HD_Cos(aAngleInRadians), 0, -1 * HD_Sin(aAngleInRadians), 0,
		0, 1, 0, 0,
		HD_Sin(aAngleInRadians), 0, HD_Cos(aAngleInRadians), 0,
		0, 0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
{
	HD_Matrix4x4 result =
	{
		HD_Cos(aAngleInRadians), HD_Sin(aAngleInRadians), 0, 0,
		-1 * HD_Sin(aAngleInRadians), HD_Cos(aAngleInRadians), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> HD_Matrix4x4<T>::CreateTranslation(T aX, T aY, T aZ)
{
	HD_Matrix4x4 result = Identity;
	result.m41 = aX;
	result.m42 = aY;
	result.m43 = aZ;

	return result;
}

template<typename T>
HD_Matrix4x4<T> operator+(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1)
{
	HD_Matrix4x4<T> result =
	{
		aMatrix0.m11 + aMatrix1.m11, aMatrix0.m12 + aMatrix1.m12, aMatrix0.m13 + aMatrix1.m13, aMatrix0.m14 + aMatrix1.m14,
		aMatrix0.m21 + aMatrix1.m21, aMatrix0.m22 + aMatrix1.m22, aMatrix0.m23 + aMatrix1.m23, aMatrix0.m24 + aMatrix1.m24,
		aMatrix0.m31 + aMatrix1.m31, aMatrix0.m32 + aMatrix1.m32, aMatrix0.m33 + aMatrix1.m33, aMatrix0.m34 + aMatrix1.m34,
		aMatrix0.m41 + aMatrix1.m41, aMatrix0.m42 + aMatrix1.m42, aMatrix0.m43 + aMatrix1.m43, aMatrix0.m44 + aMatrix1.m44
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> operator-(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1)
{
	HD_Matrix4x4<T> result =
	{
		aMatrix0.m11 - aMatrix1.m11, aMatrix0.m12 - aMatrix1.m12, aMatrix0.m13 - aMatrix1.m13, aMatrix0.m14 - aMatrix1.m14,
		aMatrix0.m21 - aMatrix1.m21, aMatrix0.m22 - aMatrix1.m22, aMatrix0.m23 - aMatrix1.m23, aMatrix0.m24 - aMatrix1.m24,
		aMatrix0.m31 - aMatrix1.m31, aMatrix0.m32 - aMatrix1.m32, aMatrix0.m33 - aMatrix1.m33, aMatrix0.m34 - aMatrix1.m34,
		aMatrix0.m41 - aMatrix1.m41, aMatrix0.m42 - aMatrix1.m42, aMatrix0.m43 - aMatrix1.m43, aMatrix0.m44 - aMatrix1.m44
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> operator*(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1)
{
	HD_Matrix4x4<T> result =
	{
		aMatrix0.m11 * aMatrix1.m11 + aMatrix0.m12 * aMatrix1.m21 + aMatrix0.m13 * aMatrix1.m31 + aMatrix0.m14 * aMatrix1.m41,
		aMatrix0.m11 * aMatrix1.m12 + aMatrix0.m12 * aMatrix1.m22 + aMatrix0.m13 * aMatrix1.m32 + aMatrix0.m14 * aMatrix1.m42,
		aMatrix0.m11 * aMatrix1.m13 + aMatrix0.m12 * aMatrix1.m23 + aMatrix0.m13 * aMatrix1.m33 + aMatrix0.m14 * aMatrix1.m43,
		aMatrix0.m11 * aMatrix1.m14 + aMatrix0.m12 * aMatrix1.m24 + aMatrix0.m13 * aMatrix1.m34 + aMatrix0.m14 * aMatrix1.m44,

		aMatrix0.m21 * aMatrix1.m11 + aMatrix0.m22 * aMatrix1.m21 + aMatrix0.m23 * aMatrix1.m31 + aMatrix0.m24 * aMatrix1.m41,
		aMatrix0.m21 * aMatrix1.m12 + aMatrix0.m22 * aMatrix1.m22 + aMatrix0.m23 * aMatrix1.m32 + aMatrix0.m24 * aMatrix1.m42,
		aMatrix0.m21 * aMatrix1.m13 + aMatrix0.m22 * aMatrix1.m23 + aMatrix0.m23 * aMatrix1.m33 + aMatrix0.m24 * aMatrix1.m43,
		aMatrix0.m21 * aMatrix1.m14 + aMatrix0.m22 * aMatrix1.m24 + aMatrix0.m23 * aMatrix1.m34 + aMatrix0.m24 * aMatrix1.m44,

		aMatrix0.m31 * aMatrix1.m11 + aMatrix0.m32 * aMatrix1.m21 + aMatrix0.m33 * aMatrix1.m31 + aMatrix0.m34 * aMatrix1.m41,
		aMatrix0.m31 * aMatrix1.m12 + aMatrix0.m32 * aMatrix1.m22 + aMatrix0.m33 * aMatrix1.m32 + aMatrix0.m34 * aMatrix1.m42,
		aMatrix0.m31 * aMatrix1.m13 + aMatrix0.m32 * aMatrix1.m23 + aMatrix0.m33 * aMatrix1.m33 + aMatrix0.m34 * aMatrix1.m43,
		aMatrix0.m31 * aMatrix1.m14 + aMatrix0.m32 * aMatrix1.m24 + aMatrix0.m33 * aMatrix1.m34 + aMatrix0.m34 * aMatrix1.m44,

		aMatrix0.m41 * aMatrix1.m11 + aMatrix0.m42 * aMatrix1.m21 + aMatrix0.m43 * aMatrix1.m31 + aMatrix0.m44 * aMatrix1.m41,
		aMatrix0.m41 * aMatrix1.m12 + aMatrix0.m42 * aMatrix1.m22 + aMatrix0.m43 * aMatrix1.m32 + aMatrix0.m44 * aMatrix1.m42,
		aMatrix0.m41 * aMatrix1.m13 + aMatrix0.m42 * aMatrix1.m23 + aMatrix0.m43 * aMatrix1.m33 + aMatrix0.m44 * aMatrix1.m43,
		aMatrix0.m41 * aMatrix1.m14 + aMatrix0.m42 * aMatrix1.m24 + aMatrix0.m43 * aMatrix1.m34 + aMatrix0.m44 * aMatrix1.m44
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> operator*(const HD_Matrix4x4<T>& aMatrix, T aScalar)
{
	HD_Matrix4x4<T> result =
	{
		aMatrix.m11 * aScalar, aMatrix.m12 * aScalar, aMatrix.m13 * aScalar, aMatrix.m14 * aScalar,
		aMatrix.m21 * aScalar, aMatrix.m22 * aScalar, aMatrix.m23 * aScalar, aMatrix.m24 * aScalar,
		aMatrix.m31 * aScalar, aMatrix.m32 * aScalar, aMatrix.m33 * aScalar, aMatrix.m34 * aScalar,
		aMatrix.m41 * aScalar, aMatrix.m42 * aScalar, aMatrix.m43 * aScalar, aMatrix.m44 * aScalar
	};

	return result;
}

template<typename T>
HD_Matrix4x4<T> operator*(T aScalar, const HD_Matrix4x4<T>& aMatrix)
{
	return aMatrix * aScalar;
}

template<typename T>
HD_Vector4<T> operator*(const HD_Vector4<T>& aVector, const HD_Matrix4x4<T>& aMatrix)
{
	HD_Vector4<T> result
	(
		aVector.myX * aMatrix.m11 + aVector.myY * aMatrix.m21 + aVector.myZ * aMatrix.m31 + aVector.myW * aMatrix.m41,
		aVector.myX * aMatrix.m12 + aVector.myY * aMatrix.m22 + aVector.myZ * aMatrix.m32 + aVector.myW * aMatrix.m42,
		aVector.myX * aMatrix.m13 + aVector.myY * aMatrix.m23 + aVector.myZ * aMatrix.m33 + aVector.myW * aMatrix.m43,
		aVector.myX * aMatrix.m14 + aVector.myY * aMatrix.m24 + aVector.myZ * aMatrix.m34 + aVector.myW * aMatrix.m44
	);

	return result;
}

template<typename T>
bool operator==(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1)
{
	return	HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m11, aMatrix1.m11) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m12, aMatrix1.m12) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m13, aMatrix1.m13) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m14, aMatrix1.m14) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m21, aMatrix1.m21) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m22, aMatrix1.m22) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m23, aMatrix1.m23) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m24, aMatrix1.m24) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m31, aMatrix1.m31) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m32, aMatrix1.m32) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m33, aMatrix1.m33) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m34, aMatrix1.m34) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m41, aMatrix1.m41) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m42, aMatrix1.m42) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m43, aMatrix1.m43) &&
		HD_ARE_FLOAT_VALUES_CLOSE(aMatrix0.m44, aMatrix1.m44);
}

template<typename T>
bool operator!=(const HD_Matrix4x4<T>& aMatrix0, const HD_Matrix4x4<T>& aMatrix1)
{
	return !(aMatrix0 == aMatrix1);
}

typedef HD_Matrix4x4<float> HD_Matrix4x4f;
typedef HD_Matrix4x4<double> HD_Matrix4x4d;

template<> const HD_Matrix4x4<float> HD_Matrix4x4<float>::Identity = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
template<> const HD_Matrix4x4<double> HD_Matrix4x4<double>::Identity = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
