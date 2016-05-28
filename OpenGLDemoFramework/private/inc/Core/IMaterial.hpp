#pragma once
#include <vector>
#include <map>
#include <string>
#include "Core/ITexture.hpp"
#include "Core/ITextureCubemap.hpp"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

class IMaterial
{
public:
    virtual const std::vector<const ITexture*>&        getTextures() const = 0;
    virtual const std::vector<const ITextureCubemap*>& getTextureCubemaps() const = 0;
    virtual void addTexture(const ITexture* tex) = 0;
    virtual void addTextureCubemap(const ITextureCubemap* tex) = 0;
    virtual void removeTexture(const ITexture* tex) = 0;
    virtual void removeTextureCubemap(const ITextureCubemap* tex) = 0;
    virtual void setProperty(const std::string name, const int v) = 0;
    virtual void setProperty(const std::string name, const unsigned int v) = 0;
    virtual void setProperty(const std::string name, const float v) = 0;
    virtual void setProperty(const std::string name, const Vec2& v) = 0;
    virtual void setProperty(const std::string name, const Vec3& v) = 0;
    virtual void setProperty(const std::string name, const Matrix4& v) = 0;
protected:
    virtual ~IMaterial() {}
};