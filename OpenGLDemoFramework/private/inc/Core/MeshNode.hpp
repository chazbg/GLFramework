#pragma once

#include "Core/Node.hpp"
#include "Core/IMesh.hpp"

class MeshNode;

typedef std::shared_ptr<MeshNode> MeshNodeSharedPtr;

class MeshNode : public Node
{
public:
    MeshNode();
    MeshNode(const MeshNode& rhs);
    void                            addMesh(IMeshSharedPtr mesh);
    std::vector<IMeshSharedPtr>&    getMeshes();
    void                            addChild(MeshNodeSharedPtr child);
    std::vector<MeshNodeSharedPtr>& getChildren();
    void                            setMaterial(IMaterial* material);

    virtual void setModelMatrix(const Matrix4& model);
    virtual void scale(const Vec3& scale);
    virtual void rotate(const float angle, const Vec3& axis);
    virtual void rotate(const Vec3& rotation);
    virtual void translate(const Vec3& translation);
private:
    std::vector<IMeshSharedPtr>    meshes;
    std::vector<MeshNodeSharedPtr> children;
};
