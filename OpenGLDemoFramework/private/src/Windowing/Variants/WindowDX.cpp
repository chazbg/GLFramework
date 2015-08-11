#include "Windowing/Window.hpp"

Window::Window(const WindowParameters& params, IApplication& app) : params(params), app(app)
{
	
}

Window::~Window()
{

}

void Window::startRenderLoop()
{
	HINSTANCE hInstance = 0;
	// Initialize the window class.
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = "WindowClass1";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(params.width), static_cast<LONG>(params.height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Window handle.
	HWND m_hwnd;

	// Create the window and store a handle to it.
	m_hwnd = CreateWindowEx(NULL,
		"WindowClass1",
		params.name.c_str(),
		WS_OVERLAPPEDWINDOW,
		params.posX,
		params.posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,		// We have no parent window, NULL.
		NULL,		// We aren't using menus, NULL.
		hInstance,
		NULL);		// We aren't using multiple windows, NULL.

	ShowWindow(m_hwnd, SW_SHOWNORMAL);

	// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
	app.onInit();

	// Main sample loop.
	MSG msg = { 0 };
	while (true)
	{
		// Process any messages in the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;

			// Pass events into our sample.
			app.onEvent(0);
		}

		app.onUpdate(0);
		app.onRender(0);
	}

	app.onDestroy();

	// Return this part of the WM_QUIT message to Windows.
	//return static_cast<char>(msg.wParam);
}

// Main message handler for the sample.
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Handle destroy/shutdown messages.
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}