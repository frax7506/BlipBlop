#pragma once

#include "HD_Vector2.h"

#include <wrl.h>

class Buffer;
class Texture;

struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct Vertex;

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

	bool CreateVertexBuffer(const char* aName, const HD_GrowingArray<Vertex>& aVertexList, Buffer& outBuffer) const;
	void SetVertexBuffer(const Buffer* aBuffer) const;

	void Draw(u32 aNumVertices) const;

private:
	void SetObjectName(const Microsoft::WRL::ComPtr<ID3D11DeviceChild>& aObject, const char* aName) const;

	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;

	HWND myWindowHandle;

	// htodo: temporary members
	Microsoft::WRL::ComPtr<struct ID3D11InputLayout> myTempInputLayout;
	Microsoft::WRL::ComPtr<struct ID3D11VertexShader> myTempVertexShader;
	Microsoft::WRL::ComPtr<struct ID3D11PixelShader> myTempPixelShader;
};
