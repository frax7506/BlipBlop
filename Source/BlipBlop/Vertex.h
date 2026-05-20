#pragma once

#include "RHIStructs.h"

#include "HD_Vector4.h"

struct Vertex
{
	HD_Vector4f myPosition = { 0.f, 0.f, 0.f, 1.f };
	HD_Vector4f myColor = { 1.f, 1.f, 1.f, 1.f };

	static const HD_GrowingArray<VertexElementDesc> ourDescription;
};
