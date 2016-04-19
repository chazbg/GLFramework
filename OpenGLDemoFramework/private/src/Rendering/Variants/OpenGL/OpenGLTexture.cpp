#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include <GL/glew.h>

OpenGLTexture::OpenGLTexture(const unsigned int width, const unsigned int height, const unsigned int bpp) :
width(width),
height(height),
bpp(bpp),
id(0)
{
}

OpenGLTexture::~OpenGLTexture()
{

}

unsigned int OpenGLTexture::getId() const
{
	return id;
}

unsigned int OpenGLTexture::getFormat(const unsigned int bpp)
{
    unsigned int format = 0;
    switch (bpp)
    {
    case 1:
    {
        format = GL_LUMINANCE;
        break;
    }
    case 3:
    {
        format = GL_RGB;
        break;
    }
    case 4:
    {
        format = GL_RGBA;
        break;
    }
    default:
        break;
    }

    return format;
}

unsigned int OpenGLTexture::getWidth() const
{
	return width;
}

unsigned int OpenGLTexture::getHeight() const
{
	return height;
}

unsigned int OpenGLTexture::getBpp() const
{
	return bpp;
}