#pragma once

#include "Core/IMaterial.hpp"
#include "Rendering/Variants/OpenGL/OpenGLMaterialProperty.hpp"
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
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<int>>& materialProperty);
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<unsigned int>>& materialProperty);
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<float>>& materialProperty);
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<Vec2>>& materialProperty);
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<Vec3>>& materialProperty);
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<Matrix4>>& materialProperty);
    virtual void setProperty(std::shared_ptr<IMaterialProperty<int>>, const int v);
    virtual void setProperty(std::shared_ptr<IMaterialProperty<unsigned int>>, const unsigned int v);
    virtual void setProperty(std::shared_ptr<IMaterialProperty<float>>, const float v);
    virtual void setProperty(std::shared_ptr<IMaterialProperty<Vec2>>, const Vec2& v);
    virtual void setProperty(std::shared_ptr<IMaterialProperty<Vec3>>, const Vec3& v);
    virtual void setProperty(std::shared_ptr<IMaterialProperty<Matrix4>>, const Matrix4& v);

    int getId() const;
    const std::vector<std::shared_ptr<OpenGLMaterialProperty<int>>>&           getIntProperties() const;
    const std::vector<std::shared_ptr<OpenGLMaterialProperty<unsigned int>>>&  getUintProperties() const;
    const std::vector<std::shared_ptr<OpenGLMaterialProperty<float>>>&         getFloatProperties() const;
    const std::vector<std::shared_ptr<OpenGLMaterialProperty<Vec2>>>&          getVec2Properties() const;
    const std::vector<std::shared_ptr<OpenGLMaterialProperty<Vec3>>>&          getVec3Properties() const;
    const std::vector<std::shared_ptr<OpenGLMaterialProperty<Matrix4>>>&       getMatrix4Properties() const;
private:
    OpenGLMaterial(const int id);
    OpenGLMaterial(const OpenGLMaterial& rhs);
    ~OpenGLMaterial();
    template <typename T1, typename T2>
    void setProperty(T1& map, const std::string name, const T2& value);
    int id;
    vector<const ITexture*> textures;
    vector<const ITextureCubemap*> textureCubemaps;

    std::vector<std::shared_ptr<OpenGLMaterialProperty<int>>>          iUniforms;
    std::vector<std::shared_ptr<OpenGLMaterialProperty<unsigned int>>> uiUniforms;
    std::vector<std::shared_ptr<OpenGLMaterialProperty<float>>>        fUniforms;
    std::vector<std::shared_ptr<OpenGLMaterialProperty<Vec2>>>         v2Uniforms;
    std::vector<std::shared_ptr<OpenGLMaterialProperty<Vec3>>>         v3Uniforms;
    std::vector<std::shared_ptr<OpenGLMaterialProperty<Matrix4>>>      m4Uniforms;

    template <typename T1, typename T2>
    void getProperty(T1 & uniforms, const std::string name, std::shared_ptr<IMaterialProperty<T2>>& materialProperty);

    template <typename T>
    void addProperty(std::vector<std::shared_ptr<T>> & uniforms, const std::string name, const int location);

    template <typename T>
    void copyProperties(T& lhsUniforms, const T& rhsUniforms);
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

template<typename T1, typename T2>
inline void OpenGLMaterial::getProperty(T1 & uniforms, const std::string name, std::shared_ptr<IMaterialProperty<T2>>& materialProperty)
{
    for (unsigned int i = 0; i < uniforms.size(); ++i)
    {
        if (name == uniforms[i]->name)
        {
            materialProperty = uniforms[i];
            break;
        }
    }
}

template<typename T>
inline void OpenGLMaterial::addProperty(std::vector<std::shared_ptr<T>> & uniforms, const std::string name, const int location)
{
    std::shared_ptr<T> p(new T());
    p->name = name;
    p->location = location;
    uniforms.push_back(p);
}

template<typename T>
inline void OpenGLMaterial::copyProperties(T & lhsUniforms, const T & rhsUniforms)
{
    for (unsigned int i = 0; i < rhsUniforms.size(); ++i)
    {
        addProperty(lhsUniforms, 
            rhsUniforms[i]->name, 
            rhsUniforms[i]->location);
        lhsUniforms[i]->value = rhsUniforms[i]->value;
    }
}
