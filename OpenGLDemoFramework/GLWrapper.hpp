#pragma once
class GLWrapper
{
public:
	GLWrapper();
	~GLWrapper();
	static void InitRenderer();
	static void ClearWindow();
	static void RenderTexture(unsigned char* buffer, unsigned int width, unsigned int height);
};

