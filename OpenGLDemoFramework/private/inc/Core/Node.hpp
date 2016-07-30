#pragma once

#include "Core/INode.hpp"

class Node : public INode
{
public:
    Node(const NodeType type);
    virtual void setModelMatrix(const Matrix4& model);
    virtual Matrix4 getModelMatrix() const;
    virtual Vec3 getPosition() const;
    virtual NodeType getNodeType() const;

    // scale.x - Scale coefficient in X-direction
    // scale.y - Scale coefficient in Y-direction
    // scale.z - Scale coefficient in Z-direction
    virtual void scale(const Vec3& scale);

    // rotation.x - Angle of rotation around X-axis (in radians)
    // rotation.y - Angle of rotation around Y-axis (in radians)
    // rotation.z - Angle of rotation around Z-axis (in radians)
    virtual void rotate(const Vec3& rotation);

    // translation.x - Translation in X-direction
    // translation.y - Translation in Y-direction
    // translation.z - Translation in Z-direction
    virtual void translate(const Vec3& translation);
private:
    NodeType type;
    Matrix4 model;
};