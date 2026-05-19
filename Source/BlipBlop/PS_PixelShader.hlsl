#include "Common.hlsli"

float4 main(VSToPs aPixel) : SV_TARGET
{
	return aPixel.myColor;
}
