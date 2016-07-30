#pragma once

#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"
#include "Core/NodeTypes.hpp"
#include <vector>

class INode
{
public:
    virtual ~INode() {}
    virtual void setModelMatrix(const Matrix4& model)     = 0;
    virtual Matrix4 getModelMatrix() const                = 0;
    virtual Vec3 getPosition() const                      = 0;
    virtual NodeType getNodeType() const                  = 0;

    // scale.x - Scale coefficient in X-direction
    // scale.y - Scale coefficient in Y-direction
    // scale.z - Scale coefficient in Z-direction
    virtual void scale(const Vec3& scale) = 0;

    // rotation.x - Angle of rotation around X-axis (in radians)
    // rotation.y - Angle of rotation around Y-axis (in radians)
    // rotation.z - Angle of rotation around Z-axis (in radians)
    virtual void rotate(const Vec3& rotation) = 0;

    // translation.x - Translation in X-direction
    // translation.y - Translation in Y-direction
    // translation.z - Translation in Z-direction
    virtual void translate(const Vec3& translation) = 0;
};