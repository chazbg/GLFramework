#pragma once
class GLUTWrapper
{
public:
	GLUTWrapper();
	~GLUTWrapper();
	static void InitWindow(void(*renderScene)());
	static void UpdateFrame();
	static void RequestNewFrame();
	static void RenderLoop();
	static void SetKeyboardCallback();
	static void SetMouseCallback();
private:
};

