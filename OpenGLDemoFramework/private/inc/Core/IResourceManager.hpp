#pragma once
#include "Core/IVertexBuffer.hpp"
#include "Core/IIndexBuffer.hpp"
#include "Core/IMaterial.hpp"
#include "Core/ITexture.hpp"
#include "Core/ITextureCubemap.hpp"
#include <string>

class IResourceManager
{
public:
	virtual ~IResourceManager(){}

    virtual ITexture* createTexture(const std::string path) = 0;
    virtual ITexture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp) = 0;
    virtual ITexture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data) = 0;
    virtual void destroyTexture(ITexture* tex) = 0;
    virtual ITextureCubemap* createTextureCubemap(
        const std::string pathFront,
        const std::string pathBack,
        const std::string pathTop,
        const std::string pathBottom,
        const std::string pathLeft,
        const std::string pathRight) = 0;
    virtual void destroyTextureCubemap(ITextureCubemap* tex) = 0;
};

