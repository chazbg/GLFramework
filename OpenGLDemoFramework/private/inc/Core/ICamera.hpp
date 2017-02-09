#pragma once

#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

class ICamera
{
public:
    virtual ~ICamera() {}
    virtual void setPosition(const Vec3& position) = 0;
    virtual Vec3 getPosition() const = 0;
    virtual void setTarget(const Vec3& position) = 0;
    virtual Vec3 getTarget() const = 0;
    virtual Vec3 getViewDirection() const = 0;
    virtual Matrix4 getViewMatrix() const = 0;
    virtual Matrix4 getViewProjectionMatrix() const = 0;
    virtual Vec3 getUpVector() const = 0;
    virtual void setUpVector(const Vec3& upVector) = 0;
};