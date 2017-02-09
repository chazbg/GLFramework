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
    Quarternion rot        = Quarternion::makeRotation(angle, axis);
    Vec3  cameraUp         = camera.getUpVector();
    Vec3  cameraPos        = camera.getPosition();
    Vec3  cameraTarget     = camera.getTarget();

    camera.setUpVector(rot.rotate(cameraUp));
    camera.setTarget(rot.rotate(cameraTarget));
    camera.setPosition(rot.rotate(cameraPos));
}

void CameraNode::rotate(const Vec3 & rotation)
{
    Matrix3 m;
    m.setRotation(rotation.x, rotation.y, rotation.z);

    camera.setTarget(m * camera.getTarget());
    camera.setUpVector(m * camera.getUpVector());
    camera.setPosition(m * camera.getPosition());
}

void CameraNode::translate(const Vec3 & translation)
{
    camera.setPosition(camera.getPosition() + translation);
    camera.setTarget(camera.getTarget() + translation);
}

void CameraNode::setTarget(const Vec3& target)
{
    camera.setTarget(target);
}

Vec3 CameraNode::getTarget() const
{
    return camera.getTarget();
}

Vec3 CameraNode::getViewDirection() const
{
    return camera.getViewDirection();
}

void CameraNode::setUpVector(const Vec3& up)
{
    camera.setUpVector(up);
}

Vec3 CameraNode::getUpVector() const
{
    return camera.getUpVector();
}