#include "Core/Node.hpp"
#include "Math/GeometryAlgorithm.hpp"

Node::Node(const NodeType type) : type(type)
{
}

void Node::setModelMatrix(const Matrix4 & model)
{
    this->model = model;
}

Matrix4 Node::getModelMatrix() const
{
    return model;
}

Vec3 Node::getPosition() const
{
    Vec4 base(0.0f, 0.0f, 0.0f, 1.0f);
    base = model * base;
    return Vec3(base.x, base.y, base.z);
}

NodeType Node::getNodeType() const
{
    return type;
}

void Node::scale(const Vec3 & scale)
{
    Matrix4 m = GeometryAlgorithm::CreateSRTMatrix(scale, Vec3(0, 0, 0), Vec3(0, 0, 0));
    model = m * model;
}

void Node::rotate(const Vec3 & rotation)
{
    Matrix4 m;
    m.setRotation(rotation.x, rotation.y, rotation.z);
    model = m * model;
}

void Node::translate(const Vec3 & translation)
{
    Matrix4 m;
    m.setTranslation(translation);
    model = m * model;
}
