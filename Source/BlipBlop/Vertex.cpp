#include "stdafx.h"
#include "Vertex.h"

const HD_GrowingArray<VertexElementDesc> Vertex::ourDescription =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT }
};
