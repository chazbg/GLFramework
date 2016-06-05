#pragma once
#include "Core/IVertexBuffer.hpp"
#include "Core/IIndexBuffer.hpp"
#include "Core/IMaterial.hpp"
#include "Core/ITexture.hpp"
#include "Core/ITextureCubemap.hpp"
#include "Core/IRenderTarget.hpp"
#include <string>

class IResourceManager
{
public:
    virtual ~IResourceManager(){}

    virtual ITexture* createTexture(const std::string path) = 0;
    virtual ITexture* createTexture(
        const unsigned int width, 
        const unsigned int height, 
        const unsigned int bpp, 
        const bool isDepthComponent) = 0;
    virtual ITexture* createTexture(
        const unsigned int width, 
        const unsigned int height, 
        const unsigned int bpp, 
        const unsigned char* data) = 0;
    virtual void destroyTexture(ITexture* tex) = 0;
    virtual ITextureCubemap* createTextureCubemap(
        const std::string pathFront,
        const std::string pathBack,
        const std::string pathTop,
        const std::string pathBottom,
        const std::string pathLeft,
        const std::string pathRight) = 0;
    virtual void destroyTextureCubemap(ITextureCubemap* tex) = 0;
    virtual IMaterial* createMaterial(const std::string vShaderPath, const std::string fShaderPath) = 0;
    virtual IMaterial* createMaterial(const std::string vShaderPath, const std::string fShaderPath, const std::string gShaderPath) = 0;
    virtual IMaterial* cloneMaterial(const IMaterial* material) = 0;
    virtual void destroyMaterial(IMaterial* material) = 0;
    virtual IVertexBuffer* createVertexBuffer(
        const unsigned int vertexCount, 
        const unsigned int attributeSize, 
        const unsigned int location, 
        const float* data) = 0;
    virtual void destroyVertexBuffer(IVertexBuffer* vb) = 0;
    virtual IIndexBuffer* createIndexBuffer(const unsigned int indexCount, const unsigned int* data) = 0;
    virtual void destroyIndexBuffer(IIndexBuffer* ib) = 0;
    virtual IRenderTarget* createRenderTarget(const unsigned int width, const unsigned height) = 0;
    virtual void destroyRenderTarget(IRenderTarget* rt) = 0;
};

