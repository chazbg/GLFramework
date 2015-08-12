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
	static void SetKeyboardCallback(void(*callback)(unsigned char, int, int));
	static void SetMouseCallback(void(*callback)(int, int, int, int));
private:
};

