#include "Core/Node.hpp"
#include "Math/GeometryAlgorithm.hpp"

Node::Node(const NodeType type) : type(type), model(), parent(0)
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

Matrix4 Node::getModelToWorldMatrix() const
{
    if (parent)
    {
        return parent->getModelToWorldMatrix() * model;
    }
    else
    {
        return model;
    }
}

Vec3 Node::getPosition() const
{
    Vec4 base(0.0f, 0.0f, 0.0f, 1.0f);
    base = getModelToWorldMatrix() * base;
    return Vec3(base.x, base.y, base.z);
}

NodeType Node::getNodeType() const
{
    return type;
}

void Node::setParent(std::shared_ptr<INode> parent)
{
    this->parent = parent;
}

std::shared_ptr<INode> Node::getParent()
{
    return parent;
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
