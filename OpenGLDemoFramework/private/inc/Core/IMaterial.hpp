#pragma once
#include <vector>
#include <map>
#include <string>
#include "Core/Texture.hpp"
#include "Core/TextureCubemap.hpp"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

class IMaterial
{
public:
    virtual ~IMaterial() {}
    virtual int getId() const = 0;
    virtual const std::vector<const Texture*>&        getTextures() const = 0;
    virtual const std::vector<const TextureCubemap*>& getTextureCubemaps() const = 0;
    virtual void addTexture(const Texture* tex) = 0;
    virtual void addTextureCubemap(const TextureCubemap* tex) = 0;
    virtual void removeTexture(const Texture* tex) = 0;
    virtual void removeTextureCubemap(const TextureCubemap* tex) = 0;
    virtual const std::map<std::string, int>&          getIntProperties() const = 0; //Refactor the many similar calls.
    virtual const std::map<std::string, unsigned int>& getUintProperties() const = 0; //Refactor the many similar calls.
    virtual const std::map<std::string, float>&        getFloatProperties() const = 0;//Refactor the many similar calls.
    virtual const std::map<std::string, Vec3>&         getVec3Properties() const = 0;//Refactor the many similar calls.
    virtual const std::map<std::string, Matrix4>&      getMatrix4Properties() const = 0;//Refactor the many similar calls.
    virtual void setProperty(const std::string name, const int v) = 0;
    virtual void setProperty(const std::string name, const unsigned int v) = 0;
    virtual void setProperty(const std::string name, const float v) = 0;
    virtual void setProperty(const std::string name, const Vec3& v) = 0;
    virtual void setProperty(const std::string name, const Matrix4& v) = 0;
};