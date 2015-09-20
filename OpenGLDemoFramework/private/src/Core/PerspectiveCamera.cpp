#include <Core/PerspectiveCamera.hpp>
#include <Math/GeometryAlgorithm.hpp>

PerspectiveCamera::PerspectiveCamera()
{
    perspective = GeometryAlgorithm::CreatePerspectiveMatrix(3.14f / 4.0f, 1.0f, 10.0f, 100);
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
