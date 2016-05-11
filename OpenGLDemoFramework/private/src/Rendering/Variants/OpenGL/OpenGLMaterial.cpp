#include "Rendering/Variants/OpenGL/OpenGLMaterial.hpp"

OpenGLMaterial::OpenGLMaterial(const int id) :
    id(id)
{
}

OpenGLMaterial::OpenGLMaterial(const OpenGLMaterial& rhs) :
    id(rhs.id),
    textures(rhs.textures),
    textureCubemaps(rhs.textureCubemaps),
    iUniforms(rhs.iUniforms),
    uiUniforms(rhs.uiUniforms),
    fUniforms(rhs.fUniforms),
    vUniforms(rhs.vUniforms),
    mUniforms(rhs.mUniforms)
{

}

OpenGLMaterial::~OpenGLMaterial()
{
}

int OpenGLMaterial::getId() const
{
    return id;
}

const std::vector<const ITexture*>& OpenGLMaterial::getTextures() const
{
    return textures;
}

const std::vector<const ITextureCubemap*>& OpenGLMaterial::getTextureCubemaps() const
{
    return textureCubemaps;
}

void OpenGLMaterial::addTexture(const ITexture* tex)
{
    textures.push_back(tex);
}

void OpenGLMaterial::addTextureCubemap(const ITextureCubemap* tex)
{
    textureCubemaps.push_back(tex);
}

void OpenGLMaterial::removeTexture(const ITexture* tex)
{
    vector<const ITexture*>::iterator it = find(textures.begin(), textures.end(), tex);
    if (textures.end() != it)
    {
        textures.erase(it);
    }
}

void OpenGLMaterial::removeTextureCubemap(const ITextureCubemap* tex)
{
    vector<const ITextureCubemap*>::iterator it = find(textureCubemaps.begin(), textureCubemaps.end(), tex);
    if (textureCubemaps.end() != it)
    {
        textureCubemaps.erase(it);
    }
}

const map<string, int>& OpenGLMaterial::getIntProperties() const
{
    return iUniforms;
}

const map<string, unsigned int>& OpenGLMaterial::getUintProperties() const
{
    return uiUniforms;
}

const map<string, float>& OpenGLMaterial::getFloatProperties() const
{
    return fUniforms;
}

const map<string, Vec3>& OpenGLMaterial::getVec3Properties() const
{
    return vUniforms;
}

const map<string, Matrix4>& OpenGLMaterial::getMatrix4Properties() const
{
    return mUniforms;
}

void OpenGLMaterial::setProperty(const string name, const int v)
{
    iUniforms[name] = v;
}

void OpenGLMaterial::setProperty(const string name, const unsigned int v)
{
    uiUniforms[name] = v;
}

void OpenGLMaterial::setProperty(const string name, const float v)
{
    fUniforms[name] = v;
}

void OpenGLMaterial::setProperty(const string name, const Vec3 & v)
{
    vUniforms[name] = v;
}

void OpenGLMaterial::setProperty(const string name, const Matrix4 & v)
{
    mUniforms[name] = v;
}
