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
    virtual void getProperty(const std::string name, IntPropertySharedPtr& materialProperty);
    virtual void getProperty(const std::string name, UintPropertySharedPtr& materialProperty);
    virtual void getProperty(const std::string name, FloatPropertySharedPtr& materialProperty);
    virtual void getProperty(const std::string name, Vec2PropertySharedPtr& materialProperty);
    virtual void getProperty(const std::string name, Vec3PropertySharedPtr& materialProperty);
    virtual void getProperty(const std::string name, Mat4PropertySharedPtr& materialProperty);
    virtual void setProperty(IntPropertySharedPtr, const int v);
    virtual void setProperty(UintPropertySharedPtr, const unsigned int v);
    virtual void setProperty(FloatPropertySharedPtr, const float v);
    virtual void setProperty(Vec2PropertySharedPtr, const Vec2& v);
    virtual void setProperty(Vec3PropertySharedPtr, const Vec3& v);
    virtual void setProperty(Mat4PropertySharedPtr, const Matrix4& v);

    int getId() const;
    const std::vector<OpenGLIntPropertySharedPtr>&    getIntProperties() const;
    const std::vector<OpenGLUintPropertySharedPtr>&   getUintProperties() const;
    const std::vector<OpenGLFloatPropertySharedPtr>&  getFloatProperties() const;
    const std::vector<OpenGLVec2PropertySharedPtr>&   getVec2Properties() const;
    const std::vector<OpenGLVec3PropertySharedPtr>&   getVec3Properties() const;
    const std::vector<OpenGLMat4PropertySharedPtr>&   getMatrix4Properties() const;
private:
    OpenGLMaterial(const int id);
    OpenGLMaterial(const OpenGLMaterial& rhs);
    ~OpenGLMaterial();
    template <typename T>
    void setPropertyImpl(
        std::vector<std::shared_ptr<OpenGLMaterialProperty<T>>>& uniforms,
        std::shared_ptr<IMaterialProperty<T>>& p, 
        const T& v);
    int id;
    vector<const ITexture*> textures;
    vector<const ITextureCubemap*> textureCubemaps;

    std::vector<OpenGLIntPropertySharedPtr>   iUniforms;
    std::vector<OpenGLUintPropertySharedPtr>  uiUniforms;
    std::vector<OpenGLFloatPropertySharedPtr> fUniforms;
    std::vector<OpenGLVec2PropertySharedPtr>  v2Uniforms;
    std::vector<OpenGLVec3PropertySharedPtr>  v3Uniforms;
    std::vector<OpenGLMat4PropertySharedPtr>  m4Uniforms;

    template <typename T1, typename T2>
    void getProperty(T1 & uniforms, const std::string name, std::shared_ptr<IMaterialProperty<T2>>& materialProperty);

    template <typename T>
    void addProperty(std::vector<std::shared_ptr<T>> & uniforms, const std::string name, const int location);

    template <typename T>
    void copyProperties(T& lhsUniforms, const T& rhsUniforms);
};


template<typename T>
inline void OpenGLMaterial::setPropertyImpl(
    std::vector<std::shared_ptr<OpenGLMaterialProperty<T>>>& uniforms,
    std::shared_ptr<IMaterialProperty<T>>& p, 
    const T & v)
{
    if (p)
    {
        OpenGLMaterialProperty<T>* glProperty =
            reinterpret_cast<OpenGLMaterialProperty<T>*>(p.get());
        uniforms[glProperty->internalLocation]->value = v;
    }
}

template<typename T1, typename T2>
inline void OpenGLMaterial::getProperty(T1 & uniforms, const std::string name, std::shared_ptr<IMaterialProperty<T2>>& materialProperty)
{
    materialProperty = 0;
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
    p->internalLocation = uniforms.size();
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
