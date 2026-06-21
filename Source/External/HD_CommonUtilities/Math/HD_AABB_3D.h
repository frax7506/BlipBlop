#pragma once

#include "HD_Vector3.h"

template<typename T>
class HD_AABB_3D
{
public:
	HD_AABB_3D();
	HD_AABB_3D(const HD_Vector3<T>& aMin, const HD_Vector3<T>& aMax);

	bool Contains(const HD_Vector3<T>& aPosition) const;
	bool Intersects(const HD_AABB_3D& aOther) const;

	HD_Vector3<T> myMin;
	HD_Vector3<T> myMax;
};

template<typename T>
HD_AABB_3D<T>::HD_AABB_3D()
{
}

template<typename T>
HD_AABB_3D<T>::HD_AABB_3D(const HD_Vector3<T>& aMin, const HD_Vector3<T>& aMax)
	: myMin(aMin)
	, myMax(aMax)
{
}

template<typename T>
bool HD_AABB_3D<T>::Contains(const HD_Vector3<T>& aPosition) const
{
	const bool xInRange = myMin.myX <= aPosition.myX && aPosition.myX <= myMax.myX;
	const bool yInRange = myMin.myY <= aPosition.myY && aPosition.myY <= myMax.myY;
	const bool zInRange = myMin.myZ <= aPosition.myZ && aPosition.myZ <= myMax.myZ;
	return xInRange && yInRange && zInRange;
}

template<typename T>
bool HD_AABB_3D<T>::Intersects(const HD_AABB_3D& aOther) const
{
	return !(myMax.myX < aOther.myMin.myX ||
		myMin.myX > aOther.myMax.myX ||
		myMax.myY < aOther.myMin.myY ||
		myMin.myY > aOther.myMax.myY ||
		myMax.myZ < aOther.myMin.myZ ||
		myMin.myZ > aOther.myMax.myZ);
}

typedef HD_AABB_3D<float> HD_AABB_3Df;
