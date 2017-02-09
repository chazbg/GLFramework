#pragma once

#include <Core/ICamera.hpp>

class PerspectiveCamera : public ICamera
{
public:
    PerspectiveCamera(float fov, float aspectRatio, float near, float far);
    virtual void setPosition(const Vec3& position);
    virtual Vec3 getPosition() const;
    virtual void setTarget(const Vec3& position);
    virtual Vec3 getTarget() const;
    virtual Vec3 getViewDirection() const;
    virtual Matrix4 getViewMatrix() const;
    virtual Matrix4 getViewProjectionMatrix() const;
    virtual Vec3 getUpVector() const;
    virtual void setUpVector(const Vec3& upVector);
private:
    Vec3 pos;
    Vec3 target;
    Vec3 up;
    Matrix4 perspective;
};