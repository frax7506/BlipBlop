#pragma once

#include "HD_Vector2.h"

#include <wrl.h>

class Texture;

struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

class RenderHardwareInterface
{
public:
	RenderHardwareInterface();
	~RenderHardwareInterface();

	bool Init(HWND aWindowHandle, bool aEnableDebug, Texture& outBackBuffer);

	void Present() const;
	void ClearRenderTarget(const Texture& aTarget) const;

	void SetRenderTarget(const Texture* aTarget) const;

	HD_Vector2ui GetClientSize() const;

private:
	void SetObjectName(const Microsoft::WRL::ComPtr<ID3D11DeviceChild>& aObject, const char* aName) const;

	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;

	HWND myWindowHandle;
};
