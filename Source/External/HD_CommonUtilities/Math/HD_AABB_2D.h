#pragma once

#include "HD_Vector2.h"

template<typename T>
class HD_AABB_2D
{
public:
	HD_AABB_2D();
	HD_AABB_2D(const HD_Vector2<T>& aMin, const HD_Vector2<T>& aMax);

	bool Contains(const HD_Vector2<T>& aPosition) const;
	bool Intersects(const HD_AABB_2D& aOther) const;

	HD_Vector2<T> myMin;
	HD_Vector2<T> myMax;
};

template<typename T>
HD_AABB_2D<T>::HD_AABB_2D()
{
}

template<typename T>
HD_AABB_2D<T>::HD_AABB_2D(const HD_Vector2<T>& aMin, const HD_Vector2<T>& aMax)
	: myMin(aMin)
	, myMax(aMax)
{
}

template<typename T>
bool HD_AABB_2D<T>::Contains(const HD_Vector2<T>& aPosition) const
{
	const bool xInRange = myMin.myX <= aPosition.myX && aPosition.myX <= myMax.myX;
	const bool yInRange = myMin.myY <= aPosition.myY && aPosition.myY <= myMax.myY;
	return xInRange && yInRange;
}

template<typename T>
bool HD_AABB_2D<T>::Intersects(const HD_AABB_2D& aOther) const
{
	return !(myMax.myX < aOther.myMin.myX ||
		myMin.myX > aOther.myMax.myX ||
		myMax.myY < aOther.myMin.myY ||
		myMin.myY > aOther.myMax.myY);
}

typedef HD_AABB_2D<float> HD_AABB_2Df;
