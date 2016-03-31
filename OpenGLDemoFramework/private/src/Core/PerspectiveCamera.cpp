#include <Core/PerspectiveCamera.hpp>
#include <Math/GeometryAlgorithm.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far) : upVector(0, 1, 0)
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

void PerspectiveCamera::setLookDirection(const Vec3 & position)
{
    lookAt = position;
}

Vec3 PerspectiveCamera::getLookDirection() const
{
    return lookAt;
}

Matrix4 PerspectiveCamera::getViewMatrix() const
{
    return GeometryAlgorithm::CreateLookAtMatrix(pos, lookAt, Vec3(0, 1, 0));   
}

Matrix4 PerspectiveCamera::getViewProjectionMatrix() const
{

    return perspective * GeometryAlgorithm::CreateLookAtMatrix(pos, lookAt, Vec3(0, 1, 0));
}

Vec3 PerspectiveCamera::getUpVector() const
{
    return upVector;
}

void PerspectiveCamera::setUpVector(const Vec3 & upVector)
{
    this->upVector = upVector;
}
