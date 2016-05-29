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
    virtual void setProperty(const std::string name, const int v);
    virtual void setProperty(const std::string name, const unsigned int v);
    virtual void setProperty(const std::string name, const float v);
    virtual void setProperty(const std::string name, const Vec2& v);
    virtual void setProperty(const std::string name, const Vec3& v);
    virtual void setProperty(const std::string name, const Matrix4& v);
    
    int getId() const;
    const std::map<std::string, std::pair<int, int>>&          getIntProperties() const;
    const std::map<std::string, std::pair<int, unsigned int>>& getUintProperties() const;
    const std::map<std::string, std::pair<int, float>>&        getFloatProperties() const;
    const std::map<std::string, std::pair<int, Vec2>>&         getVec2Properties() const;
    const std::map<std::string, std::pair<int, Vec3>>&         getVec3Properties() const;
    const std::map<std::string, std::pair<int, Matrix4>>&      getMatrix4Properties() const;
private:
    OpenGLMaterial(const int id);
    OpenGLMaterial(const OpenGLMaterial& rhs);
    ~OpenGLMaterial();
    template <typename T1, typename T2>
    void setProperty(T1& map, const std::string name, const T2& value);
    int id;
    vector<const ITexture*> textures;
    vector<const ITextureCubemap*> textureCubemaps;
    std::map<std::string, std::pair<int, int>>          iUniforms;
    std::map<std::string, std::pair<int, unsigned int>> uiUniforms;
    std::map<std::string, std::pair<int, float>>        fUniforms;
    std::map<std::string, std::pair<int, Vec2>>         v2Uniforms;
    std::map<std::string, std::pair<int, Vec3>>         vUniforms;
    std::map<std::string, std::pair<int, Matrix4>>      mUniforms;
};

template<typename T1, typename T2>
inline void OpenGLMaterial::setProperty(T1 & map, const std::string name, const T2 & value)
{
    auto it = map.find(name);
    if (it != map.end())
    {
        map[name].second = value;
    }
    //else
    //{
    //    std::cout << "No property named " << name << " for Material[" << id << "]" << std::endl;
    //}
}
