#pragma once

#include "Core/Node.hpp"
#include "Core/ICamera.hpp"
#include <memory>

class CameraNode : public Node
{
public:
    CameraNode(ICamera& camera);
    ICamera& getCamera();

    void setPosition(const Vec3& pos);
    virtual Vec3 getPosition() const;
    virtual void setModelMatrix(const Matrix4& model);
    virtual Matrix4 getModelMatrix();
    virtual void scale(const Vec3& scale);
    virtual void rotate(const float angle, const Vec3& axis);
    virtual void rotate(const Vec3& rotation);
    virtual void translate(const Vec3& translation);
    virtual void setDirVector(const Vec3& dir);
    virtual Vec3 getDirVector() const;
    virtual void setUpVector(const Vec3& up);
    virtual Vec3 getUpVector() const;
private:
    ICamera& camera;
};