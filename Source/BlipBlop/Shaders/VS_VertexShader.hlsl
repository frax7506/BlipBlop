#include "Common.hlsli"

struct Vertex
{
	float4 myPosition : POSITION;
	float4 myColor : COLOR;
};

VSToPs main(Vertex aVertex)
{
	VSToPs result;
	result.myPosition = aVertex.myPosition;
	result.myColor = aVertex.myColor;
	
	return result;
}
