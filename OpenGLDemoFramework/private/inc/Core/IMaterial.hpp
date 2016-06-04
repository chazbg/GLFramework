#pragma once
#include "Core/ITexture.hpp"
#include "Core/ITextureCubemap.hpp"
#include "Core/IMaterialProperty.hpp"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>

class IMaterial
{
public:
    virtual const std::vector<const ITexture*>&        getTextures() const = 0;
    virtual const std::vector<const ITextureCubemap*>& getTextureCubemaps() const = 0;
    virtual void addTexture(const ITexture* tex) = 0;
    virtual void addTextureCubemap(const ITextureCubemap* tex) = 0;
    virtual void removeTexture(const ITexture* tex) = 0;
    virtual void removeTextureCubemap(const ITextureCubemap* tex) = 0;
    virtual void getProperty(const std::string name, IntPropertySharedPtr& materialProperty) = 0;
    virtual void getProperty(const std::string name, UintPropertySharedPtr& materialProperty) = 0;
    virtual void getProperty(const std::string name, FloatPropertySharedPtr& materialProperty) = 0;
    virtual void getProperty(const std::string name, Vec2PropertySharedPtr& materialProperty) = 0;
    virtual void getProperty(const std::string name, Vec3PropertySharedPtr& materialProperty) = 0;
    virtual void getProperty(const std::string name, Mat4PropertySharedPtr& materialProperty) = 0;
    virtual void setProperty(IntPropertySharedPtr,   const int v) = 0;
    virtual void setProperty(UintPropertySharedPtr,  const unsigned int v) = 0;
    virtual void setProperty(FloatPropertySharedPtr, const float v) = 0;
    virtual void setProperty(Vec2PropertySharedPtr,  const Vec2& v) = 0;
    virtual void setProperty(Vec3PropertySharedPtr,  const Vec3& v) = 0;
    virtual void setProperty(Mat4PropertySharedPtr,  const Matrix4& v) = 0;
protected:
    virtual ~IMaterial() {}
};