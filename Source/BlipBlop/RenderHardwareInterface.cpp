#include "stdafx.h"
#include "RenderHardwareInterface.h"

#include "Texture.h"

#include <vector>

using namespace Microsoft::WRL;

RenderHardwareInterface::RenderHardwareInterface()
{
}

RenderHardwareInterface::~RenderHardwareInterface()
{
}

bool RenderHardwareInterface::Init(HWND aWindowHandle, bool aEnableDebug, Texture& outBackBuffer)
{
	HRESULT result = E_FAIL;

	ComPtr<IDXGIFactory> dxFactory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), &dxFactory);
	if (FAILED(result))
	{
		LOG_ERROR("Failed to create DXGI Factory!");
		return false;
	}

	LOG_MESSAGE("Initializing RHI...");

	ComPtr<IDXGIAdapter> tempAdapter;
	std::vector<ComPtr<IDXGIAdapter>> adapters;
	while (dxFactory->EnumAdapters(static_cast<u32>(adapters.size()), &tempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.emplace_back(tempAdapter);
	}

	ComPtr<IDXGIAdapter> selectedAdapter;
	DXGI_ADAPTER_DESC selectedAdapterDesc = {};
	for (const ComPtr<IDXGIAdapter>& adapter : adapters)
	{
		DXGI_ADAPTER_DESC desc = {};
		adapter->GetDesc(&desc);
		if (selectedAdapterDesc.DedicatedVideoMemory < desc.DedicatedVideoMemory)
		{
			selectedAdapter = adapter;
			selectedAdapterDesc = desc;
		}
	}

	LOG_MESSAGE_F(L"Selected adapter is %s", selectedAdapterDesc.Description);

	result = D3D11CreateDevice(
		selectedAdapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		aEnableDebug ? D3D11_CREATE_DEVICE_DEBUG : 0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&myDevice,
		NULL,
		&myContext
	);

	if (FAILED(result))
	{
		LOG_ERROR("Failed to create Device!");
		return false;
	}

	const char* adapterName = "Adapter";
	myDevice->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<u32>(HD_Strlen(adapterName)), adapterName);

	const char* contextName = "Context";
	myContext->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<u32>(HD_Strlen(contextName)), contextName);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.OutputWindow = aWindowHandle;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;
	
	result = dxFactory->CreateSwapChain(myDevice.Get(), &swapChainDesc, &mySwapChain);

	if (FAILED(result))
	{
		LOG_ERROR("Failed to create Swap Chain!");
	}

	const char* swapChainName = "SwapChain";
	mySwapChain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<u32>(HD_Strlen(swapChainName)), swapChainName);

	ComPtr<ID3D11Texture2D> backBufferTexture;
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBufferTexture);

	if (FAILED(result))
	{
		LOG_ERROR("Failed to fetch Back Buffer!");
		return false;
	}

	SetObjectName(backBufferTexture, "BackBuffer_T2D");

	result = myDevice->CreateRenderTargetView(backBufferTexture.Get(), nullptr, &outBackBuffer.myRTV);

	if (FAILED(result))
	{
		LOG_ERROR("Failed to create Render Target View!");
		return false;
	}

	SetObjectName(outBackBuffer.myRTV, "BackBuvver_RTV");

	LOG_MESSAGE("RHI started!");
	return true;
}

void RenderHardwareInterface::Present() const
{
	mySwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

void RenderHardwareInterface::ClearRenderTarger(const Texture& aTarget) const
{
	f32 clearColor[4] = { 0, 1, 0, 0 };
	myContext->ClearRenderTargetView(aTarget.myRTV.Get(), clearColor);
}

void RenderHardwareInterface::SetObjectName(const Microsoft::WRL::ComPtr<ID3D11DeviceChild>& aObject, const char* aName) const
{
	if (aObject)
	{
		aObject->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<u32>(HD_Strlen(aName)), aName);
	}
}
