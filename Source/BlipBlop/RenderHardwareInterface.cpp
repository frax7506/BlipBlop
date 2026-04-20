#include "stdafx.h"
#include "RenderHardwareInterface.h"

#include <vector>

using namespace Microsoft::WRL;

RenderHardwareInterface::RenderHardwareInterface()
{
}

RenderHardwareInterface::~RenderHardwareInterface()
{
}

bool RenderHardwareInterface::Init(HWND aWindowHandle, bool aEnableDebug)
{
	HD_Unused(aWindowHandle);
	HD_Unused(aEnableDebug);

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

	// fortsätt här, fixa så att loggern kan printa wchar. (union i LogEntry av HD_String och HD_WString?)
	// LOG_W_MESSAGE(L"Selected adapter is %s", selectedAdapterDesc.Description);

	return true;
}
