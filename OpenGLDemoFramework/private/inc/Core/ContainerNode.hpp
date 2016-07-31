#pragma once

#include "Core/Node.hpp"
#include <memory>

class ContainerNode : public Node
{
public:
    ContainerNode();
    void addChild(std::shared_ptr<INode> child);
    std::vector<std::shared_ptr<INode>>& getChildren();

    virtual void setModelMatrix(const Matrix4& model);
    virtual void scale(const Vec3& scale);
    virtual void rotate(const float angle, const Vec3& axis);
    virtual void rotate(const Vec3& rotation);
    virtual void translate(const Vec3& translation);
private:
    std::vector<std::shared_ptr<INode>> children;
};