#pragma once

#include "Core/Node.hpp"
#include "Core/IMesh.hpp"

class MeshNode : public Node
{
public:
    MeshNode(std::shared_ptr<IMesh> mesh);
    MeshNode(const MeshNode& rhs);
    std::shared_ptr<IMesh> getMesh();
    void addChild(std::shared_ptr<MeshNode> child);
    std::vector<std::shared_ptr<MeshNode>>& getChildren();
    void setMaterial(IMaterial* material);

    virtual void setModelMatrix(const Matrix4& model);
    virtual void scale(const Vec3& scale);
    virtual void rotate(const Vec3& rotation);
    virtual void translate(const Vec3& translation);
private:
    std::shared_ptr<IMesh> mesh;
    std::vector<std::shared_ptr<MeshNode>> children;
};