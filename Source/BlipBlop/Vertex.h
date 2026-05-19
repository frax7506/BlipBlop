#pragma once

#include "RHIStructs.h"

#include "HD_Vector4.h"

struct Vertex
{
	Vertex();
	Vertex(const HD_Vector4f& aPosition, const HD_Vector4f& aColor);

	HD_Vector4f myPosition;
	HD_Vector4f myColor;

	static const HD_GrowingArray <VertexElementDesc> ourDescription;
};
