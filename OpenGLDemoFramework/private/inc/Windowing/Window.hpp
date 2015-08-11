#pragma once

#include "Math/Vector.hpp"
#include <string>
#include <windows.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

#include <string>

#include <wrl.h>

using namespace Microsoft::WRL;

struct WindowParameters
{
	int width;
	int height;
	int posX;
	int posY;
	Vec4 clearColor;
	std::string name;
};

class IApplication 
{
public:
	virtual ~IApplication() {}
	virtual void onInit() = 0;
	virtual void onUpdate(const unsigned int deltaTime) = 0;
	virtual void onRender(const unsigned int deltaTime) = 0;
	virtual void onDestroy() = 0;
	virtual void onEvent(const unsigned int event) = 0;
};

class Window
{
public:
	Window(const WindowParameters& params, IApplication& app);
	~Window();
	void startRenderLoop();
private:
	const WindowParameters& params;
	IApplication& app;

	static const UINT FrameCount = 2;

	// Pipeline objects.
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	UINT m_rtvDescriptorSize;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;

	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//#ifdef USE_DX12
//#include "Windowing/Variants/WindowDX.hpp"
//#else
//#include "Windowing/Variants/GLUTWrapper.hpp"
//#endif
};