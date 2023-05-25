
#include "DX11.h"

#include <d3d11.h>
#include <iostream>

ComPtr<ID3D11Device> DX11::Device;
ComPtr<ID3D11DeviceContext> DX11::Context;
ComPtr<IDXGISwapChain> DX11::SwapChain;

ID3D11RenderTargetView* DX11::BackBuffer;
ComPtr<ID3D11ShaderResourceView> DX11::BackBufferSRV;
ID3D11Texture2D* DX11::BackBufferTex;
ComPtr<ID3D11DepthStencilView> DX11::DepthBuffer;


DX11::DX11()
{}

DX11::~DX11()
{}

bool DX11::Init(HWND aWindowHandle, bool aEnableDeviceDebug)
{
	bool result = true;

	WindowHandle = aWindowHandle;

	result = CreateSwapChain(aEnableDeviceDebug);
	if(!result)
	{
		return false;
	}

	result = CreateTexture2D();
	if(!result)
	{
		return false;
	}

	result = CreateDepthBuffer();
	if(!result)
	{
		return false;
	}

	result = ResizeViewport();
	if(!result)
	{
		return false;
	}

	return true;
}

void DX11::BeginFrame(std::array<float, 4> aClearColor)
{
	Context->OMSetRenderTargets(1, &BackBuffer, DepthBuffer.Get());
	Context->ClearRenderTargetView(BackBuffer, &aClearColor[0]);
}

void DX11::EndFrame()
{
	SwapChain->Present(1, 0);
}

RECT DX11::GetClientSize()
{
	RECT clientRect;
	GetClientRect(WindowHandle, &clientRect);
	return clientRect;
}


bool DX11::CreateSwapChain(bool aEnableDeviceDebug)
{

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = clientRect.right - clientRect.left;
	swapChainDesc.BufferDesc.Height = clientRect.bottom - clientRect.top;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;


	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = WindowHandle;
	swapChainDesc.Windowed = true;

	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		(aEnableDeviceDebug ? D3D11_CREATE_DEVICE_DEBUG : 0),
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf()
	);
	if(FAILED(result))
		return false;

	return true;
}

bool DX11::CreateTexture2D()
{

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	HRESULT result = S_FALSE;


	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTex);
	if(FAILED(result))
		return false;


	result = Device->CreateRenderTargetView(BackBufferTex, nullptr, &BackBuffer);
	if(FAILED(result))
		return false;


	return true;
}

bool DX11::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = clientRect.right - clientRect.left;
	depthBufferDesc.Height = clientRect.bottom - clientRect.top;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	result = Device->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf());
	if(FAILED(result))
		return false;

	result = Device->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, DepthBuffer.GetAddressOf());
	if(FAILED(result))
		return false;

	return true;
}

bool DX11::ResizeViewport()
{
	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);


	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<FLOAT>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<FLOAT>(clientRect.bottom - clientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	Context->RSSetViewports(1, &viewport);

	return true;
}

bool DX11::CreateShaderResourceView()
{
	return true;
}

bool DX11::CreateSampler()
{
	
	return true;
}

bool DX11::CreateSelectionView()
{
	return true;
}

void DX11::Resize()
{
	Context->ClearState();
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	Context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	BackBuffer->Release();
	DepthBuffer->Release();
	//SamplerStateWrap->Release();
	//BackBufferTex = nullptr;


	if(BackBufferTex != nullptr)
	{
		BackBufferTex->Release();
	}

	RECT clientRect = { 0,0,0,0 };
	GetClientRect(WindowHandle, &clientRect);

	Context->Flush();

	HRESULT result;
	result = SwapChain->ResizeBuffers(1, static_cast<UINT>(clientRect.right - clientRect.left), static_cast<UINT>(clientRect.bottom - clientRect.top), DXGI_FORMAT_UNKNOWN, 0);
	if(FAILED(result))
		return;

	ComPtr<ID3D11Texture2D> spBackBuffer;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(spBackBuffer.GetAddressOf()));
	if(FAILED(result))
		return;

	result = CreateTexture2D();
	if(FAILED(result))
	{
		std::cout << "Failed Texture Creation" << "\n";
	}

	result = CreateDepthBuffer();
	if(FAILED(result))
	{
		std::cout << "Failed Depth Creation" << "\n";
	}

	result = ResizeViewport();
	if(FAILED(result))
	{
		std::cout << "Failed Viewport Creation" << "\n";
	}





}
