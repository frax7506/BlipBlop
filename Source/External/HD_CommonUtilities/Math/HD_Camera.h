#pragma once

#include "HD_Matrix.h"

class HD_Camera
{
public:
	HD_Camera();

	void InitAsPerspectiveCamera(const HD_Vector2ui& aResolution, float aVerticalFoV, float aNear, float aFar);
	void InitAsOrthographicCamera(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar);

	void Update();

	void SetResolution(const HD_Vector2ui& aResolution);
	void SetVerticalFoV(float aVerticalFoV);

	void SetPerspectiveProjection(const HD_Vector2ui& aResolution, float aVerticalFoV, float aNear, float aFar);
	void SetOrthographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar);

	void SetPosition(const HD_Vector3f& aPosition);
	void SetHeading(float aHeading);
	void SetPitch(float aPitch);
	void SetBank(float aBank);

	HD_Vector3f GetPosition() const;
	HD_Vector3f GetRotationInHPB() const;

	HD_Vector3f GetRight() const;
	HD_Vector3f GetUp() const;
	HD_Vector3f GetForward() const;

	HD_Vector4f WorldSpaceToClipSpace(const HD_Vector4f& aPointInWorldSpace) const;
	HD_Vector3f ClipSpaceToPerspectiveDivide(const HD_Vector4f& aPointInClipSpace) const;
	HD_Vector3f PerspectiveDivideToScreenSpace(const HD_Vector3f& aPointPostPerspectiveDivide) const;

private:
	void CalculateToClipSpace();

	HD_Matrix4x4f myTransform;
	HD_Matrix4x4f myToClipSpace;
	bool myToClipSpaceDirty;

	// Perspective variables
	HD_Vector2ui myResolution;
	float myVerticalFoV;

	// Orthographic variables
	float myLeft;
	float myRight;
	float myTop;
	float myBottom;

	// Both perspective and orthographic variables
	float myNear;
	float myFar;

	bool myIsUsingPerspectiveProjection;
};
