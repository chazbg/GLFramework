#include "Core/MeshNode.hpp"
#include "Math/Quarternion.hpp"

MeshNode::MeshNode() : Node(NodeType::Mesh)
{
}

MeshNode::MeshNode(const MeshNode & rhs) : Node(NodeType::Mesh)
{
    for (auto mesh : rhs.meshes)
    {
        meshes.push_back(mesh->clone());
    }

    for (auto child : rhs.children)
    {
        addChild(std::shared_ptr<MeshNode>(new MeshNode(*child)));
    }
}

void MeshNode::addMesh(std::shared_ptr<IMesh> mesh)
{
    if (nullptr != mesh)
    {
        meshes.push_back(mesh);
    }
}

std::vector<std::shared_ptr<IMesh>>& MeshNode::getMeshes()
{
    return meshes;
}

void MeshNode::addChild(shared_ptr<MeshNode> child)
{
    if (nullptr != child)
    {
        children.push_back(child);
    }    
}

std::vector<shared_ptr<MeshNode>>& MeshNode::getChildren()
{
    return children;
}

void MeshNode::setMaterial(IMaterial * material)
{
    for (auto mesh : meshes)
    {
        mesh->setMaterial(material);
    }

    for (auto child : children)
    {
        child->setMaterial(material);
    }
}

void MeshNode::setModelMatrix(const Matrix4 & model)
{
    Node::setModelMatrix(model);
}

void MeshNode::scale(const Vec3 & scale)
{
    Node::scale(scale);
}

void MeshNode::rotate(const float angle, const Vec3 & axis)
{
    Quarternion rot = Quarternion::makeRotation(angle, axis);
    Matrix4 rotation = rot.toMatrix().toMatrix4();

    Node::setModelMatrix(rotation * getModelMatrix());
}

void MeshNode::rotate(const Vec3 & rotation)
{
    Node::rotate(rotation);
}

void MeshNode::translate(const Vec3 & translation)
{
    Node::translate(translation);
}
