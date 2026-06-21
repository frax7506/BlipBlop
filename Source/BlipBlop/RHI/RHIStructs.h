#pragma once

#include "HD_String.h"

struct Viewport
{
	f32 myTopLeftX = 0.f;
	f32 myTopLeftY = 0.f;
	f32 myWidth = 0.f;
	f32 myHeight = 0.f;
	f32 myMinDepth = 0.f;
	f32 myMaxDepth = 1.f;
};

enum class BufferType : u8
{
	Unknown,
	VertexBuffer
};

enum class Topology : u32
{
	Undefined = 0,
	TriangleList = 4,
};

struct VertexElementDesc
{
	HD_String mySemantic;
	u32 mySemanticIndex = 0;
	u32 myFormat = 0;
};
