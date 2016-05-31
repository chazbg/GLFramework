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
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<int>>& materialProperty) = 0;
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<unsigned int>>& materialProperty) = 0;
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<float>>& materialProperty) = 0;
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<Vec2>>& materialProperty) = 0;
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<Vec3>>& materialProperty) = 0;
    virtual void getProperty(const std::string name, std::shared_ptr<IMaterialProperty<Matrix4>>& materialProperty) = 0;
    virtual void setProperty(std::shared_ptr<IMaterialProperty<int>>         , const int v) = 0;
    virtual void setProperty(std::shared_ptr<IMaterialProperty<unsigned int>>, const unsigned int v) = 0;
    virtual void setProperty(std::shared_ptr<IMaterialProperty<float>>       , const float v) = 0;
    virtual void setProperty(std::shared_ptr<IMaterialProperty<Vec2>>        , const Vec2& v) = 0;
    virtual void setProperty(std::shared_ptr<IMaterialProperty<Vec3>>        , const Vec3& v) = 0;
    virtual void setProperty(std::shared_ptr<IMaterialProperty<Matrix4>>     , const Matrix4& v) = 0;
protected:
    virtual ~IMaterial() {}
};