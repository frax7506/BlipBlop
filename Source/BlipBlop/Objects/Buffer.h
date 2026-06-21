#pragma once

#include "RHIStructs.h"

#include "HD_String.h"

#include <wrl.h>

struct ID3D11Buffer;

class Buffer
{
	friend class RenderHardwareInterface;

public:
	Buffer();
	~Buffer();

private:
	HD_String myName;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	size_t mySize;
	u32 myStride;
	BufferType myType;
};
