#pragma once

#include <Core/IMaterial.hpp>
#include <string>
#include <Core/Texture.hpp>
#include <Core/TextureCubemap.hpp>

class ShaderMaterial : public IMaterial
{
public:
    ShaderMaterial(const std::string vShaderPath, const std::string fShaderPath);
    ~ShaderMaterial();
    virtual int getId() const;
    virtual const std::vector<const Texture*>& getTextures() const;
    virtual const std::vector<const TextureCubemap*>& getTextureCubemaps() const;
    virtual void addTexture(const Texture* tex);
    virtual void addTextureCubemap(const TextureCubemap* tex);
    virtual void removeTexture(const Texture* tex);
    virtual void removeTextureCubemap(const TextureCubemap* tex);
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
private:
    int id;
    vector<const Texture*> textures;
    vector<const TextureCubemap*> textureCubemaps;
    map<std::string, int> iUniforms;
    map<std::string, unsigned int> uiUniforms;
    map<std::string, float> fUniforms;
    map<std::string, Vec3> vUniforms;
    map<std::string, Matrix4> mUniforms;
};