#include "Core/CameraNode.hpp"
#include "Math/Quarternion.hpp"

CameraNode::CameraNode(ICamera& camera) : Node(NodeType::Camera), camera(camera)
{
}

ICamera& CameraNode::getCamera()
{
    return camera;
}

void CameraNode::setPosition(const Vec3 & pos)
{
    camera.setPosition(pos);
}

Vec3 CameraNode::getPosition() const
{
    return camera.getPosition();
}

void CameraNode::setModelMatrix(const Matrix4 & model)
{
}

Matrix4 CameraNode::getModelMatrix()
{
    return Matrix4();
}

void CameraNode::scale(const Vec3 & scale)
{
}

void CameraNode::rotate(const float angle, const Vec3 & axis)
{
    Quarternion rot = Quarternion::makeRotation(angle, axis);
    Vec3 uv = camera.getUpVector();
    Vec3 ld = camera.getLookDirection().normalize();
    float len = camera.getLookDirection().length();
    camera.setUpVector(rot.rotate(uv));
    camera.setLookDirection(camera.getPosition() + rot.rotate(ld) * len);
    camera.setPosition(rot.rotate(camera.getPosition()));
}

void CameraNode::rotate(const Vec3 & rotation)
{
    Matrix3 m;
    m.setRotation(rotation.x, rotation.y, rotation.z);
    Vec3 ld = m * camera.getLookDirection().normalize();
    Vec3 uv = m * camera.getUpVector().normalize();

    camera.setLookDirection(camera.getPosition() + ld);
    camera.setUpVector(uv);
    camera.setPosition(m * camera.getPosition());
}

void CameraNode::translate(const Vec3 & translation)
{
    camera.setPosition(camera.getPosition() + translation);
    camera.setLookDirection(camera.getPosition() + camera.getLookDirection() + translation);
}

void CameraNode::setDirVector(const Vec3& dir)
{
    camera.setLookDirection(dir);
}

Vec3 CameraNode::getDirVector() const
{
    return camera.getLookDirection().normalize();
}

void CameraNode::setUpVector(const Vec3& up)
{
    camera.setUpVector(up);
}

Vec3 CameraNode::getUpVector() const
{
    return camera.getUpVector();
}