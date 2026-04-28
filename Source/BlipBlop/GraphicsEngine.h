#pragma once

#include "RenderHardwareInterface.h"
#include "Texture.h"

class GraphicsEngine
{
public:
	static GraphicsEngine& Get();

	bool Initialize(HWND aWindowHandle);
	void Render();

private:
	GraphicsEngine();
	~GraphicsEngine();

	RenderHardwareInterface myRHI;
	Texture myBackBuffer;
};
