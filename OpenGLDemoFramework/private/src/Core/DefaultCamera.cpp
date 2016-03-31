#include <Core/DefaultCamera.hpp>

DefaultCamera::DefaultCamera()
{

}

DefaultCamera::~DefaultCamera()
{

}

void DefaultCamera::setPosition(const Vec3& position)
{
}

Vec3 DefaultCamera::getPosition() const
{
    return Vec3();
}

void DefaultCamera::setLookDirection(const Vec3& position)
{
}

Vec3 DefaultCamera::getLookDirection() const
{
    return Vec3();
}

Matrix4 DefaultCamera::getViewMatrix() const
{
    return Matrix4();
}

Matrix4 DefaultCamera::getViewProjectionMatrix() const
{
    return Matrix4();
}

Vec3 DefaultCamera::getUpVector() const
{
    return Vec3();
}

void DefaultCamera::setUpVector(const Vec3 & upVector)
{
}
