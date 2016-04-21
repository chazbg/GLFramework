#pragma once

#include "Core/IResourceManager.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTextureCubemap.hpp"
#include <FreeImage.h>
#include <GL/glew.h>

class OpenGLResourceManager : public IResourceManager
{
public:
    OpenGLResourceManager()
    {
#ifdef FREEIMAGE_LIB
        FreeImage_Initialise();
#endif
    }

    virtual ~OpenGLResourceManager()
    {
#ifdef FREEIMAGE_LIB
        FreeImage_DeInitialise();
#endif
    }

    virtual ITexture* createTexture(const std::string path)
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

    virtual ITexture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data)
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

    virtual ITexture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const bool isDepthComponent)
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

    virtual void destroyTexture(ITexture* tex)
    {
        std::vector<ITexture*>::iterator it = std::find(textures.begin(), textures.end(), tex);
        if (it != textures.end())
        {
            textures.erase(it);
            OpenGLTexture* openglTex = reinterpret_cast<OpenGLTexture*>(tex);
            unsigned int id = openglTex->getId();
            glDeleteTextures(1, &id);
            delete openglTex;
        }
    }

    virtual ITextureCubemap* createTextureCubemap(
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

    virtual void destroyTextureCubemap(ITextureCubemap* tex)
    {
        std::vector<ITextureCubemap*>::iterator it = std::find(textureCubemaps.begin(), textureCubemaps.end(), tex);
        if (it != textureCubemaps.end())
        {
            textureCubemaps.erase(it);
            OpenGLTextureCubemap* openglTex = reinterpret_cast<OpenGLTextureCubemap*>(tex);
            unsigned int id = openglTex->getId();
            glDeleteTextures(1, &id);
            delete openglTex;
        }
    }

private:
    FIBITMAP* loadImage(const std::string path)
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

    void unloadImage(FIBITMAP* dib)
    {
        FreeImage_Unload(dib);
    }

    std::vector<ITexture*> textures;
    std::vector<ITextureCubemap*> textureCubemaps;
    std::vector<FIBITMAP*> bitmaps;
};