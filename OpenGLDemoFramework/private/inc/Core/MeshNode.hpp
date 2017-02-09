#pragma once

#include "Core/Node.hpp"
#include "Core/IMesh.hpp"

class MeshNode : public Node
{
public:
    MeshNode();
    MeshNode(const MeshNode& rhs);
    void addMesh(std::shared_ptr<IMesh> mesh);
    std::vector<std::shared_ptr<IMesh>>& getMeshes();
    void addChild(std::shared_ptr<MeshNode> child);
    std::vector<std::shared_ptr<MeshNode>>& getChildren();
    void setMaterial(IMaterial* material);

    virtual void setModelMatrix(const Matrix4& model);
    virtual void scale(const Vec3& scale);
    virtual void rotate(const float angle, const Vec3& axis);
    virtual void rotate(const Vec3& rotation);
    virtual void translate(const Vec3& translation);
private:
    std::vector<std::shared_ptr<IMesh>> meshes;
    std::vector<std::shared_ptr<MeshNode>> children;
};