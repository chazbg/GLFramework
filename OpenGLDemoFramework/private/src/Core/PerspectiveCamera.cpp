#include <Core/PerspectiveCamera.hpp>
#include <Math/GeometryAlgorithm.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far) : up(0.0f, 1.0f, 0.0f)
{
    perspective = GeometryAlgorithm::CreatePerspectiveMatrix(fov, aspectRatio, near, far);
}

void PerspectiveCamera::setPosition(const Vec3 & position)
{
    pos = position;
}

Vec3 PerspectiveCamera::getPosition() const
{
    return pos;
}

void PerspectiveCamera::setTarget(const Vec3& position)
{
    target = position;
}

Vec3 PerspectiveCamera::getTarget() const
{
    return target;
}

Vec3 PerspectiveCamera::getViewDirection() const
{
    return (target - pos).normalize();
}

Matrix4 PerspectiveCamera::getViewMatrix() const
{
    return GeometryAlgorithm::CreateLookAtMatrix(pos, target, up);
}

Matrix4 PerspectiveCamera::getViewProjectionMatrix() const
{

    return perspective * getViewMatrix();
}

Vec3 PerspectiveCamera::getUpVector() const
{
    return up;
}

void PerspectiveCamera::setUpVector(const Vec3 & upVector)
{
    up = upVector;
}
