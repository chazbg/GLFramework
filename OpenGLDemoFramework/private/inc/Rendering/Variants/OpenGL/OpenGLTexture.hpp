#pragma once

#include "Core/ITexture.hpp"
#include <string>

class OpenGLTexture : ITexture
{
    friend class OpenGLResourceManager;
public:
    virtual unsigned int getWidth() const;
    virtual unsigned int getHeight() const;
    virtual unsigned int getBpp() const;
    unsigned int getId() const;
    static unsigned int getFormat(const unsigned int bpp);
private:
    OpenGLTexture(const unsigned int width, const unsigned int height, const unsigned int bpp);
    ~OpenGLTexture();

    unsigned int width;
    unsigned int height;
    unsigned int bpp;
	unsigned int id;
};