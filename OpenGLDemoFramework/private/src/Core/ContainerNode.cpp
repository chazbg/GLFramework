#include "Core/ContainerNode.hpp"

ContainerNode::ContainerNode() : Node(NodeType::Container)
{
}

void ContainerNode::addChild(shared_ptr<INode> child)
{
    if (nullptr != child)
    {
        children.push_back(child);
    }    
}

std::vector<shared_ptr<INode>>& ContainerNode::getChildren()
{
    return children;
}

void ContainerNode::setModelMatrix(const Matrix4 & model)
{
    Node::setModelMatrix(model);

    for (auto child : children)
    {
        child->setModelMatrix(model);
    }
}

void ContainerNode::scale(const Vec3 & scale)
{
    Node::scale(scale);

    for (auto child : children)
    {
        child->scale(scale);
    }
}

void ContainerNode::rotate(const float angle, const Vec3 & axis)
{
    //Node::rotate(rotation);

    //for (auto child : children)
    //{
    //    child->rotate(rotation);
    //}
}

void ContainerNode::rotate(const Vec3 & rotation)
{
    Node::rotate(rotation);

    for (auto child : children)
    {
        child->rotate(rotation);
    }
}

void ContainerNode::translate(const Vec3 & translation)
{
    Node::translate(translation);

    for (auto child : children)
    {
        child->translate(translation);
    }
}
