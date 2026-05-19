#include "stdafx.h"
#include "RenderHardwareInterface.h"

#include "Buffer.h"
#include "RHIStructs.h"
#include "Texture.h"
#include "Vertex.h"

#include <vector>

// htodo: temporary includes
#include "TemporaryShaders/VertexShader.h"
#include "TemporaryShaders/PixelShader.h"

using namespace Microsoft::WRL;

RenderHardwareInterface::RenderHardwareInterface()
{
}

RenderHardwareInterface::~RenderHardwareInterface()
{
}

bool RenderHardwareInterface::Init(HWND aWindowHandle, bool aEnableDebug, Texture& outBackBuffer)
{
	myWindowHandle = aWindowHandle;
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

	SetObjectName(outBackBuffer.myRTV, "BackBuffer_RTV");

	HD_Vector2ui clientSize = GetClientSize();
	Viewport viewport = { 0, 0, static_cast<f32>(clientSize.myX), static_cast<f32>(clientSize.myY), 0, 1 };
	outBackBuffer.myViewport = viewport;

	// htodo: temporary code

	myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(Topology::TriangleList));

	HD_GrowingArray<D3D11_INPUT_ELEMENT_DESC> elements;
	elements.Reserve(Vertex::ourDescription.GetSize());

	// fortsätt här: ourDescription for inte sätt strings
	for (const VertexElementDesc& desc : Vertex::ourDescription)
	{
		D3D11_INPUT_ELEMENT_DESC element = {};
		element.SemanticName = desc.mySemantic.GetBuffer();
		element.SemanticIndex = desc.mySemanticIndex;
		element.Format = static_cast<DXGI_FORMAT>(desc.myFormat);

		element.InputSlot = 0;
		element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		element.InstanceDataStepRate = 0;

		elements.PushBack(element);
	}

	myDevice->CreateInputLayout(
		elements.GetData(),
		static_cast<u32>(elements.GetSize()),
		TEMP_VertexShader_ByteCode,
		sizeof(TEMP_VertexShader_ByteCode),
		&myTempInputLayout
	);

	myContext->IASetInputLayout(myTempInputLayout.Get());

	myDevice->CreateVertexShader(TEMP_VertexShader_ByteCode, sizeof(TEMP_VertexShader_ByteCode), nullptr, &myTempVertexShader);
	myContext->VSSetShader(myTempVertexShader.Get(), nullptr, 0);

	myDevice->CreatePixelShader(TEMP_PixelShader_ByteCode, sizeof(TEMP_PixelShader_ByteCode), nullptr, &myTempPixelShader);
	myContext->PSSetShader(myTempPixelShader.Get(), nullptr, 0);

	SetObjectName(myTempInputLayout, "TemporaryInputLayout");
	SetObjectName(myTempVertexShader, "TemporaryVertexShader");
	SetObjectName(myTempPixelShader, "TemporaryPixelShader");

	// end of temporary code

	LOG_MESSAGE("RHI started!");
	return true;
}

void RenderHardwareInterface::Present() const
{
	mySwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

void RenderHardwareInterface::ClearRenderTarget(const Texture& aTarget) const
{
	f32 clearColor[4] = { 0, 0, 0, 0 };
	myContext->ClearRenderTargetView(aTarget.myRTV.Get(), clearColor);
}

void RenderHardwareInterface::SetRenderTarget(const Texture* aTarget) const
{
	ID3D11RenderTargetView* renderTargetView = nullptr;
	D3D11_VIEWPORT viewport = { 0, 0, 0, 0, 0, 1 };

	if (aTarget)
	{
		renderTargetView = aTarget->myRTV.Get();
		memcpy_s(&viewport, sizeof(D3D11_VIEWPORT), &aTarget->myViewport, sizeof(Viewport));
	}

	myContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	myContext->RSSetViewports(1, &viewport);
}

HD_Vector2ui RenderHardwareInterface::GetClientSize() const
{
	RECT clientRect = {};
	GetClientRect(myWindowHandle, &clientRect);
	const u32 width = clientRect.right - clientRect.left;
	const u32 height = clientRect.bottom - clientRect.top;

	return { width, height };
}

bool RenderHardwareInterface::CreateVertexBuffer(const char* aName, const HD_GrowingArray<Vertex>& aVertexList, Buffer& outBuffer) const
{
	if (aVertexList.GetIsEmpty())
	{
		LOG_ERROR_F("Failed to create vertex buffer for %s. Vertex list is empty.", aName);
		return false;
	}

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.ByteWidth = static_cast<u32>(sizeof(Vertex) * aVertexList.GetSize());

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = aVertexList.GetData();

	const HRESULT result = myDevice->CreateBuffer(&desc, &data, &outBuffer.myBuffer);
	if (FAILED(result))
	{
		LOG_ERROR_F("Failed to create vertex buffer for %s. Failed to create Buffer.", aName);
		return false;
	}

	const HD_String bufferName = HD_Format("%s_VX", aName).GetBuffer();
	SetObjectName(outBuffer.myBuffer, bufferName.GetBuffer());

	outBuffer.myName = aName;
	outBuffer.mySize = desc.ByteWidth;
	outBuffer.myStride = sizeof(Vertex);
	outBuffer.myType = BufferType::VertexBuffer;

	return true;
}

void RenderHardwareInterface::SetVertexBuffer(const Buffer* aBuffer) const
{
	constexpr u32 offset = 0;

	if (aBuffer)
	{
		myContext->IASetVertexBuffers(0, 1, aBuffer->myBuffer.GetAddressOf(), &aBuffer->myStride, &offset);
	}
	else
	{
		ID3D11Buffer* buffer = nullptr;
		constexpr u32 stride = 0;
		myContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	}
}

void RenderHardwareInterface::Draw(u32 aNumVertices) const
{
	myContext->Draw(aNumVertices, 0);
}

void RenderHardwareInterface::SetObjectName(const Microsoft::WRL::ComPtr<ID3D11DeviceChild>& aObject, const char* aName) const
{
	if (aObject)
	{
		aObject->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<u32>(HD_Strlen(aName)), aName);
	}
}
