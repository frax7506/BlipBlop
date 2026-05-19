#pragma once

#include "RenderHardwareInterface.h"
#include "Texture.h"

// htodo: temporary include
#include "Buffer.h"

class GraphicsEngine
{
public:
	static GraphicsEngine& Get();

	bool Init(HWND aWindowHandle);
	void Render();

private:
	GraphicsEngine();
	~GraphicsEngine();

	RenderHardwareInterface myRHI;
	Texture myBackBuffer;

	// htodo: temporary code
	Buffer myTempBuffer;
};
