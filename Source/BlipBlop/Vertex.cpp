#include "stdafx.h"
#include "Vertex.h"

const HD_GrowingArray<VertexElementDesc> Vertex::ourDescription =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT }
};

Vertex::Vertex()
	: myPosition(0.f, 0.f, 0.f, 1.f)
	, myColor(1.f, 1.f, 1.f, 1.f)
{
}

Vertex::Vertex(const HD_Vector4f& aPosition, const HD_Vector4f& aColor)
	: myPosition(aPosition)
	, myColor(aColor)
{
}
