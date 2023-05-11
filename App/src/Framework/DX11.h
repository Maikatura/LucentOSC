#pragma once
#include <array>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct D3D11_TEXTURE2D_DESC;

class DX11 
{

public:
	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;

	static ID3D11RenderTargetView* BackBuffer;
	static ComPtr<ID3D11ShaderResourceView> BackBufferSRV;
	static ID3D11Texture2D* BackBufferTex;
	static ComPtr<ID3D11DepthStencilView> DepthBuffer;

	DX11();
	~DX11();

	static bool Init(HWND aWindowHandle, bool aEnableDeviceDebug);
	static void BeginFrame(std::array<float, 4> aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
	static void EndFrame();

	static RECT GetClientSize();

	static void Resize();

	static bool ResizeViewport();

private:

	
	static bool CreateSwapChain(bool aEnableDeviceDebug);
	static bool CreateTexture2D();
	static bool CreateDepthBuffer();
	static bool CreateShaderResourceView();
	static bool CreateSampler();
	static bool CreateSelectionView();

	inline static HWND WindowHandle;
};


