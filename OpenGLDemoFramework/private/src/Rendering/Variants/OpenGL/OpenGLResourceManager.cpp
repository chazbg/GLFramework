#include "Rendering/Variants/OpenGL/OpenGLResourceManager.hpp"
#include "Core/Shader.hpp"
#include "Rendering/Variants/OpenGL/OpenGLMaterial.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTextureCubemap.hpp"
#include "Rendering/Variants/OpenGL/OpenGLVertexBuffer.hpp"
#include "Rendering/Variants/OpenGL/OpenGLIndexBuffer.hpp"
#include "Rendering/Variants/OpenGL/OpenGLRenderTarget.hpp"
#include <GL/glew.h>

OpenGLResourceManager::OpenGLResourceManager(IResourceManagerNotify& notify) :
    notify(notify)
{
#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }
}

OpenGLResourceManager::~OpenGLResourceManager()
{
#ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
#endif
}

ITexture* OpenGLResourceManager::createTexture(const std::string path)
{
    OpenGLTexture* tex = 0;

    FIBITMAP* dib = loadImage(path);

    //pointer to the image data
    BYTE* bits(0);
    //image width and height
    unsigned int width(0), height(0), bpp(0);

    bits = FreeImage_GetBits(dib);
    //get the image width and height
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    bpp = FreeImage_GetBPP(dib) / 8;

    //if this somehow one of these failed (they shouldn't), return failure
    if ((bits == 0) || (width == 0) || (height == 0) || (bpp == 0))
    {
        cout << "Error: bits - " << bits
            << " | width - " << width
            << " | height - " << height
            << " | bpp - " << bpp << endl;
    }
    else
    {
        unsigned int id;
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, OpenGLTexture::getFormat(bpp, false), width, height, 0, OpenGLTexture::getFormat(bpp, false), GL_UNSIGNED_BYTE, bits);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        tex = new OpenGLTexture(width, height, bpp);
        tex->id = id;

        textures.push_back(tex);
    }

    unloadImage(dib);

    return tex;
}

ITexture* OpenGLResourceManager::createTexture(
    const unsigned int width,
    const unsigned int height, 
    const unsigned int bpp, 
    const unsigned char* data)
{
    OpenGLTexture* tex = 0;
    unsigned int id;

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, OpenGLTexture::getFormat(bpp, false), width, height, 0, OpenGLTexture::getFormat(bpp, false), GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    tex = new OpenGLTexture(width, height, bpp);
    tex->id = id;

    textures.push_back(tex);
    return tex;
}

ITexture* OpenGLResourceManager::createTexture(
    const unsigned int width, 
    const unsigned int height, 
    const unsigned int bpp, 
    const bool isDepthComponent)
{
    OpenGLTexture* tex = 0;
    unsigned int id;
    unsigned int format = OpenGLTexture::getFormat(bpp, isDepthComponent);
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    tex = new OpenGLTexture(width, height, bpp);
    tex->id = id;

    textures.push_back(tex);
    return tex;
}

void OpenGLResourceManager::destroyTexture(ITexture* tex)
{
    auto it = std::find(textures.begin(), textures.end(), tex);
    if (it != textures.end())
    {
        textures.erase(it);
        OpenGLTexture* openglTex = reinterpret_cast<OpenGLTexture*>(tex);
        unsigned int id = openglTex->getId();
        glDeleteTextures(1, &id);
        delete openglTex;
    }
}

ITextureCubemap* OpenGLResourceManager::createTextureCubemap(
    const std::string pathFront,
    const std::string pathBack,
    const std::string pathTop,
    const std::string pathBottom,
    const std::string pathLeft,
    const std::string pathRight)
{
    OpenGLTextureCubemap* tex = 0;

    FIBITMAP* dib[6];
    dib[0] = loadImage(pathLeft);
    dib[1] = loadImage(pathRight);
    dib[2] = loadImage(pathTop);
    dib[3] = loadImage(pathBottom);
    dib[4] = loadImage(pathFront);
    dib[5] = loadImage(pathBack);

    unsigned int id;
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    tex = new OpenGLTextureCubemap(id);

    for (int i = 0; i < 6; i++)
    {
        unsigned char* bits = FreeImage_GetBits(dib[i]);
        unsigned int width = FreeImage_GetWidth(dib[i]);
        unsigned int height = FreeImage_GetHeight(dib[i]);
        unsigned int bpp = FreeImage_GetBPP(dib[i]) / 8;

        //if this somehow one of these failed (they shouldn't), return failure
        if ((bits == 0) || (width == 0) || (height == 0) || (bpp == 0))
        {
            cout << "Error: bits - " << bits
                << " | width - " << width
                << " | height - " << height
                << " | bpp - " << bpp << endl;
        }
        else
        {
            unsigned int id;
            unsigned int format = OpenGLTexture::getFormat(bpp, false);
            glGenTextures(1, &id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bits);

            textureCubemaps.push_back(tex);
        }

        unloadImage(dib[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return tex;
}

void OpenGLResourceManager::destroyTextureCubemap(ITextureCubemap* tex)
{
    auto it = std::find(textureCubemaps.begin(), textureCubemaps.end(), tex);
    if (it != textureCubemaps.end())
    {
        textureCubemaps.erase(it);
        OpenGLTextureCubemap* openglTex = reinterpret_cast<OpenGLTextureCubemap*>(tex);
        unsigned int id = openglTex->getId();
        glDeleteTextures(1, &id);
        delete openglTex;
    }
}

IMaterial * OpenGLResourceManager::createMaterial(const std::string vShaderPath, const std::string fShaderPath)
{
    int id = LoadShaders(vShaderPath.c_str(), fShaderPath.c_str());
    OpenGLMaterial* material = new OpenGLMaterial(id);
    
    auto it = materialRefCounters.find(id);

    if (it == materialRefCounters.end())
    {
        materialRefCounters[id] = 0;
        notify.materialCreated(*material);
    }
    else
    {
        materialRefCounters[id]++;
    }

    materials.push_back(material);

    return material;
}

IMaterial * OpenGLResourceManager::createMaterial(const std::string vShaderPath, const std::string fShaderPath, const std::string gShaderPath)
{
    int id = LoadShaders(vShaderPath.c_str(), fShaderPath.c_str(), gShaderPath.c_str());
    OpenGLMaterial* material = new OpenGLMaterial(id);

    auto it = materialRefCounters.find(id);

    if (it == materialRefCounters.end())
    {
        materialRefCounters[id] = 0;
    }
    else
    {
        materialRefCounters[id]++;
    }

    materials.push_back(material);

    return material;
}

IMaterial * OpenGLResourceManager::cloneMaterial(const IMaterial * material)
{
    const OpenGLMaterial* glMaterial = reinterpret_cast<const OpenGLMaterial*>(material);
    OpenGLMaterial* newMaterial = new OpenGLMaterial(*glMaterial);

    materialRefCounters[glMaterial->getId()]++;

    materials.push_back(newMaterial);

    return newMaterial;
}

void OpenGLResourceManager::destroyMaterial(IMaterial * material)
{
    auto it = std::find(materials.begin(), materials.end(), material);
    if (it != materials.end())
    {
        materials.erase(it);
        OpenGLMaterial* glMaterial = reinterpret_cast<OpenGLMaterial*>(material);
        unsigned int id = glMaterial->getId();

        auto refCountersIt = materialRefCounters.find(id);

        if (refCountersIt->second <= 1)
        {
            materialRefCounters.erase(refCountersIt);
            glDeleteProgram(id);
            notify.materialDestroyed(*material);
        }
        else
        {
            materialRefCounters[id]--;
        }
        
        delete glMaterial;
    }
}

IVertexBuffer * OpenGLResourceManager::createVertexBuffer(
    const unsigned int vertexCount, 
    const unsigned int attributeSize,
    const unsigned int location,
    const float * data)
{
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * attributeSize * sizeof(float), data, GL_STATIC_DRAW);
    OpenGLVertexBuffer* vb = new OpenGLVertexBuffer(id, location, vertexCount, attributeSize);
    vertexBuffers.push_back(vb);

    return vb;
}

void OpenGLResourceManager::destroyVertexBuffer(IVertexBuffer * vb)
{
    auto it = std::find(vertexBuffers.begin(), vertexBuffers.end(), vb);
    if (it != vertexBuffers.end())
    {
        vertexBuffers.erase(it);
        OpenGLVertexBuffer* glVertexBuffer = reinterpret_cast<OpenGLVertexBuffer*>(vb);
        unsigned int id = glVertexBuffer->getId();
        glDeleteBuffers(1, &id);
        delete glVertexBuffer;
    }
}

IIndexBuffer * OpenGLResourceManager::createIndexBuffer(const unsigned int indexCount, const unsigned int * data)
{
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), data, GL_STATIC_DRAW);
    OpenGLIndexBuffer* ib = new OpenGLIndexBuffer(id, indexCount);
    indexBuffers.push_back(ib);
    return ib;
}

void OpenGLResourceManager::destroyIndexBuffer(IIndexBuffer * ib)
{
    auto it = std::find(indexBuffers.begin(), indexBuffers.end(), ib);
    if (it != indexBuffers.end())
    {
        indexBuffers.erase(it);
        OpenGLIndexBuffer* glIndexBuffer = reinterpret_cast<OpenGLIndexBuffer*>(ib);
        unsigned int id = glIndexBuffer->getId();
        glDeleteBuffers(1, &id);
        delete glIndexBuffer;
    }
}

IRenderTarget * OpenGLResourceManager::createRenderTarget()
{
    unsigned int id;
    glGenFramebuffers(1, &id);
    IRenderTarget* rt = new OpenGLRenderTarget(id);
    renderTargets.push_back(rt);
    return rt;
}

void OpenGLResourceManager::destroyRenderTarget(IRenderTarget * rt)
{
    auto it = std::find(renderTargets.begin(), renderTargets.end(), rt);
    if (it != renderTargets.end())
    {
        renderTargets.erase(it);
        OpenGLRenderTarget* glRenderTarget = reinterpret_cast<OpenGLRenderTarget*>(rt);
        unsigned int id = glRenderTarget->getId();
        glDeleteFramebuffers(1, &id);
        delete glRenderTarget;
    }
}

FIBITMAP* OpenGLResourceManager::loadImage(const std::string path)
{
    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image, once loaded
    FIBITMAP *dib(0);

    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(path.c_str(), 0);

    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(path.c_str());

    //if still unkown, return failure
    if (fif == FIF_UNKNOWN)
    {
        cout << "Error: Unknown file format" << endl;
        return dib;
    }

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, path.c_str());

    //if the image failed to load, return failure
    if (!dib)
    {
        cout << "Error: Failed to load image - " << path << endl;
    }

    //retrieve the image data
    FreeImage_FlipVertical(dib);

    return dib;
}

void OpenGLResourceManager::unloadImage(FIBITMAP* dib)
{
    FreeImage_Unload(dib);
}