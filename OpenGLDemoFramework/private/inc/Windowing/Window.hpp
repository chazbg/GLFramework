#pragma once

#include "Math/Vector.hpp"
#include <string>
//#include <windows.h>

//#include <d3d12.h>
//#include <dxgi1_4.h>
//#include <D3Dcompiler.h>
//#include <DirectXMath.h>
//#include "d3dx12.h"

#include <string>

//#include <wrl.h>

//using namespace Microsoft::WRL;

struct WindowParameters
{
	int width;
	int height;
	int posX;
	int posY;
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
	virtual void onMouseEvent(int button, int state, int x, int y) = 0;
	virtual void onKeyboardEvent(unsigned char c, int x, int y) = 0;
	virtual void onMouseMove(int x, int y) = 0;
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

#ifdef USE_DX12
#include "Windowing/Variants/WindowDX.hpp"
#else
#include "Windowing/Variants/GLUTWrapper.hpp"
#endif
};