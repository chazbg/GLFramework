#pragma once

#include "Core/IResourceManager.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTextureCubemap.hpp"
#include <FreeImage.h>
#include <GL/glew.h>
#include <map>

class OpenGLResourceManager : public IResourceManager
{
public:
    OpenGLResourceManager();
    virtual ~OpenGLResourceManager();
    virtual ITexture* createTexture(const std::string path);
    virtual ITexture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data);
    virtual ITexture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const bool isDepthComponent);
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
    virtual IMaterial* cloneMaterial(const IMaterial* material);
    virtual void destroyMaterial(IMaterial* material);
private:
    FIBITMAP* loadImage(const std::string path); //TODO: Implement class Image (open, close, getters, etc.)
    void unloadImage(FIBITMAP* dib); //TODO: Implement class Image (open, close, getters, etc.)

    std::vector<ITexture*> textures;
    std::vector<ITextureCubemap*> textureCubemaps;
    std::vector<FIBITMAP*> bitmaps; //TODO: Implement class Image (open, close, getters, etc.)
    std::vector<IMaterial*> materials;
    std::map<int, int> materialRefCounters;
};