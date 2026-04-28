#pragma once

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
	void ClearRenderTarger(const Texture& aTarget) const;

private:
	void SetObjectName(const Microsoft::WRL::ComPtr<ID3D11DeviceChild>& aObject, const char* aName) const;

	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;
};
