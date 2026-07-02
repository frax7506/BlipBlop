#include "HD_Camera.h"

#include <cassert>

HD_Camera::HD_Camera()
	: myToClipSpaceDirty(false)
	, myVerticalFoV(0.f)
	, myLeft(0.f)
	, myRight(0.f)
	, myTop(0.f)
	, myBottom(0.f)
	, myNear(0.f)
	, myFar(0.f)
	, myIsUsingPerspectiveProjection(true)
{
}

void HD_Camera::InitAsPerspectiveCamera(const HD_Vector2ui& aResolution, float aVerticalFoV, float aNear, float aFar)
{
	SetPerspectiveProjection(aResolution, aVerticalFoV, aNear, aFar);
}

void HD_Camera::InitAsOrthographicCamera(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar)
{
	SetOrthographicProjection(aLeft, aRight, aTop, aBottom, aNear, aFar);
}

void HD_Camera::Update()
{
	if (myToClipSpaceDirty)
	{
		CalculateToClipSpace();
		myToClipSpaceDirty = false;
	}
}

void HD_Camera::SetResolution(const HD_Vector2ui& aResolution)
{
	assert(myIsUsingPerspectiveProjection && "Setting the resolution on an orthographic camera has no effect.");

	myResolution = aResolution;
	myToClipSpaceDirty = true;
}

void HD_Camera::SetVerticalFoV(float aVerticalFoV)
{
	assert(myIsUsingPerspectiveProjection && "Setting the field of view on an orthographic camera has no effect.");

	myVerticalFoV = aVerticalFoV;
	myToClipSpaceDirty = true;
}

void HD_Camera::SetPerspectiveProjection(const HD_Vector2ui& aResolution, float aVerticalFoV, float aNear, float aFar)
{
	assert(0.f < aNear && aNear < aFar);

	myResolution = aResolution;
	myVerticalFoV = aVerticalFoV;
	myNear = aNear;
	myFar = aFar;

	myIsUsingPerspectiveProjection = true;
	myToClipSpaceDirty = true;
}

void HD_Camera::SetOrthographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar)
{
	assert(aLeft < aRight && aBottom < aTop && aNear < aFar);

	myLeft = aLeft;
	myRight = aRight;
	myTop = aTop;
	myBottom = aBottom;

	myNear = aNear;
	myFar = aFar;

	myIsUsingPerspectiveProjection = false;
	myToClipSpaceDirty = true;
}

void HD_Camera::SetPosition(const HD_Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void HD_Camera::SetHeading(float aHeading)
{
	myTransform.SetRotationAroundY(aHeading);
}

void HD_Camera::SetPitch(float aPitch)
{
	myTransform.SetRotationAroundX(aPitch);
}

void HD_Camera::SetBank(float aBank)
{
	myTransform.SetRotationAroundZ(aBank);
}

HD_Vector3f HD_Camera::GetPosition() const
{
	return myTransform.GetPosition();
}

HD_Vector3f HD_Camera::GetRotationInHPB() const
{
	return myTransform.GetRotationInHPB();
}

HD_Vector3f HD_Camera::GetRight() const
{
	return myTransform.GetRightVector();
}

HD_Vector3f HD_Camera::GetUp() const
{
	return myTransform.GetUpVector();
}

HD_Vector3f HD_Camera::GetForward() const
{
	return myTransform.GetForwardVector();
}

HD_Vector4f HD_Camera::WorldSpaceToClipSpace(const HD_Vector4f& aPointInWorldSpace) const
{
	HD_Vector4f viewSpace = aPointInWorldSpace * myTransform.GetFastInverse();
	HD_Vector4f clipSpace = viewSpace * myToClipSpace;
	return clipSpace;
}

HD_Vector3f HD_Camera::ClipSpaceToPerspectiveDivide(const HD_Vector4f& aPointInClipSpace) const
{
	HD_Vector3f pointInClipSpaceXYZ(aPointInClipSpace.myX, aPointInClipSpace.myY, aPointInClipSpace.myZ);
	HD_Vector3f postPerspectiveDivide = pointInClipSpaceXYZ / aPointInClipSpace.myW;

	return postPerspectiveDivide;
}

HD_Vector3f HD_Camera::PerspectiveDivideToScreenSpace(const HD_Vector3f& aPointPostPerspectiveDivide) const
{
	float aspectRatio = static_cast<float>(myResolution.myX) / static_cast<float>(myResolution.myY);
	float screenSpaceX = HD_Remap(aPointPostPerspectiveDivide.myX, -1.f, 1.f, 0.f, aspectRatio);
	float screenSpaceY = HD_Remap(aPointPostPerspectiveDivide.myY, -1.f, 1.f, 0.f, 1.f);

	HD_Vector3f screenSpace(screenSpaceX, screenSpaceY, aPointPostPerspectiveDivide.myZ);
	return screenSpace;
}

void HD_Camera::CalculateToClipSpace()
{
	myToClipSpace = HD_Matrix4x4f::Identity;

	if (myIsUsingPerspectiveProjection)
	{
		float resolutionWidth = static_cast<float>(myResolution.myX);
		float resolutionHeight = static_cast<float>(myResolution.myY);

		myToClipSpace(1, 1) = (resolutionHeight / resolutionWidth) * (1.f / HD_Tan(myVerticalFoV / 2.f));
		myToClipSpace(2, 2) = 1.f / HD_Tan(myVerticalFoV / 2.f);
		myToClipSpace(3, 3) = myFar / (myFar - myNear);
		myToClipSpace(3, 4) = 1.f;
		myToClipSpace(4, 3) = (-1.f * myNear) * (myFar / (myFar - myNear));
		myToClipSpace(4, 4) = 0.f;
	}
	else
	{
		myToClipSpace(1, 1) = 2.f / (myRight - myLeft);
		myToClipSpace(2, 2) = 2.f / (myBottom - myTop);
		myToClipSpace(3, 3) = 1.f / (myFar - myNear);

		myToClipSpace(4, 1) = (-1.f * (myRight + myLeft)) / (myRight - myLeft);
		myToClipSpace(4, 2) = (-1.f * (myBottom + myTop)) / (myBottom - myTop);
		myToClipSpace(4, 3) = (-1.f * myNear) / (myFar - myNear);
	}
}
