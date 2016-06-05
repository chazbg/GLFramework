#pragma once

#include "Core/IResourceManager.hpp"
#include "Core/IResourceManagerNotify.hpp"
#include <FreeImage.h>
#include <map>

class OpenGLResourceManager : public IResourceManager
{
public:
    OpenGLResourceManager(IResourceManagerNotify& notify);
    virtual ~OpenGLResourceManager();
    virtual ITexture* createTexture(const std::string path);
    virtual ITexture* createTexture(
        const unsigned int width, 
        const unsigned int height, 
        const unsigned int bpp, 
        const unsigned char* data);
    virtual ITexture* createTexture(
        const unsigned int width, 
        const unsigned int height, 
        const unsigned int bpp, 
        const bool isDepthComponent);
    virtual void destroyTexture(ITexture* tex);
    virtual ITextureCubemap* createTextureCubemap(
        const std::string pathFront,
        const std::string pathBack,
        const std::string pathTop,
        const std::string pathBottom,
        const std::string pathLeft,
        const std::string pathRight);
    virtual void destroyTextureCubemap(ITextureCubemap* tex);
    virtual IMaterial* createMaterial(const std::string vShaderPath, const std::string fShaderPath);
    virtual IMaterial* createMaterial(const std::string vShaderPath, const std::string fShaderPath, const std::string gShaderPath);
    virtual IMaterial* cloneMaterial(const IMaterial* material);
    virtual void destroyMaterial(IMaterial* material);
    virtual IVertexBuffer* createVertexBuffer(
        const unsigned int vertexCount, 
        const unsigned int attributeSize, 
        const unsigned int location,
        const float* data);
    virtual void destroyVertexBuffer(IVertexBuffer* vb);
    virtual IIndexBuffer* createIndexBuffer(const unsigned int indexCount, const unsigned int* data);
    virtual void destroyIndexBuffer(IIndexBuffer* ib);
    virtual IRenderTarget* createRenderTarget();
    virtual void destroyRenderTarget(IRenderTarget* rt);
private:
    FIBITMAP* loadImage(const std::string path); //TODO: Implement class Image (open, close, getters, etc.)
    void unloadImage(FIBITMAP* dib); //TODO: Implement class Image (open, close, getters, etc.)

    IResourceManagerNotify& notify;
    std::vector<ITexture*> textures;
    std::vector<ITextureCubemap*> textureCubemaps;
    std::vector<FIBITMAP*> bitmaps; //TODO: Implement class Image (open, close, getters, etc.)
    std::vector<IMaterial*> materials;
    std::map<int, int> materialRefCounters;
    std::vector<IVertexBuffer*> vertexBuffers;
    std::vector<IIndexBuffer*> indexBuffers;
    std::vector<IRenderTarget*> renderTargets;
};