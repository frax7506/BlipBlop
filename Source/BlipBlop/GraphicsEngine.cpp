#include "stdafx.h"
#include "GraphicsEngine.h"

#include "Vertex.h"

/*
 * GraphicsEngine handles rendering of a scene.
 * Handles culling using whatever cameras it's told to use.
 * Renders onto a user-provided render target and depth.
 */

GraphicsEngine& GraphicsEngine::Get()
{
	static GraphicsEngine myInstance;
	return myInstance;
}

bool GraphicsEngine::Init(HWND aWindowHandle)
{
	if (!myRHI.Init(aWindowHandle, true, myBackBuffer))
	{
		return false;
	}

	// htodo: temporary code
	HD_GrowingArray<Vertex> vertices;
	vertices.Resize(3);

	vertices[0].myPosition.Set(0.f, 0.75f, 0.f, 1.f);
	vertices[1].myPosition.Set(0.75f, -0.75f, 0.f, 1.f);
	vertices[2].myPosition.Set(-0.75f, -0.75f, 0.f, 1.f);

	vertices[0].myColor.Set(1.f, 0.f, 0.f, 1.f);
	vertices[1].myColor.Set(0.f, 1.f, 0.f, 1.f);
	vertices[2].myColor.Set(0.f, 0.f, 1.f, 1.f);

	myRHI.CreateVertexBuffer("Triangle", vertices, myTempBuffer);

	return true;
}

void GraphicsEngine::Render()
{
	myRHI.ClearRenderTarget(myBackBuffer);
	myRHI.SetRenderTarget(&myBackBuffer);

	myRHI.SetVertexBuffer(&myTempBuffer);
	myRHI.Draw(3);

	myRHI.Present();
}

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}
