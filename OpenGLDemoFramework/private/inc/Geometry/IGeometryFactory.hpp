#pragma once

#include "Core/IResourceManager.hpp"
#include "Core/MeshNode.hpp"
#include <memory>

class IGeometryFactory
{
public:
    virtual std::shared_ptr<MeshNode> createCustomGeometry(const std::string fileName) = 0;
    virtual std::shared_ptr<MeshNode> createCustomGeometry(const std::string fileName, const bool flipFaces) = 0;
    virtual std::shared_ptr<MeshNode> createRectangle() = 0;
    virtual std::shared_ptr<MeshNode> createBlockMesh() = 0;
    virtual std::shared_ptr<MeshNode> createPlaneMesh(const unsigned int width, const unsigned int height) = 0;
};