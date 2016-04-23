#pragma once

#include "Core/IMaterial.hpp"
#include <string>

class OpenGLMaterial : public IMaterial
{
    friend class OpenGLResourceManager;
public:
    virtual const std::vector<const ITexture*>& getTextures() const;
    virtual const std::vector<const ITextureCubemap*>& getTextureCubemaps() const;
    virtual void addTexture(const ITexture* tex);
    virtual void addTextureCubemap(const ITextureCubemap* tex);
    virtual void removeTexture(const ITexture* tex);
    virtual void removeTextureCubemap(const ITextureCubemap* tex);
    virtual const std::map<std::string, int>&          getIntProperties() const;
    virtual const std::map<std::string, unsigned int>& getUintProperties() const;
    virtual const std::map<std::string, float>&        getFloatProperties() const;
    virtual const std::map<std::string, Vec3>&         getVec3Properties() const;
    virtual const std::map<std::string, Matrix4>&      getMatrix4Properties() const;
    virtual void setProperty(const std::string name, const int v);
    virtual void setProperty(const std::string name, const unsigned int v);
    virtual void setProperty(const std::string name, const float v);
    virtual void setProperty(const std::string name, const Vec3& v);
    virtual void setProperty(const std::string name, const Matrix4& v);
    
    int getId() const;
private:
    OpenGLMaterial(const int id);
    OpenGLMaterial(const OpenGLMaterial& rhs);
    ~OpenGLMaterial();
    int id;
    vector<const ITexture*> textures;
    vector<const ITextureCubemap*> textureCubemaps;
    map<std::string, int> iUniforms;
    map<std::string, unsigned int> uiUniforms;
    map<std::string, float> fUniforms;
    map<std::string, Vec3> vUniforms;
    map<std::string, Matrix4> mUniforms;
};