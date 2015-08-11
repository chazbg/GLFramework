#include "FrameBuffer.hpp"
#include <GL/glew.h>

FrameBuffer::FrameBuffer() : fbo(0), colorTex(0), depthTex(0)
{
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &colorTex);
	glGenTextures(1, &depthTex);
}

FrameBuffer::~FrameBuffer()
{
	//TODO: delete fbo
}

unsigned int FrameBuffer::getFbo()
{
	return fbo;
}

unsigned int FrameBuffer::getColorTex()
{
	return colorTex;
}

unsigned int FrameBuffer::getDepthTex()
{
	return depthTex;
}