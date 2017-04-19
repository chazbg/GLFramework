#include "Core/OrthographicCamera.hpp"
#include "Math/GeometryAlgorithm.hpp"

OrthographicCamera::OrthographicCamera(float near, float far, float left, float right, float top, float bottom)
{
    projection = GeometryAlgorithm::CreateOrthographicMatrix(near, far, left, right, top, bottom);
}

void OrthographicCamera::setPosition(const Vec3 & position)
{
    pos = position;
}

Vec3 OrthographicCamera::getPosition() const
{
    return pos;
}

void OrthographicCamera::setTarget(const Vec3 & position)
{
    target = position;
}

Vec3 OrthographicCamera::getTarget() const
{
    return target;
}

Vec3 OrthographicCamera::getViewDirection() const
{
    return (target - pos).normalize();
}

Matrix4 OrthographicCamera::getViewMatrix() const
{
    return GeometryAlgorithm::CreateLookAtMatrix(pos, target, up);
}

Matrix4 OrthographicCamera::getViewProjectionMatrix() const
{
    return projection * getViewMatrix();
}

Vec3 OrthographicCamera::getUpVector() const
{
    return up;
}

void OrthographicCamera::setUpVector(const Vec3 & upVector)
{
    up = upVector;
}
