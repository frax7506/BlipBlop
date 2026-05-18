#pragma once

#include "RHIStructs.h"

#include <wrl.h>

struct ID3D11RenderTargetView;

class Texture
{
	friend class RenderHardwareInterface;

public:
	Texture();
	~Texture();

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myRTV;
	Viewport myViewport;
};
