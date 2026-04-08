#pragma once

#include "HD_Vector2.h"

class HD_AABB_2D
{
public:
	HD_AABB_2D()
		: myCenter()
		, myHalfSize(0.f)
	{
	}

	HD_AABB_2D(const HD_Vector2f& aCenter, float aSize)
		: myCenter(aCenter)
		, myHalfSize(aSize / 2.f)
	{
	}

	bool Contains(const HD_Vector2f& aPosition) const
	{
		const bool xInRange = aPosition.myX >= (myCenter.myX - myHalfSize) && aPosition.myX <= (myCenter.myX + myHalfSize);
		const bool yInRange = aPosition.myY >= (myCenter.myY - myHalfSize) && aPosition.myY <= (myCenter.myY + myHalfSize);
		return xInRange && yInRange;
	}

	bool Intersects(const HD_AABB_2D& aOther) const
	{
		return	!(aOther.myCenter.myX - aOther.myHalfSize > myCenter.myX + myHalfSize ||
			aOther.myCenter.myX + aOther.myHalfSize < myCenter.myX - myHalfSize ||
			aOther.myCenter.myY - aOther.myHalfSize > myCenter.myY + myHalfSize ||
			aOther.myCenter.myY + aOther.myHalfSize < myCenter.myY - myHalfSize);
	}

	HD_Vector2f myCenter;
	float myHalfSize;
};
